#include "MountRecord.hpp"

MountRecordBuilder &MountRecordBuilder::withId(std::optional<core::Id> id) noexcept {
    record_.id = id ? id : std::nullopt;
    return *this;
}

MountRecordBuilder &MountRecordBuilder::withDevNode(const core::path &devNode) noexcept {
    record_.devNode = devNode;
    return *this;
}

MountRecordBuilder &MountRecordBuilder::withMountPoint(const core::path &mountPoint) noexcept {
    record_.mountPoint = mountPoint;
    return *this;
}

MountRecordBuilder &MountRecordBuilder::withInfo(const DeviceInfo &info) noexcept {
    record_.info = info;
    return *this;
}

MountRecordBuilder &MountRecordBuilder::withMode(const MountMode &mode) noexcept {
    record_.mode = mode;
    return *this;
}

MountRecord MountRecordBuilder::build() const noexcept {
    return record_;
}