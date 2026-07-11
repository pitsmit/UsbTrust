#pragma once

#include "types/types.hpp"

enum EventType { INSERT, REMOVE };

struct DeviceEvent {
    EventType type;
    core::path devNode;
};