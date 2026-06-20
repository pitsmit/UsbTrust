#include "HttpServer.hpp"

#include <memory>

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "DefaultApi.h"
#include "exceptions/Exceptions.hpp"
#include "infrastructure/config/Config.hpp"
#include "infrastructure/logging/DevLogger.hpp"

void HttpServer::start() {
    int port = Config::getHttpPort();

    try {
        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(port));

        auto endpoint = std::make_unique<Pistache::Http::Endpoint>(addr);
        auto router = std::make_shared<Pistache::Rest::Router>();
        auto opts = Pistache::Http::Endpoint::options().threads(1);

        endpoint->init(opts);
        auto api = std::make_shared<api::DefaultApi>(router, facade);

        api->init();
        endpoint->setHandler(router->handler());
        endpoint->serveThreaded();
        mylog->info("Server started on port {}", port);
    } catch (const std::exception &e) {
        throw HttpServerError(e.what());
    }
}