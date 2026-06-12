#pragma once

#include "DeviceCommands.hpp"
#include "CommandContext.hpp"
#include "DeviceManager.hpp"
#include "MountRegistry.hpp"
#include "MountUtils.hpp"
#include "MountManager.hpp"

class IMountSystem;
class IDeviceResolver;
class DBConnection;


class Facade {
private: 
    DeviceManager deviceManager;
    MountRegistryManager mountRegistry;
    MountUtils mountUtils;
    MountManager mountManager;
    CommandContext ctx;

public:
    Facade(DBConnection &db, IMountSystem &sys, IDeviceResolver &res)
        : deviceManager(db),
          mountRegistry(db),
          mountUtils(sys),
          mountManager(deviceManager, mountUtils, res),
          ctx {deviceManager, mountRegistry, mountManager}
    {}

    void execute(Command& command) {
        command.execute(ctx);
    }

    DeviceManager& devices() { return deviceManager; }
    MountManager& mounts() { return mountManager; }
    MountRegistryManager& registry() { return mountRegistry; }
    MountUtils& utils() { return mountUtils; }
};