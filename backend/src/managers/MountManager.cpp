#include "MountManager.hpp"

#include "infrastructure/logging/DevLogger.hpp"
#include "services/MountPointBuilder.hpp"

MountRecord MountManager::mount(std::string_view devNode) {
    auto info = resolver.resolve(devNode);
    auto mountPoint = MountPointBuilder::build(info);
    MountPointBuilder::ensureExists(mountPoint);
    auto id = deviceManager.isAllowed(info);
    auto mode = MountMode::fromPresence(id);
    mountService.mount(devNode, mountPoint, mode);
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
    mountService.unmount(mountPoint);
    mylog->info("Unmounted: {}", mountPoint);
}

void MountManager::remount(const MountRecord &record) {
    mountService.remount(record.mountPoint, record.mode);
    mylog->info("Remounted: {}", record.mountPoint);
}