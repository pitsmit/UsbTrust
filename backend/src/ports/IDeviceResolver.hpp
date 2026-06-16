#pragma once

#include <string_view>
#include <vector>

class DeviceInfo;
class MountMode;

class IDeviceResolver {
public:
    virtual ~IDeviceResolver() = default;
    virtual DeviceInfo resolve(std::string_view devNode) = 0;
    virtual std::vector<std::string> getUsbDevNodes() = 0;
    virtual std::string getMountPoint(std::string_view devNode) = 0;
    virtual MountMode getMountMode(std::string_view mountpoint) = 0;
};