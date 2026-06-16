#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "infrastructure/config/Config.hpp"
#include "repositories/DBInitializer.hpp"
#include "managers/DeviceEventNotifyManager.hpp"
#include "dispatch/EventLoop.hpp"
#include "dispatch/EventQueue.hpp"
#include "facade/Facade.hpp"
#include "linux/LinuxMountSystem.hpp"
#include "services/RecoveryService.hpp"
#include "linux/UdevDeviceResolver.hpp"
#include "linux/Watcher.hpp"
#include "transport/WebSocketServer.hpp"

#ifdef BUILD_HTTP_SERVER
#include "transport/HttpServer.hpp"
#endif

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
    RecoveryService rec;

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