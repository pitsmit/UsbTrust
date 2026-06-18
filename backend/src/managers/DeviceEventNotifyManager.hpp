#pragma once

#include "ports/IWebSocketServer.hpp"

class MountRecord;

class DeviceEventNotifyManager {
  private:
    IWebSocketServer &ws_;

  public:
    explicit DeviceEventNotifyManager(IWebSocketServer &ws) : ws_(ws) {}

    void notifyInsert(const MountRecord &record);
    void notifyRemove(std::string_view mountPoint);
};