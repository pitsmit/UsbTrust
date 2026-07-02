#pragma once

#include <optional>
#include <string>
#include <vector>

#include "commands/Command.hpp"
#include "entities/MountRecord/MountRecord.hpp"
#include "types/types.hpp"

struct Device;

class GetWhiteListDeviceCommand : public Command {
  public:
    std::vector<Device> list;

    void execute(CommandContext &ctx) override;
};

class AddDeviceToWhiteListCommand : public Command {
  public:
    MountRecord record;

    explicit AddDeviceToWhiteListCommand(MountRecord &record_) : record(record_) {}

    void execute(CommandContext &ctx) override;
};

class DeleteDeviceFromWhiteListCommand : public Command {
    core::Id id;

  public:
    explicit DeleteDeviceFromWhiteListCommand(core::Id id_) : id(id_) {}

    void execute(CommandContext &ctx) override;
};

class PatchValidToDeviceCommand : public Command {
    core::Id id;
    std::optional<std::string> validTo;

  public:
    explicit PatchValidToDeviceCommand(core::Id id_, std::optional<std::string> validTo_)
        : id(id_), validTo(validTo_) {}

    void execute(CommandContext &ctx) override;
};

class GetCurrentConnectedDevicesCommand : public Command {
  public:
    std::vector<MountRecord> records;

    void execute(CommandContext &ctx) override;
};