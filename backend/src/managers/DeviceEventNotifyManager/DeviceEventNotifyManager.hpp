#pragma once

#include "ports/IWebSocketServer.hpp"
#include "types/types.hpp"

class MountRecord;

class DeviceEventNotifyManager {
    IWebSocketServer &server;

  public:
    explicit DeviceEventNotifyManager(IWebSocketServer &server_) : server(server_) {}

    void notifyInsert(const MountRecord &record);
    void notifyRemove(const core::path &mountPoint);
};