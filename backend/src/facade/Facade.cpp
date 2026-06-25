#include "Facade.hpp"

void Facade::execute(Command &command) {
    command.execute(ctx);
}