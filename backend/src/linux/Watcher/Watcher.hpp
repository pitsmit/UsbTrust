#pragma once

#include "dispatch/EventQueue.hpp"
#include "entities/DeviceEvent.hpp"

class Watcher {
    EventQueue<DeviceEvent> &queue_;

  public:
    explicit Watcher(EventQueue<DeviceEvent> &queue) : queue_(queue) {}

    void run();
};