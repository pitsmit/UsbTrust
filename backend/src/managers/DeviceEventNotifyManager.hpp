#pragma once

#include <string>

#include "ports/IWebSocketServer.hpp"
#include "entities/MountRecord.hpp"
#include "transport/JsonUtils.hpp"

class DeviceEventNotifyManager {
private:
    IWebSocketServer& ws_;

public:
    explicit DeviceEventNotifyManager(IWebSocketServer& ws)
        : ws_(ws)
    {}

    void notifyInsert(const MountRecord& record)
    {
        json payload = {
            { "type", "insert" },
            { "data", record }
        };

        ws_.broadcast(payload.dump());
    }

    void notifyRemove(std::string_view mountPoint)
    {
        json payload = {
            { "type", "remove" },
            {
                "data",
                {
                    { "mountPoint", mountPoint }
                }
            }
        };

        ws_.broadcast(payload.dump());
    }
};