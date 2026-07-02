#include "DeviceInfo.hpp"

DeviceInfoBuilder &DeviceInfoBuilder::withVendorId(std::string_view v) noexcept {
    info_.vendorId = v;
    return *this;
}

DeviceInfoBuilder &DeviceInfoBuilder::withProductId(std::string_view v) noexcept {
    info_.productId = v;
    return *this;
}

DeviceInfoBuilder &DeviceInfoBuilder::withSerial(std::string_view v) noexcept {
    info_.serial = v;
    return *this;
}

DeviceInfoBuilder &DeviceInfoBuilder::withVendorName(std::string_view v) noexcept {
    info_.vendorName = v;
    return *this;
}

DeviceInfoBuilder &DeviceInfoBuilder::withProductName(std::string_view v) noexcept {
    info_.productName = v;
    return *this;
}

DeviceInfo DeviceInfoBuilder::build() const noexcept {
    return info_;
}