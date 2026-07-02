#include "DeviceEventNotifyManager.hpp"

#include "transport/JsonUtils.hpp"

void DeviceEventNotifyManager::notifyInsert(const MountRecord &record) {
    json payload = {{"type", "insert"}, {"data", record}};

    server.broadcast(payload.dump());
}

void DeviceEventNotifyManager::notifyRemove(const core::path &mountPoint) {
    json payload = {{"type", "remove"}, {"data", {{"mountPoint", mountPoint}}}};

    server.broadcast(payload.dump());
}