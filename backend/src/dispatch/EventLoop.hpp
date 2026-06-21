#pragma once

#include "EventQueue.hpp"
#include "entities/DeviceEvent.hpp"
#include "infrastructure/logging/DevLogger.hpp"
#include "services/DeviceEventService/DeviceEventService.hpp"

class EventLoop {
  private:
    EventQueue<DeviceEvent> &queue_;
    DeviceEventService &service_;

  public:
    EventLoop(EventQueue<DeviceEvent> &queue, DeviceEventService &service)
        : queue_(queue), service_(service) {}

    void run() {
        while (auto event = queue_.pop()) {
            try {
                service_.handle(*event);
            } catch (const std::exception &ex) {
                mylog->error("Failed to handle event for {}: {}", event->devNode, ex.what());
            }
        }
    }
};