#pragma once

#include "ports/IWebSocketServer.hpp"
#include "types/types.hpp"

class MountRecord;

class DeviceEventNotifyManager {
  private:
    IWebSocketServer &ws_;

  public:
    explicit DeviceEventNotifyManager(IWebSocketServer &ws) : ws_(ws) {}

    void notifyInsert(const MountRecord &record);
    void notifyRemove(const core::path &mountPoint);
};