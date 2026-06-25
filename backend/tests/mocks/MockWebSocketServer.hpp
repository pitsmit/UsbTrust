#pragma once

#include "ports/IWebSocketServer.hpp"

class MockWebSocketServer : public IWebSocketServer {
  public:
    MockWebSocketServer() = default;

    void start() {}
    void broadcast(std::string_view) {}
};