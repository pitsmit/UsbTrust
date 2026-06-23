#pragma once

#include "DeviceInfo.hpp"
#include "MountMode.hpp"
#include "types/types.hpp"

struct MountRecord {
    std::optional<core::Id> id; /// id таблицы Device
    core::path devNode;
    core::path mountPoint;
    DeviceInfo info;
    MountMode mode{MountMode::RO};
};

class MountRecordBuilder {
  private:
    MountRecord record_{};

  public:
    MountRecordBuilder &withId(std::optional<core::Id> id) noexcept {
        record_.id = id ? id : std::nullopt;
        return *this;
    }

    MountRecordBuilder &withDevNode(const core::path &devNode) noexcept {
        record_.devNode = devNode;
        return *this;
    }

    MountRecordBuilder &withMountPoint(const core::path &mountPoint) noexcept {
        record_.mountPoint = mountPoint;
        return *this;
    }

    MountRecordBuilder &withInfo(const DeviceInfo &info) noexcept {
        record_.info = info;
        return *this;
    }

    MountRecordBuilder &withMode(const MountMode &mode) noexcept {
        record_.mode = mode;
        return *this;
    }

    MountRecord build() const {
        return record_;
    }
};