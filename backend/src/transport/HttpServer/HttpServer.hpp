#pragma once

class Facade;

class HttpServer {
    Facade &facade;

  public:
    explicit HttpServer(Facade &facade) : facade(facade) {}

    void start();
};