#pragma once

#include <string_view>

#include "DeviceManager.hpp"
#include "MountUtils.hpp"
#include "IDeviceResolver.hpp"
#include "MountPointBuilder.hpp"
#include "MountRecord.hpp"

class MountManager {
private:
    DeviceManager& deviceManager_;
    MountUtils& mountUtils_;
    IDeviceResolver& resolver_;

public:
    MountManager(
        DeviceManager& deviceManager,
        MountUtils& mountUtils,
        IDeviceResolver& resolver
    ) :
        deviceManager_(deviceManager),
        mountUtils_(mountUtils),
        resolver_(resolver)
    {}

    MountRecord mount(std::string_view devNode)
    {
        auto info = resolver_.resolve(devNode);
        auto mountPoint = MountPointBuilder::build(info);
        MountPointBuilder::ensureExists(mountPoint);
        auto id = deviceManager_.isAllowed(info);
        auto mode = MountMode::fromPresence(id);

        mountUtils_.mountDevice(devNode, mountPoint, mode);

        return MountRecordBuilder()
                .withDevNode(devNode)
                .withId(id)
                .withInfo(info)
                .withMountPoint(mountPoint)
                .withMode(mode)
                .build();
    }

    void unmount(std::string_view mountPoint)
    {
        mountUtils_.handleUnmount(mountPoint);
    }

    void remount(const MountRecord &record)
    {
        mountUtils_.remountDevice(
            record.mountPoint,
            record.mode
        );
    }
};