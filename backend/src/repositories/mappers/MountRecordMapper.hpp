#pragma once

#include "DeviceInfoMapper.hpp"
#include "entities/MountRecord.hpp"

class MountRecordMapper {
  public:
    enum Columns {
        ID = 0,
        DEVICE_INFO_ID,
        DEV_NODE,
        MOUNT_POINT,
        MODE,
        VENDOR_ID,
        PRODUCT_ID,
        SERIAL,
        VENDOR_NAME,
        PRODUCT_NAME
    };

  public:
    static MountRecord fromRow(char **v) noexcept {
        return MountRecordBuilder()
            .withId(std::stoull(v[ID]))
            .withDevNode(v[DEV_NODE])
            .withMountPoint(v[MOUNT_POINT])
            .withMode(MountMode::parse(v[MODE]))
            .withInfo(DeviceInfoMapper::fromRow(
                v, VENDOR_ID, PRODUCT_ID, SERIAL, VENDOR_NAME, PRODUCT_NAME))
            .build();
    }
};