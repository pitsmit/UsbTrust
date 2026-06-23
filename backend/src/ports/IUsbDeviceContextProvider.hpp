#pragma once

#include <vector>

#include "types/types.hpp"

class DeviceInfo;
class MountMode;

class IUsbDeviceContextProvider {
  public:
    virtual ~IUsbDeviceContextProvider() = default;
    virtual DeviceInfo getDeviceInfo(const core::path &devNode) = 0;
    virtual std::vector<core::path> getUsbDevNodes() = 0;
    virtual core::path getMountPoint(const core::path &devNode) = 0;
    virtual MountMode getMountMode(const core::path &mountpoint) = 0;
};