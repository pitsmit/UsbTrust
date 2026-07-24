#pragma once

#include <httplib.h>

#include "facade/Facade.hpp"

namespace api {

class DeviceApi {
  public:
    explicit DeviceApi(Facade &facade_) : facade(facade_){};

    void registerRoutes(httplib::Server &server);

  private:
    Facade &facade;

    void addWhitelist(const httplib::Request &req, httplib::Response &res);
    void deleteWhitelist(const httplib::Request &req, httplib::Response &res);
    void patchWhitelist(const httplib::Request &req, httplib::Response &res);
    void getWhitelist(const httplib::Request &req, httplib::Response &res);
    void getConnectedDevices(const httplib::Request &req, httplib::Response &res);

#ifdef ENABLE_TEST_API
    void seedWhitelist(const httplib::Request &req, httplib::Response &res);
#endif
};

} // namespace api