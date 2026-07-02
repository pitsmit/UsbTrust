#include "DeviceEventService.hpp"

#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "infrastructure/logging/DevLogger.hpp"

void DeviceEventService::handle(const DeviceEvent &event) {
    mylog->info("Start handle {} event with devnode {}",
                event.type == EventType::INSERT ? "INSERT" : "REMOVE",
                event.devNode.c_str());

    if (event.type == EventType::INSERT) {
        auto record = coordinator.mount(event.devNode);
        notifier_.notifyInsert(record);
    } else if (event.type == EventType::REMOVE) {
        auto mountPoint = resolver_.getMountPoint(event.devNode);
        coordinator.unmount(mountPoint, event.devNode);
        notifier_.notifyRemove(mountPoint);
    }
}