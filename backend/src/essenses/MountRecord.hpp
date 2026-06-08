#pragma once

#include <string>

#include "DeviceInfo.hpp"
#include "MountMode.hpp"

using Id = std::int64_t;

struct MountRecord {
    std::optional<Id> id; /// id таблицы Device
    std::string devNode;
    std::string mountPoint;
    DeviceInfo info;
    MountMode mode{MountMode::RO};
};

class MountRecordBuilder {
private:
    MountRecord record_{};

public:
    MountRecordBuilder& withId(Id id)
    {
        record_.id = id;
        return *this;
    }

    MountRecordBuilder& withDevNode(
        const std::string& devNode)
    {
        record_.devNode = devNode;
        return *this;
    }

    MountRecordBuilder& withMountPoint(
        const std::string& mountPoint)
    {
        record_.mountPoint = mountPoint;
        return *this;
    }

    MountRecordBuilder& withInfo(
        const DeviceInfo& info)
    {
        record_.info = info;
        return *this;
    }

    MountRecordBuilder& withMode(
        const MountMode& mode)
    {
        record_.mode = mode;
        return *this;
    }

    MountRecord build() { return record_; }
};