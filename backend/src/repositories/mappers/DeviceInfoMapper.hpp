#pragma once

#include "entities/DeviceInfo.hpp"

class DeviceInfoMapper {
public:
    static DeviceInfo fromRow(char** v,
        int vendorIdIdx,
        int productIdIdx,
        int serialIdx,
        int vendorNameIdx,
        int productNameIdx)
    {
        DeviceInfoBuilder builder;
        if (v[vendorIdIdx]) builder.withVendorId(v[vendorIdIdx]);
        if (v[productIdIdx]) builder.withProductId(v[productIdIdx]);
        if (v[serialIdx]) builder.withSerial(v[serialIdx]);
        if (v[vendorNameIdx]) builder.withVendorName(v[vendorNameIdx]);
        if (v[productNameIdx]) builder.withProductName(v[productNameIdx]);
        return builder.build();
    }
};