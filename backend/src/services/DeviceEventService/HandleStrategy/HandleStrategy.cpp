#include "HandleStrategy.hpp"

#include "infrastructure/logging/DevLogger.hpp"

void HandleInsertEventStrategy::handle(const core::path &node) {
    mylog->info("Start handle INSERT event with devnode {}", node.c_str());
    auto record = context.coordinator.mount(node);
    context.notifier.notifyInsert(record);
}

void HandleRemoveEventStrategy::handle(const core::path &node) {
    mylog->info("Start handle REMOVE event with devnode {}", node.c_str());
    auto mountPoint = context.provider.getMountPoint(node);
    context.coordinator.unmount(mountPoint, node);
    context.notifier.notifyRemove(mountPoint);
}