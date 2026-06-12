#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "Config.hpp"
#include "DBInitializer.hpp"
#include "DeviceEventNotifyManager.hpp"
#include "EventLoop.hpp"
#include "EventQueue.hpp"
#include "Facade.hpp"
#include "LinuxMountSystem.hpp"
#include "MountRecoveryService.hpp"
#include "UdevDeviceResolver.hpp"
#include "Watcher.hpp"
#include "WebSocketServer.hpp"

class App {
private:
    DBConnection db;
    LinuxMountSystem linms;
    UdevDeviceResolver resolver;
    Facade facade;
    WebSocketServer ws;
    DeviceEventNotifyManager notifier;
    EventQueue<DeviceEvent> queue;
    DeviceControlService service;
    EventLoop loop;
    UdevWatcher watcher;
    MountRecoveryService rec;

#ifdef BUILD_HTTP_SERVER
    std::optional<std::jthread> httpThread;
#endif

    template<typename F>
    static std::jthread makeThread(std::string_view name, F&& f)
    {
        return std::jthread(
            [name, fn = std::forward<F>(f)] {
                try {
                    fn();
                }
                catch (const std::exception& e) {
                    std::cerr << name << ": " << e.what() << '\n';
                }
            });
    }

    void startOptionalServices()
    {
#ifdef BUILD_HTTP_SERVER
        httpThread.emplace([this] {
            try {
                HttpServer(facade).start();
            }
            catch (const std::exception& e) {
                std::cerr << "http: " << e.what() << '\n';
            }
        });
#endif
    }

public:
    App()
        : db(Config::getDBPath())
        , linms()
        , resolver()
        , facade(db, linms, resolver)
        , ws(Config::getWebSocketPort())
        , notifier(ws)
        , queue()
        , service(facade.registry(), facade.mounts(), notifier, resolver)
        , loop(queue, service)
        , watcher(queue)
        , rec(facade.registry(), resolver, facade.mounts(), facade.devices())
    {
    }

    void init()
    {
        DBInitializer::init(db);
        rec.run();
        ws.start();
        startOptionalServices();
    }

    void run()
    {
        using namespace std::chrono_literals;

        auto watcherThread = makeThread("watcher", [this] {
            watcher.run();
        });

        auto loopThread = makeThread("loop", [this] {
            loop.run();
        });

        for (;;)
            std::this_thread::sleep_for(10s);
    }
};

int main()
{
    App app;
    app.init();
    app.run();
}