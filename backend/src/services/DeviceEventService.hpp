#pragma once

#include "managers/DeviceEventNotifyManager.hpp"
#include "managers/MountManager.hpp"
#include "managers/MountRegistry.hpp"
#include "ports/IDeviceResolver.hpp"

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