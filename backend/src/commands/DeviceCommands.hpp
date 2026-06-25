#pragma once

#include <optional>
#include <string>
#include <vector>

#include "commands/Command.hpp"
#include "entities/MountRecord.hpp"
#include "types/types.hpp"

struct Device;

class GetWhiteListDeviceCommand : public Command {
  public:
    std::vector<Device> list;

    void execute(CommandContext &ctx) override;
};

class AddDeviceToWhiteListCommand : public Command {
  private:
    MountRecord record;

  public:
    core::Id id;

    explicit AddDeviceToWhiteListCommand(MountRecord &d) : record(d) {}

    void execute(CommandContext &ctx) override;
};

class DeleteDeviceFromWhiteListCommand : public Command {
  private:
    core::Id id;

  public:
    explicit DeleteDeviceFromWhiteListCommand(core::Id id_) : id(id_) {}

    void execute(CommandContext &ctx) override;
};

class PatchValidToDeviceCommand : public Command {
  private:
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