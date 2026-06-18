#pragma once

#include <vector>

#include "entities/Device.hpp"
#include "entities/MountRecord.hpp"
#include "types/types.hpp"

class CommandContext;

class Command {
  public:
    virtual ~Command() = default;
    virtual void execute(CommandContext &ctx) = 0;
};

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

    AddDeviceToWhiteListCommand(MountRecord &d) : record(d) {}

    void execute(CommandContext &ctx) override;
};

class DeleteDeviceFromWhiteListCommand : public Command {
  private:
    core::Id id;

  public:
    DeleteDeviceFromWhiteListCommand(core::Id id) : id(id) {}

    void execute(CommandContext &ctx) override;
};

class PatchValidToDeviceCommand : public Command {
  private:
    core::Id id;
    std::optional<std::string> validTo;

  public:
    PatchValidToDeviceCommand(core::Id id, std::optional<std::string> validTo)
        : id(id), validTo(validTo) {}

    void execute(CommandContext &ctx) override;
};

class GetCurrentConnectedDevicesCommand : public Command {
  public:
    std::vector<MountRecord> records;

    void execute(CommandContext &ctx) override;
};