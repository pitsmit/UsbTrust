#pragma once

#include <httplib.h>

#include "facade/Facade.hpp"

class HttpServer {
  public:
    explicit HttpServer(Facade &f) : facade(f){};
    void start();

  private:
    Facade &facade;
    httplib::Server server;
};