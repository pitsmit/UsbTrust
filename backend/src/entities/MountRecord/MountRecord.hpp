#pragma once

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "entities/MountMode/MountMode.hpp"
#include "types/types.hpp"

struct MountRecord {
    std::optional<core::Id> id; /// id таблицы Device
    core::path devNode;
    core::path mountPoint;
    DeviceInfo info;
    MountMode mode;

    bool operator==(const MountRecord &) const = default;
};

class MountRecordBuilder {
    MountRecord record_{};

  public:
    MountRecordBuilder &withId(std::optional<core::Id> id) noexcept;
    MountRecordBuilder &withDevNode(const core::path &devNode) noexcept;
    MountRecordBuilder &withMountPoint(const core::path &mountPoint) noexcept;
    MountRecordBuilder &withInfo(const DeviceInfo &info) noexcept;
    MountRecordBuilder &withMode(const MountMode &mode) noexcept;
    MountRecord build() const noexcept;
};