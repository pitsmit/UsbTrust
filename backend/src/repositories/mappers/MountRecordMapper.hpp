#pragma once

#include "MountRecord.hpp"
#include "DeviceInfoMapper.hpp"

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

private:
    static ::MODE parseMode(const char* value)
    {
        return std::string(value) == "RW"
            ? ::MODE::RW
            : ::MODE::RO;
    }

public:
    static MountRecord fromRow(char** v)
    {
        return MountRecordBuilder()
            .withId(std::stoull(v[ID]))
            .withDevNode(v[DEV_NODE] ? v[DEV_NODE] : "")
            .withMountPoint(v[MOUNT_POINT] ? v[MOUNT_POINT] : "")
            .withMode(parseMode(v[MODE]))
            .withInfo(
                DeviceInfoMapper::fromRow(
                    v,
                    VENDOR_ID,
                    PRODUCT_ID,
                    SERIAL,
                    VENDOR_NAME,
                    PRODUCT_NAME
                )
            )
            .build();
    }
};