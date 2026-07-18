#include "MountManager.hpp"

#include "infrastructure/logging/DevLogger.hpp"
#include "services/MountPointBuilderService/MountPointBuilderService.hpp"

MountRecord MountManager::mount(const core::path &devNode) {
    auto info = provider.getDeviceInfo(devNode);
    auto mountPoint = MountPointBuilderService::build(info);
    MountPointBuilderService::ensureExists(mountPoint);
    auto id = devices.isAllowed(info);
    auto mode = MountMode::fromBool(id.has_value());
    mounter.mount(devNode, mountPoint, mode);
    mylog->info("Mounted: {}", mountPoint.c_str());
    return MountRecord{
        .device_id = id, .devNode = devNode, .mountPoint = mountPoint, .info = info, .mode = mode};
}

void MountManager::unmount(const core::path &mountPoint) {
    mounter.unmount(mountPoint);
    mylog->info("Unmounted: {}", mountPoint.c_str());
}

void MountManager::remount(const MountRecord &record) {
    mounter.remount(record.mountPoint, record.mode);
    mylog->info("Remounted: {}", record.mountPoint.c_str());
}