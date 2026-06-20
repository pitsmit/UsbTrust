#pragma once

#include <mutex>
#include <set>

#include <ixwebsocket/IXConnectionState.h>
#include <ixwebsocket/IXWebSocketServer.h>

#include "ports/IWebSocketServer.hpp"

class WebSocketServer : public IWebSocketServer {
  private:
    ix::WebSocketServer server_;
    std::mutex mutex_;
    std::set<ix::WebSocket *> clients_;

  public:
    explicit WebSocketServer(int port) : server_(port) {}

    void start() override;
    void broadcast(std::string_view message) override;
};