#pragma once

#include "entities/MountRecord.hpp"
#include "managers/DeviceManager/DeviceManager.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"
#include "services/MountService/MountService.hpp"

class MountManager {
  private:
    DeviceManager &deviceManager;
    MountService &mountService;
    IUsbDeviceContextProvider &resolver;

  public:
    MountManager(DeviceManager &dm, MountService &ms, IUsbDeviceContextProvider &rs)
        : deviceManager(dm), mountService(ms), resolver(rs) {}

    MountRecord mount(const core::path &devNode);
    void unmount(const core::path &mountPoint);
    void remount(const MountRecord &record);
};