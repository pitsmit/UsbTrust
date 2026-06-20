#include "Watcher.hpp"

#include "exceptions/Exceptions.hpp"
#include "linux/SDdevView.hpp"
#include "linux/SDmon.hpp"

void Watcher::run() {
    SDmon mon;
    mon.applyFilter();
    if (sd_device_monitor_start(mon.get(), monitorCallback, this) < 0) {
        throw DeviceMonitorError("Failed to start monitor");
    }
    while (true)
        pause();
}

int Watcher::monitorCallback(sd_device_monitor *, sd_device *dev, void *userdata) {
    auto *self = static_cast<Watcher *>(userdata);
    sd_device_action_t action;
    if (sd_device_get_action(dev, &action) < 0)
        return 0;
    if (action == SD_DEVICE_ADD || action == SD_DEVICE_CHANGE) {
        SDdevView device(dev);
        if (device.isUsbDevice()) {
            if (auto node = device.getDevNode()) {
                auto event =
                    DeviceEventBuilder().withType(EventType::INSERT).withDevNode(*node).build();
                self->queue_.push(event);
            }
        }
    } else if (action == SD_DEVICE_REMOVE) {
        SDdevView device(dev);
        if (auto node = device.getDevNode()) {
            auto event =
                DeviceEventBuilder().withType(EventType::REMOVE).withDevNode(*node).build();
            self->queue_.push(event);
        }
    }
    return 0;
}