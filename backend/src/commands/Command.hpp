#pragma once

class CommandContext;

class Command {
  public:
    virtual ~Command() = default;
    virtual void execute(CommandContext &ctx) = 0;
};