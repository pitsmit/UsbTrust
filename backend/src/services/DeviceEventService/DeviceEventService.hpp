#pragma once

#include "managers/DeviceEventNotifyManager/DeviceEventNotifyManager.hpp"
#include "managers/MountCoordinator/MountCoordinator.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"

class DeviceEvent;

class DeviceEventService {
    DeviceEventNotifyManager &notifier_;
    IUsbDeviceContextProvider &resolver_;
    MountCoordinator &coordinator;

  public:
    DeviceEventService(DeviceEventNotifyManager &notifier,
                       IUsbDeviceContextProvider &resolver,
                       MountCoordinator &coordinator_)
        : notifier_(notifier), resolver_(resolver), coordinator(coordinator_) {}

    void handle(const DeviceEvent &event);
};