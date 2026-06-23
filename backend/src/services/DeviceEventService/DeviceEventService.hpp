#pragma once

#include "managers/DeviceEventNotifyManager/DeviceEventNotifyManager.hpp"
#include "managers/MountManager/MountManager.hpp"
#include "managers/MountRegistryManager/MountRegistryManager.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"

class DeviceEvent;

class DeviceEventService {
  private:
    MountRegistryManager &mountRegistry_;
    MountManager &mountManager_;
    DeviceEventNotifyManager &notifier_;
    IUsbDeviceContextProvider &resolver_;

  public:
    DeviceEventService(MountRegistryManager &mountRegistry,
                       MountManager &mountManager,
                       DeviceEventNotifyManager &notifier,
                       IUsbDeviceContextProvider &resolver)
        : mountRegistry_(mountRegistry), mountManager_(mountManager), notifier_(notifier),
          resolver_(resolver) {}

    void handle(const DeviceEvent &event);
};