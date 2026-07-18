#pragma once

#include <optional>

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "entities/MountMode/MountMode.hpp"
#include "types/types.hpp"

struct MountRecord {
    std::optional<core::Id> device_id;
    core::path devNode;
    core::path mountPoint;
    DeviceInfo info;
    MountMode mode;

    bool operator==(const MountRecord &) const = default;
};