#include "RecoveryService.hpp"

#include <unordered_set>

#include "infrastructure/logging/DevLogger.hpp"

void RecoveryService::actualize(MountRecord &rec) {
    const auto &devNode = rec.devNode;

    core::path mountPoint;

    try {
        mountPoint = resolver.getMountPoint(devNode);
    } catch (const std::exception &e) {
        try {
            auto newrec = manager.mount(devNode);
            registry.recreate(newrec);
        } catch (const MountError &ee) {
            return;
        }
    }

    auto md = resolver.getMountMode(mountPoint);
    auto info = resolver.getDeviceInfo(devNode.c_str());

    bool some_changes = false;

    if (info != rec.info) {
        rec.info = info;
        some_changes = true;
    }

    MountMode desired = MountMode::fromPresence(devman.isAllowed(info));

    if (rec.mode != desired) {
        rec.mode = desired;
        some_changes = true;
    }

    if (mountPoint != rec.mountPoint) {
        rec.mountPoint = mountPoint;
        some_changes = true;
    }

    if (md != desired) {
        try {
            manager.remount(rec);
        } catch (const MountError &e) {
            mylog->error("Failed remount for {}", rec.devNode.c_str());
        }
    }

    if (some_changes) {
        registry.refresh(rec);
    }
}

void RecoveryService::run() {
    auto regs = registry.getAll();
    auto devNodes = resolver.getUsbDevNodes();

    std::unordered_map<std::string, MountRecord> regMap;
    for (const auto &reg : regs)
        regMap[reg.devNode] = reg;
    std::unordered_set<std::string> devSet(devNodes.begin(), devNodes.end());

    // удаление отсутствующих устройств
    for (const auto &reg : regs) {
        if (!devSet.contains(reg.devNode)) {
            try {
                registry.removeByDevNode(reg.devNode);
                manager.unmount(reg.mountPoint);
            } catch (const UnMountError &e) {
                mylog->error("Failed unmount for {}", reg.mountPoint.c_str());
            }
        }
    }

    // актуализизация существующих и создание новых
    for (const auto &node : devNodes) {
        auto it = regMap.find(node);
        if (it != regMap.end()) {
            actualize(it->second);
        } else {
            try {
                auto rec = manager.mount(node);
                registry.add(rec);
            } catch (const MountError &e) {
                mylog->error("Failed mount for {}", node.c_str());
            }
        }
    }
}