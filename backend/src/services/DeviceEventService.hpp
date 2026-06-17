#pragma once

#include "managers/MountRegistry.hpp"
#include "managers/MountManager.hpp"
#include "ports/IDeviceResolver.hpp"
#include "managers/DeviceEventNotifyManager.hpp"

class DeviceEvent;

class DeviceEventService {
private:
    MountRegistryManager& mountRegistry_;
    MountManager& mountManager_;
    DeviceEventNotifyManager& notifier_;
    IDeviceResolver& resolver_;

public:
    DeviceEventService(
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

    void handle(const DeviceEvent& event);
};