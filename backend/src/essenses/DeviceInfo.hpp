#pragma once

#include <string>
#include <optional>

struct DeviceInfo {
    std::string vendorId;
    std::string productId;
    std::string serial;
    std::optional<std::string> vendorName;
    std::optional<std::string> productName;

    bool operator==(const DeviceInfo& other) const
    {
        return vendorId    == other.vendorId &&
               productId   == other.productId &&
               serial      == other.serial &&
               vendorName  == other.vendorName &&
               productName == other.productName;
    }

    bool operator!=(const DeviceInfo& other) const
    {
        return !(*this == other);
    }
};

class DeviceInfoBuilder {
private:
    DeviceInfo info_;

public:
    DeviceInfoBuilder& withVendorId(const std::string& v) {
        info_.vendorId = v;
        return *this;
    }

    DeviceInfoBuilder& withProductId(const std::string& v) {
        info_.productId = v;
        return *this;
    }

    DeviceInfoBuilder& withSerial(const std::string& v) {
        info_.serial = v;
        return *this;
    }

    DeviceInfoBuilder& withVendorName(const std::string& v) {
        info_.vendorName = v;
        return *this;
    }

    DeviceInfoBuilder& withProductName(const std::string& v) {
        info_.productName = v;
        return *this;
    }

    DeviceInfo build() { return info_; }
};