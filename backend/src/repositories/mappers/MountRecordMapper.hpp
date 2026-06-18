#pragma once

#include "DeviceInfoMapper.hpp"
#include "entities/MountRecord.hpp"

class MountRecordMapper {
  public:
    static MountRecord fromRow(const Row& r) noexcept {
        return MountRecordBuilder()
            .withId(r.get<std::uint64_t>("id"))
            .withDevNode(r.get<std::string>("dev_node"))
            .withMountPoint(r.get<std::string>("mount_point"))
            .withMode(MountMode::parse(r.get<std::string>("mode")))
            .withInfo(DeviceInfoMapper::fromRow(r))
            .build();
    }
};