#pragma once

#include <systemd/sd-device.h>

class SDmon {
    sd_device_monitor *mon = nullptr;

  public:
    explicit SDmon();
    ~SDmon();

    void applyFilter();
    void start(sd_device_monitor_handler_t callback, void *userdata);
};