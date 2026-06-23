#pragma once

#include "dispatch/EventQueue.hpp"
#include "entities/DeviceEvent.hpp"

class EventWatcherService {
    EventQueue<DeviceEvent> &queue_;

  public:
    explicit EventWatcherService(EventQueue<DeviceEvent> &queue) : queue_(queue) {}

    void run();
};