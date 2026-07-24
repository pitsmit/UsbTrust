#pragma once

#include <httplib.h>

#include "facade/Facade.hpp"

class HttpServer {
  public:
    explicit HttpServer(Facade &facade);

    void start();

  private:
    Facade &facade;

    httplib::Server server;
};