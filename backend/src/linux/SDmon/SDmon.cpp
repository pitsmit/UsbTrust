#include "SDmon.hpp"

#include "exceptions/Exceptions.hpp"
#include "linux/SDdevView/SDdevView.hpp"

SDmon::SDmon() {
    if (sd_device_monitor_new(&mon) < 0) {
        throw DeviceMonitorError("Failed to create device monitor");
    }
}

SDmon::~SDmon() {
    sd_device_monitor_unref(mon);
}

void SDmon::applyFilter() {
    if (sd_device_monitor_filter_add_match_subsystem_devtype(mon, "block", "partition") < 0) {
        throw DeviceMonitorError("Failed to add monitor filter");
    }
    sd_device_monitor_filter_update(mon);
}

void SDmon::start(Handler h) {
    handler_ = std::move(h);
    if (sd_device_monitor_start(mon, SDmon::monitorCallback, this) < 0) {
        throw DeviceMonitorError("Failed to start monitor");
    }
}

int SDmon::monitorCallback(sd_device_monitor *, sd_device *dev, void *userdata) {
    auto *self = static_cast<SDmon *>(userdata);
    SDdevView view(dev);
    self->handler_(std::move(view));
    return 0;
}