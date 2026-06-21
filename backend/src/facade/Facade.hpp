#pragma once

#include "commands/CommandContext.hpp"
#include "commands/DeviceCommands.hpp"
#include "managers/DeviceManager/DeviceManager.hpp"
#include "managers/MountManager/MountManager.hpp"
#include "managers/MountRegistryManager/MountRegistryManager.hpp"
#include "services/MountService/MountService.hpp"

class IMountSystem;
class IUsbDeviceContextProvider;
class SqlExecutor;

class Facade {
  private:
    DeviceManager deviceManager;
    MountRegistryManager mountRegistry;
    MountService mountService;
    MountManager mountManager;
    CommandContext ctx;

  public:
    Facade(SqlExecutor &ex, IMountSystem &sys, IUsbDeviceContextProvider &res)
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