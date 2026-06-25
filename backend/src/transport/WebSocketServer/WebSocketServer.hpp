#pragma once

#include "ports/IWebSocketServer.hpp"

class WebSocketServer : public IWebSocketServer {
    int port;

  public:
    explicit WebSocketServer(int port_) : port(port_) {}

    void start() override;
    void broadcast(std::string_view message) override;
};