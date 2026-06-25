#pragma once

#include <string_view>

class IWebSocketServer {
  public:
    virtual ~IWebSocketServer() = default;

    virtual void start() = 0;
    virtual void broadcast(std::string_view message) = 0;
};