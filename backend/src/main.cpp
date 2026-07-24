#include <iostream>
#include <optional>
#include <thread>

#include "dispatch/EventLoop/EventLoop.hpp"
#include "dispatch/EventQueue/EventQueue.hpp"
#include "facade/Facade.hpp"
#include "infrastructure/config/Config.hpp"
#include "linux/LinuxMountSystem/LinuxMountSystem.hpp"
#include "managers/DeviceEventNotifyManager/DeviceEventNotifyManager.hpp"
#include "services/EventWatcherService/EventWatcherService.hpp"
#include "services/RecoveryService/RecoveryService.hpp"
#include "services/UsbDeviceContextProvider/UsbDeviceContextProvider.hpp"
#include "storage/DBInitializer/DBInitializer.hpp"
#include "storage/DataBase/DataBase.hpp"
#include "storage/SqlExecutor/SqlExecutor.hpp"
#include "transport/WebSocketServer/WebSocketServer.hpp"

#ifdef BUILD_HTTP_SERVER
#include "transport/http/HttpServer/HttpServer.hpp"
#endif

class App {
    DataBase db;
    SqlExecutor exec;
    DeviceManager deviceManager;
    MountRegistryManager mountRegistry;
    LinuxMountSystem linms;
    UsbDeviceContextProvider provider;
    MountService mountService;
    MountManager mountManager;
    EventQueue<DeviceEvent> queue;
    EventWatcherService watcher;
    MountCoordinator coordinator;
    WebSocketServer ws;
    DeviceEventNotifyManager notifier;
    DeviceEventService service;
    EventLoop loop;
    RecoveryService rec;
    CommandContext ctx;
    Facade facade;

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
        : db(Config::getDBPath()), exec(db), deviceManager(exec), mountRegistry(exec), linms(),
          provider(), mountService(linms, provider),
          mountManager(deviceManager, mountService, provider), queue(), watcher(queue),
          coordinator(mountManager, mountRegistry), ws(Config::getWebSocketPort()), notifier(ws),
          service(notifier, provider, coordinator), loop(queue, service),
          rec(mountRegistry, provider, coordinator), ctx{deviceManager, mountRegistry, coordinator},
          facade(ctx) {}

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