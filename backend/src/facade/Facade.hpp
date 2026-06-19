#pragma once

#include "commands/CommandContext.hpp"
#include "commands/DeviceCommands.hpp"
#include "managers/DeviceManager.hpp"
#include "managers/MountManager.hpp"
#include "managers/MountRegistry.hpp"
#include "services/MountService.hpp"

class IMountSystem;
class IDeviceResolver;
class SqlExecutor;

class Facade {
  private:
    DeviceManager deviceManager;
    MountRegistryManager mountRegistry;
    MountService mountService;
    MountManager mountManager;
    CommandContext ctx;

  public:
    Facade(SqlExecutor &ex, IMountSystem &sys, IDeviceResolver &res)
        : deviceManager(ex), mountRegistry(ex), mountService(sys),
          mountManager(deviceManager, mountService, res),
          ctx{deviceManager, mountRegistry, mountManager} {}

    void execute(Command &command) {
        command.execute(ctx);
    }

    DeviceManager &devices() noexcept {
        return deviceManager;
    }
    MountManager &mounts() noexcept {
        return mountManager;
    }
    MountRegistryManager &registry() noexcept {
        return mountRegistry;
    }
};