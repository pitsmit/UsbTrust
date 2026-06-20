#include "Watcher.hpp"

#include "exceptions/Exceptions.hpp"
#include "linux/SDmon.hpp"

void Watcher::run() {
    SDmon mon;
    if (sd_device_monitor_filter_add_match_subsystem_devtype(mon.get(), "block", "partition") < 0) {
        throw DeviceMonitorError("Failed to add monitor filter");
    }
    sd_device_monitor_filter_update(mon.get());
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
        if (!isUsbDevice(dev))
            return 0;
        const char *devNode = nullptr;
        if (sd_device_get_devname(dev, &devNode) < 0)
            return 0;
        auto event = DeviceEventBuilder().withType(EventType::INSERT).withDevNode(devNode).build();
        self->queue_.push(event);
    } else if (action == SD_DEVICE_REMOVE) {
        const char *devNode = nullptr;
        if (sd_device_get_devname(dev, &devNode) < 0)
            return 0;
        auto event = DeviceEventBuilder().withType(EventType::REMOVE).withDevNode(devNode).build();
        self->queue_.push(event);
    }
    return 0;
}

bool Watcher::isUsbDevice(sd_device *dev) {
    sd_device *parent = nullptr;
    return sd_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device", &parent) >= 0;
}