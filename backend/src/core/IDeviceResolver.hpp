#pragma once

class DeviceInfo;
enum MODE;

class IDeviceResolver {
public:
    virtual ~IDeviceResolver() = default;
    virtual DeviceInfo resolve(const std::string &devNode) = 0;
    virtual std::vector<std::string> getUsbDevNodes() = 0;
    virtual std::string getMountPoint(const std::string& devNode) = 0;
    virtual MODE getMountMode(const std::string& mountpoint) = 0;
};