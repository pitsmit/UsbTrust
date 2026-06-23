#include "Watcher.hpp"

#include "linux/SDact/SDact.hpp"
#include "linux/SDmon/SDmon.hpp"

void Watcher::run() {
    SDmon mon;
    mon.applyFilter();
    mon.start([this](SDdevView dev) {
        SDact action(dev);
        if (auto event = action.toEvent()) {
            queue_.push(std::move(*event));
        }
    });
}