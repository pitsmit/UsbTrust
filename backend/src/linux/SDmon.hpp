#pragma once

#include <systemd/sd-device.h>

#include "exceptions/Exceptions.hpp"

class SDmon {
    sd_device_monitor *mon;

  public:
    explicit SDmon() {
        if (sd_device_monitor_new(&mon) < 0) {
            throw DeviceMonitorError("Failed to create device monitor");
        }
    }
    ~SDmon() {
        sd_device_monitor_unref(mon);
    }

    sd_device_monitor *get() const {
        return mon;
    }
};