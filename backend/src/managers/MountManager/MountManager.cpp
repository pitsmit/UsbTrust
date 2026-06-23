#include "MountManager.hpp"

#include "infrastructure/logging/DevLogger.hpp"
#include "services/MountPointBuilder.hpp"

MountRecord MountManager::mount(const core::path &devNode) {
    auto info = resolver.getDeviceInfo(devNode);
    auto mountPoint = MountPointBuilder::build(info);
    MountPointBuilder::ensureExists(mountPoint);
    auto id = deviceManager.isAllowed(info);
    auto mode = MountMode::fromPresence(id);
    mountService.mount(devNode, mountPoint, mode);
    mylog->info("Mounted: {}", mountPoint.c_str());
    return MountRecordBuilder()
        .withDevNode(devNode)
        .withId(id)
        .withInfo(info)
        .withMountPoint(mountPoint)
        .withMode(mode)
        .build();
}

void MountManager::unmount(const core::path &mountPoint) {
    mountService.unmount(mountPoint);
    mylog->info("Unmounted: {}", mountPoint.c_str());
}

void MountManager::remount(const MountRecord &record) {
    mountService.remount(record.mountPoint, record.mode);
    mylog->info("Remounted: {}", record.mountPoint.c_str());
}