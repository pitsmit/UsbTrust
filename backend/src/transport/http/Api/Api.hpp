#pragma once

#include <httplib.h>

#include "facade/Facade.hpp"
#include "transport/JsonUtils.hpp"

class Api {
  public:
    using request = const httplib::Request &;
    using response = httplib::Response;
    explicit Api(Facade &facade_) : facade(facade_){};
    void registerRoutes(httplib::Server &server);

  private:
    Facade &facade;

    void addWhitelist(request req, const response &res);
    void deleteWhitelist(request req, const response &res);
    void patchWhitelist(request req, const response &res);
    void getWhitelist(request req, const response &res);
    void getConnectedDevices(request req, const response &res);

    void sendJson(response res, const json &body, int status = 200);
    void sendStatus(response res, int status);
    void sendError(response res, const std::exception &e);
    template <typename Handler> auto safe(Handler &&handler);

#ifdef ENABLE_TEST_API
    void seedWhitelist(request req, respone res);
#endif
};

template <typename Handler> auto Api::safe(Handler &&handler) {
    return [handler = std::forward<Handler>(handler), this](request req, response res) {
        try {
            handler(req, res);
        } catch (const std::exception &e) {
            sendError(res, e);
        }
    };
}