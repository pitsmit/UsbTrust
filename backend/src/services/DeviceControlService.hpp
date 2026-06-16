#pragma once

#include "entities/DeviceEvent.hpp"
#include "infrastructure/logging/DevLogger.hpp"
#include "managers/MountRegistry.hpp"
#include "managers/MountManager.hpp"
#include "ports/IDeviceResolver.hpp"
#include "managers/DeviceEventNotifyManager.hpp"

class DeviceControlService {
private:
    MountRegistryManager& mountRegistry_;
    MountManager& mountManager_;
    DeviceEventNotifyManager& notifier_;
    IDeviceResolver& resolver_;

public:
    DeviceControlService(
        MountRegistryManager& mountRegistry,
        MountManager& mountManager,
        DeviceEventNotifyManager& notifier,
        IDeviceResolver& resolver
    ) :
        mountRegistry_(mountRegistry),
        mountManager_(mountManager),
        notifier_(notifier),
        resolver_(resolver)
    {}

    void handleEvent(const DeviceEvent& event) {
        mylog->info("Start handle {} event with devnode {}", 
            event.type == EventType::INSERT ? "INSERT" : "REMOVE", event.devNode);

        if (event.type == EventType::INSERT) {
            auto record = mountManager_.mount(event.devNode);
            mountRegistry_.add(record);
            notifier_.notifyInsert(record);
        }
        else if (event.type == EventType::REMOVE) {
            auto mountPoint = resolver_.getMountPoint(event.devNode);
            mountManager_.unmount(mountPoint);
            notifier_.notifyRemove(mountPoint);
            mountRegistry_.removeByDevNode(event.devNode);
        }
    }
};