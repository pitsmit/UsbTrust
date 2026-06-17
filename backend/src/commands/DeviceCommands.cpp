#include "DeviceCommands.hpp"

#include "managers/DeviceManager.hpp"
#include "CommandContext.hpp"
#include "managers/MountRegistry.hpp"
#include "managers/MountManager.hpp"

void GetWhiteListDeviceCommand::execute(CommandContext& ctx) {
    list = ctx.deviceManager.getWhitelist();
}

void AddDeviceToWhiteListCommand::execute(CommandContext& ctx) {
    id = ctx.deviceManager.addToWhitelist(record.info);
    record.mode = MountMode::rw();
    record.id = id;
    ctx.mountService.remount(record);
    ctx.mountRegistry.refresh(record);
}

void DeleteDeviceFromWhiteListCommand::execute(CommandContext& ctx) {
    if (auto record = ctx.mountRegistry.getById(id)) {
        record->mode = MountMode::ro();
        ctx.mountService.remount(*record);
        ctx.mountRegistry.refresh(*record);
    }
    ctx.deviceManager.removeFromWhitelist(id);
}

void PatchValidToDeviceCommand::execute(CommandContext& ctx) {
    ctx.deviceManager.patchValidTo(id, validTo);
}

void GetCurrentConnectedDevicesCommand::execute(CommandContext& ctx) {
    records = ctx.mountRegistry.getAll();
}