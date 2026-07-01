#include "RecoveryService.hpp"

#include <unordered_set>

void RecoveryService::actualize(MountRecord &rec) {
    auto mountPoint = provider.getMountPoint(rec.devNode);
    if (mountPoint.empty()) {
        coordinator.mount(rec.devNode);
        return;
    }
    auto some_changes = false;
    if (auto info = provider.getDeviceInfo(rec.devNode); info != rec.info) {
        rec.info = info;
        some_changes = true;
    }
    if (mountPoint != rec.mountPoint) {
        rec.mountPoint = mountPoint;
        some_changes = true;
    }
    MountMode desired = MountMode::fromPresence(devman.isAllowed(rec.info));
    if (rec.mode != desired) {
        rec.mode = desired;
        some_changes = true;
    }
    if (auto md = provider.getMountMode(rec.mountPoint); md != desired) {
        coordinator.remount(rec);
    }
    if (some_changes) {
        coordinator.refresh(rec);
    }
}

void RecoveryService::run() {
    auto currentNodes = provider.getUsbDevNodes();
    std::unordered_set<core::path> currentNodesSet(currentNodes.begin(), currentNodes.end());
    std::unordered_map<core::path, MountRecord> nodesRecordsMap;

    for (const auto &record : registrator.getAll()) {
        if (!currentNodesSet.contains(record.devNode)) {
            coordinator.unmount(record);
            continue;
        }
        nodesRecordsMap.emplace(record.devNode, record);
    }

    for (const auto &node : currentNodes) {
        auto it = nodesRecordsMap.find(node);
        if (it != nodesRecordsMap.end()) {
            actualize(it->second);
        } else {
            coordinator.mount(node);
        }
    }
}