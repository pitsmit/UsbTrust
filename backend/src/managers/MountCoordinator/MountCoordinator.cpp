#include "MountCoordinator.hpp"

MountRecord MountCoordinator::mount(const core::path &node) {
    registrator.removeByDevNode(node);
    auto rec = mounter.mount(node);
    registrator.add(rec);
    return rec;
}

void MountCoordinator::unmount(const MountRecord &record) {
    mounter.unmount(record.mountPoint);
    registrator.removeByDevNode(record.devNode);
}

void MountCoordinator::remount(const MountRecord &record) {
    mounter.remount(record);
    registrator.refresh(record);
}

void MountCoordinator::refresh(const MountRecord &record) {
    registrator.refresh(record);
}

void MountCoordinator::unmount(const core::path &mountPoint, const core::path &node) {
    mounter.unmount(mountPoint);
    registrator.removeByDevNode(node);
}

void MountCoordinator::unmount(const core::path &mountPoint) {
    mounter.unmount(mountPoint);
}
