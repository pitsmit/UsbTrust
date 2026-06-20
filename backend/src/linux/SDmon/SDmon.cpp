#include "SDmon.hpp"

#include "exceptions/Exceptions.hpp"

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

void SDmon::start(sd_device_monitor_handler_t callback, void *userdata) {
    if (sd_device_monitor_start(mon, callback, userdata) < 0) {
        throw DeviceMonitorError("Failed to start monitor");
    }
}