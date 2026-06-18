#pragma once

#include "commands/CommandContext.hpp"
#include "commands/DeviceCommands.hpp"
#include "managers/DeviceManager.hpp"
#include "managers/MountManager.hpp"
#include "managers/MountRegistry.hpp"
#include "services/MountService.hpp"

class IMountSystem;
class IDeviceResolver;
class DBConnection;

class Facade {
  private:
    DeviceManager deviceManager;
    MountRegistryManager mountRegistry;
    MountService mountService;
    MountManager mountManager;
    CommandContext ctx;

  public:
    Facade(DBConnection &db, IMountSystem &sys, IDeviceResolver &res)
        : deviceManager(db), mountRegistry(db), mountService(sys),
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