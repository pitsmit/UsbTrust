#pragma once

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "CommandContext.hpp"
#include "MountRegistry.hpp"
#include "MountManager.hpp"

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
    int id;

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
    size_t id;

public:
    DeleteDeviceFromWhiteListCommand(size_t id)
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
    size_t id;
    std::optional<std::string> validTo;

public:
    PatchValidToDeviceCommand(
        size_t id,
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