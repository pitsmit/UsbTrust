#pragma once

#include <systemd/sd-device.h>

#include "dispatch/EventQueue.hpp"
#include "entities/DeviceEvent.hpp"

class Watcher {
    EventQueue<DeviceEvent> &queue_;

  public:
    explicit Watcher(EventQueue<DeviceEvent> &queue) : queue_(queue) {}

    void run();
    static int monitorCallback(sd_device_monitor *, sd_device *dev, void *userdata);
};