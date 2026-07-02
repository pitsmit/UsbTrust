#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T> class EventQueue {
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

  public:
    void push(T value);
    std::optional<T> pop();
};

template <typename T> void EventQueue<T>::push(T value) {
    {
        std::lock_guard lock(mutex_);
        queue_.push(std::move(value));
    }
    cv_.notify_one();
}

template <typename T> std::optional<T> EventQueue<T>::pop() {
    std::unique_lock lock(mutex_);
    cv_.wait(lock, [&] { return !queue_.empty(); });
    if (queue_.empty()) {
        return std::nullopt;
    }
    T value = std::move(queue_.front());
    queue_.pop();
    return value;
}