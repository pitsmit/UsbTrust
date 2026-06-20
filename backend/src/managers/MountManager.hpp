#pragma once

#include "DeviceManager.hpp"
#include "entities/MountRecord.hpp"
#include "ports/IDeviceResolver.hpp"
#include "services/MountService.hpp"

class MountManager {
  private:
    DeviceManager &deviceManager;
    MountService &mountService;
    IUsbDeviceContextProvider &resolver;

  public:
    MountManager(DeviceManager &dm, MountService &ms, IUsbDeviceContextProvider &rs)
        : deviceManager(dm), mountService(ms), resolver(rs) {}

    MountRecord mount(std::string_view devNode);
    void unmount(std::string_view mountPoint);
    void remount(const MountRecord &record);
};