#pragma once

#include <optional>
#include <string>
#include <vector>

#include "ports/IDeviceResolver.hpp"
#include "entities/DeviceInfo.hpp"
#include "entities/MountRecord.hpp"

class MockDeviceResolver : public IDeviceResolver {
private:
    DeviceInfo info_;
    bool shouldReturn_ = true;
    std::string mountpoint_;

public:
    void setResult(const DeviceInfo& info)
    {
        info_ = info;
        shouldReturn_ = true;
    }

    void setmountpoint(std::string mt)
    {
        mountpoint_ = mt;
    }

    void setEmpty()
    {
        shouldReturn_ = false;
    }

    DeviceInfo
    resolve(std::string_view /*devNode*/) override
    {
        if (!shouldReturn_) {
            return DeviceInfoBuilder().build();
        }
        return info_;
    }

    std::string getMountPoint(std::string_view devNode)
    {
        return mountpoint_;
    }

    MountMode getMountMode(std::string_view mountpoint)
    {
        return MountMode::ro();
    }

    std::vector<std::string> getUsbDevNodes()
    {
        return {
            "/dev/sdb1",
            "/dev/sdc1",
            "/dev/sdd1"
        };
    }
};