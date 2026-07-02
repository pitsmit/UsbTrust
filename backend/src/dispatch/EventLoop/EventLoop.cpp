#include "EventLoop.hpp"

#include "infrastructure/logging/DevLogger.hpp"

void EventLoop::run() {
    while (auto event = queue.pop()) {
        try {
            service.handle(*event);
        } catch (const std::exception &ex) {
            mylog->error("Failed to handle event for {}: {}", event->devNode.c_str(), ex.what());
        }
    }
}