#pragma once

class Facade;

class HttpServer {
    Facade &facade;

  public:
    explicit HttpServer(Facade &facade_) : facade(facade_) {}

    void start();
};