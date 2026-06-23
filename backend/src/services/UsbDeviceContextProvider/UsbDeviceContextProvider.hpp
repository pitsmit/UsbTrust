#pragma once

#include "entities/DeviceInfo.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"
#include "types/types.hpp"

class UsbDeviceContextProvider : public IUsbDeviceContextProvider {
  public:
    DeviceInfo getDeviceInfo(const core::path &devNode) override;
    std::vector<core::path> getUsbDevNodes() override;
    core::path getMountPoint(const core::path &devNode) override;
    MountMode getMountMode(const core::path &mountpoint) override;
};