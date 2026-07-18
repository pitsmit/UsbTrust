#pragma once

#include <optional>
#include <string>

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "types/types.hpp"

struct Device {
    core::Id id;
    DeviceInfo info;
    std::optional<std::string> validTo;
};