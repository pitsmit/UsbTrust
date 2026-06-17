#pragma once

#include "EventQueue.hpp"
#include "services/DeviceEventService.hpp"
#include "infrastructure/logging/DevLogger.hpp"
#include "entities/DeviceEvent.hpp"

class EventLoop {
private:
    EventQueue<DeviceEvent>& queue_;
    DeviceEventService& service_;

public:
    EventLoop(
        EventQueue<DeviceEvent>& queue,
        DeviceEventService& service) :
        queue_(queue),
        service_(service) {}

    void run()
    {
        while (auto event = queue_.pop()) {
            try {
                service_.handle(*event);
            } catch (const std::exception& ex) {
                mylog->error(
                    "Failed to handle event for {}: {}",
                    event->devNode,
                    ex.what()
                );
            }
        }
    }
};