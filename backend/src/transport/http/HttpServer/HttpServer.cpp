#include "HttpServer.hpp"

#include "infrastructure/config/Config.hpp"
#include "infrastructure/logging/DevLogger.hpp"
#include "transport/http/Api/Api.hpp"

void HttpServer::start() {
    Api api(facade);
    api.registerRoutes(server);
    auto port = Config::getHttpPort();
    mylog->info("Listening on {}", port);
    server.listen("0.0.0.0", port);
}