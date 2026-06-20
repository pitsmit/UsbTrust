#include "WebSocketServer.hpp"

void WebSocketServer::start() {
    server_.setOnClientMessageCallback([this](std::shared_ptr<ix::ConnectionState>,
                                              ix::WebSocket &webSocket,
                                              const ix::WebSocketMessagePtr &msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            std::lock_guard lock(mutex_);

            clients_.insert(&webSocket);
        } else if (msg->type == ix::WebSocketMessageType::Close) {
            std::lock_guard lock(mutex_);

            clients_.erase(&webSocket);
        }
    });

    auto result = server_.listen();

    if (!result.first) {
        throw std::runtime_error(result.second);
    }

    server_.start();
}

void WebSocketServer::broadcast(std::string_view message) {
    std::lock_guard lock(mutex_);

    for (auto *client : clients_) {
        client->send(std::string(message));
    }
}