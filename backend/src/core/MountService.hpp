#pragma once

#include <optional>
#include <string>

#include "DeviceManager.hpp"
#include "MountUtils.hpp"
#include "IDeviceResolver.hpp"
#include "MountPointBuilder.hpp"
#include "MountRecord.hpp"
#include "Exceptions.hpp"

class MountService {
private:
    DeviceManager& deviceManager_;
    MountUtils& mountUtils_;
    IDeviceResolver& resolver_;

public:
    MountService(
        DeviceManager& deviceManager,
        MountUtils& mountUtils,
        IDeviceResolver& resolver
    ) :
        deviceManager_(deviceManager),
        mountUtils_(mountUtils),
        resolver_(resolver)
    {}

    MountRecord mount(const std::string& devNode)
    {
        auto info = resolver_.resolve(devNode);
        auto mountPoint = MountPointBuilder::build(info);
        MountPointBuilder::ensureExists(mountPoint);
        int id = deviceManager_.isAllowed(info);

        mountUtils_.mountDevice(devNode, mountPoint, !id);

        return MountRecordBuilder()
                .withDevNode(devNode)
                .withId(id)
                .withInfo(info)
                .withMountPoint(mountPoint)
                .withMode(id ? MODE::RW : MODE::RO)
                .build();
    }

    void unmount(const std::string& mountPoint)
    {
        mountUtils_.handleUnmount(mountPoint);
    }

    void remount(MountRecord &record)
    {
        mountUtils_.remountDevice(
            record.mountPoint,
            record.mode == MODE::RO ? true : false
        );
    }
};