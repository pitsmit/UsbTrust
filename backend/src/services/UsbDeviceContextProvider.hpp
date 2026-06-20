#pragma once

#include "entities/DeviceInfo.hpp"
#include "ports/IDeviceResolver.hpp"

class UsbDeviceContextProvider : public IUsbDeviceContextProvider {
  public:
    DeviceInfo getDeviceInfo(std::string_view devNode) override;
    std::vector<std::string> getUsbDevNodes() override;
    std::string getMountPoint(std::string_view devNode) override;
    MountMode getMountMode(std::string_view mountpoint) override;
};