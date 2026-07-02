#pragma once

#include "entities/MountRecord/MountRecord.hpp"
#include "managers/DeviceManager/DeviceManager.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"
#include "services/MountService/MountService.hpp"

class MountManager {
    DeviceManager &devices;
    MountService &mounter;
    IUsbDeviceContextProvider &provider;

  public:
    MountManager(DeviceManager &devices_,
                 MountService &mounter_,
                 IUsbDeviceContextProvider &provider_)
        : devices(devices_), mounter(mounter_), provider(provider_) {}

    MountRecord mount(const core::path &devNode);
    void unmount(const core::path &mountPoint);
    void remount(const MountRecord &record);
};