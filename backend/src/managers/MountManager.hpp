#pragma once

#include "DeviceManager.hpp"
#include "entities/MountRecord.hpp"
#include "ports/IDeviceResolver.hpp"
#include "services/MountService.hpp"

class MountManager {
  private:
    DeviceManager &deviceManager_;
    MountService &mountUtils_;
    IDeviceResolver &resolver_;

  public:
    MountManager(DeviceManager &deviceManager, MountService &mountUtils, IDeviceResolver &resolver)
        : deviceManager_(deviceManager), mountUtils_(mountUtils), resolver_(resolver) {}

    MountRecord mount(std::string_view devNode);
    void unmount(std::string_view mountPoint);
    void remount(const MountRecord &record);
};