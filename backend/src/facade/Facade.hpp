#pragma once

#include "commands/Command.hpp"
#include "commands/CommandContext.hpp"

class Facade {
    CommandContext ctx;

  public:
    Facade(CommandContext &ctx_) : ctx{ctx_} {}

    void execute(Command &command);
};