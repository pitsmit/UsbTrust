#pragma once

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "CommandContext.hpp"
#include "MountRegistry.hpp"
#include "MountManager.hpp"
#include "types.hpp"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(CommandContext& ctx) = 0;
};

class GetWhiteListDeviceCommand : public Command {
public:
    std::vector<Device> list;

    void execute(CommandContext& ctx) override {
        list = ctx.deviceManager.getWhitelist();
    }
};

class AddDeviceToWhiteListCommand : public Command {
private:
    MountRecord record;

public:
    core::Id id;

    AddDeviceToWhiteListCommand(MountRecord& d)
        : record(d) {}

    void execute(CommandContext& ctx) override {
        id = ctx.deviceManager.addToWhitelist(record.info);
        record.mode = MountMode::rw();
        record.id = id;
        ctx.mountService.remount(record);
        ctx.mountRegistry.refresh(record);
    }
};

class DeleteDeviceFromWhiteListCommand : public Command {
private:
    core::Id id;

public:
    DeleteDeviceFromWhiteListCommand(core::Id id)
        : id(id) {}

    void execute(CommandContext& ctx) override {
        if (auto record = ctx.mountRegistry.getById(id)) {
            record->mode = MountMode::ro();
            ctx.mountService.remount(*record);
            ctx.mountRegistry.refresh(*record);
        }
        ctx.deviceManager.removeFromWhitelist(id);
    }
};

class PatchValidToDeviceCommand : public Command {
private:
    core::Id id;
    std::optional<std::string> validTo;

public:
    PatchValidToDeviceCommand(
        core::Id id,
        std::optional<std::string> validTo)
        : id(id), validTo(validTo) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.patchValidTo(id, validTo);
    }
};

class GetCurrentConnectedDevicesCommand : public Command {
public:
    std::vector<MountRecord> records;

    void execute(CommandContext& ctx) override {
        records = ctx.mountRegistry.getAll();
    }
};