#pragma once

#include "ports/IWebSocketServer.hpp"

#include <string>

class MockWebSocketServer: public IWebSocketServer {
private:

public:
    explicit MockWebSocketServer(int port) {}

    void start()
    {

    }

    void broadcast(std::string_view message)
    {

    }
};