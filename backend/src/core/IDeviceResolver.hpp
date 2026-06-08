#pragma once

class DeviceInfo;
class MountMode;

class IDeviceResolver {
public:
    virtual ~IDeviceResolver() = default;
    virtual DeviceInfo resolve(std::string_view devNode) = 0;
    virtual std::vector<std::string> getUsbDevNodes() = 0;
    virtual std::string getMountPoint(const std::string& devNode) = 0;
    virtual MountMode getMountMode(const std::string& mountpoint) = 0;
};