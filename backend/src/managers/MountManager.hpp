#pragma once

#include <string_view>

#include "DeviceManager.hpp"
#include "services/MountService.hpp"
#include "ports/IDeviceResolver.hpp"
#include "services/MountPointBuilder.hpp"
#include "entities/MountRecord.hpp"
#include "infrastructure/logging/DevLogger.hpp"

class MountManager {
private:
    DeviceManager& deviceManager_;
    MountService& mountUtils_;
    IDeviceResolver& resolver_;

public:
    MountManager(
        DeviceManager& deviceManager,
        MountService& mountUtils,
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
        mylog->info("Mounted: {}", mountPoint);

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
        mylog->info("Unmounted: {}", mountPoint);
    }

    void remount(const MountRecord &record)
    {
        mountUtils_.remountDevice(
            record.mountPoint,
            record.mode
        );
        mylog->info("Remounted: {}", record.mountPoint);
    }
};