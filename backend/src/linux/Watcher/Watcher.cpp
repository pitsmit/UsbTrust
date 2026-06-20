#include "Watcher.hpp"

#include "linux/SDact/SDact.hpp"
#include "linux/SDdevView/SDdevView.hpp"
#include "linux/SDmon/SDmon.hpp"

void Watcher::run() {
    SDmon mon;
    mon.applyFilter();
    mon.start(monitorCallback, this);
    while (true)
        pause();
}

int Watcher::monitorCallback(sd_device_monitor *, sd_device *dev, void *userdata) {
    auto *self = static_cast<Watcher *>(userdata);
    SDdevView device(dev);
    SDact action(device);
    if (!action.isValid())
        return 0;
    if (auto event = action.toEvent(device)) {
        self->queue_.push(std::move(*event));
    }
    return 0;
}