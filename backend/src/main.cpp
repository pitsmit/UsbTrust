#include <iostream>
#include <optional>
#include <thread>

#include "dispatch/EventLoop.hpp"
#include "dispatch/EventQueue.hpp"
#include "facade/Facade.hpp"
#include "infrastructure/config/Config.hpp"
#include "linux/LinuxMountSystem/LinuxMountSystem.hpp"
#include "linux/Watcher/Watcher.hpp"
#include "managers/DeviceEventNotifyManager.hpp"
#include "services/RecoveryService.hpp"
#include "services/UsbDeviceContextProvider.hpp"
#include "storage/executor/SqlExecutor.hpp"
#include "storage/init/DBInitializer.hpp"
#include "transport/WebSocketServer.hpp"

#ifdef BUILD_HTTP_SERVER
#include "transport/HttpServer.hpp"
#endif

class App {
  private:
    DataBase db;
    SqlExecutor exec;
    LinuxMountSystem linms;
    UsbDeviceContextProvider resolver;
    Facade facade;
    WebSocketServer ws;
    DeviceEventNotifyManager notifier;
    EventQueue<DeviceEvent> queue;
    DeviceEventService service;
    EventLoop loop;
    Watcher watcher;
    RecoveryService rec;

#ifdef BUILD_HTTP_SERVER
    std::optional<std::jthread> httpThread;
#endif

    template <typename F> static std::jthread makeThread(std::string_view name, F &&f) {
        return std::jthread([name, fn = std::forward<F>(f)] {
            try {
                fn();
            } catch (const std::exception &e) {
                std::cerr << name << ": " << e.what() << '\n';
            }
        });
    }

    void startOptionalServices() {
#ifdef BUILD_HTTP_SERVER
        httpThread.emplace([this] {
            try {
                HttpServer(facade).start();
            } catch (const std::exception &e) {
                std::cerr << "http: " << e.what() << '\n';
            }
        });
#endif
    }

  public:
    App()
        : db(Config::getDBPath()), exec(db), linms(), resolver(), facade(exec, linms, resolver),
          ws(Config::getWebSocketPort()), notifier(ws), queue(),
          service(facade.registry(), facade.mounts(), notifier, resolver), loop(queue, service),
          watcher(queue), rec(facade.registry(), resolver, facade.mounts(), facade.devices()) {}

    void init() {
        DBInitializer::init(exec);
        rec.run();
        ws.start();
        startOptionalServices();
    }

    void run() {
        using namespace std::chrono_literals;

        auto watcherThread = makeThread("watcher", [this] { watcher.run(); });

        auto loopThread = makeThread("loop", [this] { loop.run(); });

        for (;;)
            std::this_thread::sleep_for(10s);
    }
};

int main() {
    App app;
    app.init();
    app.run();
}