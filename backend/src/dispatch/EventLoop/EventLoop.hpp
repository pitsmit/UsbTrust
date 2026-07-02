#pragma once

#include "dispatch/EventQueue/EventQueue.hpp"
#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "services/DeviceEventService/DeviceEventService.hpp"

class EventLoop {
    EventQueue<DeviceEvent> &queue;
    DeviceEventService &service;

  public:
    EventLoop(EventQueue<DeviceEvent> &queue_, DeviceEventService &service_)
        : queue(queue_), service(service_) {}

    void run();
};