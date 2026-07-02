#include "DeviceCommands.hpp"

#include "commands/CommandContext.hpp"
#include "managers/DeviceManager/DeviceManager.hpp"
#include "managers/MountCoordinator/MountCoordinator.hpp"
#include "managers/MountRegistryManager/MountRegistryManager.hpp"

void GetWhiteListDeviceCommand::execute(CommandContext &ctx) {
    list = ctx.deviceManager.getWhitelist();
}

void AddDeviceToWhiteListCommand::execute(CommandContext &ctx) {
    record.mode = MountMode::rw();
    record.id = ctx.deviceManager.addToWhitelist(record.info);
    ctx.coordinator.remount(record);
}

void DeleteDeviceFromWhiteListCommand::execute(CommandContext &ctx) {
    if (auto record = ctx.mountRegistry.getById(id)) {
        record->mode = MountMode::ro();
        ctx.coordinator.remount(*record);
    }
    ctx.deviceManager.removeFromWhitelist(id);
}

void PatchValidToDeviceCommand::execute(CommandContext &ctx) {
    ctx.deviceManager.patchValidTo(id, validTo);
}

void GetCurrentConnectedDevicesCommand::execute(CommandContext &ctx) {
    records = ctx.mountRegistry.getAll();
}