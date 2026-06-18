#include "MountManager.hpp"

#include "infrastructure/logging/DevLogger.hpp"
#include "services/MountPointBuilder.hpp"

MountRecord MountManager::mount(std::string_view devNode) {
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

void MountManager::unmount(std::string_view mountPoint) {
    mountUtils_.handleUnmount(mountPoint);
    mylog->info("Unmounted: {}", mountPoint);
}

void MountManager::remount(const MountRecord &record) {
    mountUtils_.remountDevice(record.mountPoint, record.mode);
    mylog->info("Remounted: {}", record.mountPoint);
}