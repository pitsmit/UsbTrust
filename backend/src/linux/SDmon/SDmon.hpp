#pragma once

#include <functional>
#include <systemd/sd-device.h>

#include "linux/SDdevView/SDdevView.hpp"

class SDmon {
  public:
    explicit SDmon();
    ~SDmon();
    using Handler = std::function<void(SDdevView)>;

    void applyFilter();
    void start(Handler h);
    static int monitorCallback(sd_device_monitor *, sd_device *dev, void *userdata);

  private:
    sd_device_monitor *mon = nullptr;
    Handler handler_;
};