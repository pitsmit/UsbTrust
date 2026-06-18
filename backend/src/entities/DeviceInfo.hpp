#pragma once

#include <optional>
#include <string>

struct DeviceInfo {
    std::string vendorId;
    std::string productId;
    std::string serial;
    std::optional<std::string> vendorName;
    std::optional<std::string> productName;

    bool operator==(const DeviceInfo &) const = default;

    bool operator!=(const DeviceInfo &other) const {
        return !(*this == other);
    }
};

class DeviceInfoBuilder {
  private:
    DeviceInfo info_;

  public:
    DeviceInfoBuilder &withVendorId(std::string_view v) noexcept {
        info_.vendorId = v;
        return *this;
    }

    DeviceInfoBuilder &withProductId(std::string_view v) noexcept {
        info_.productId = v;
        return *this;
    }

    DeviceInfoBuilder &withSerial(std::string_view v) noexcept {
        info_.serial = v;
        return *this;
    }

    DeviceInfoBuilder &withVendorName(std::string_view v) noexcept {
        info_.vendorName = v;
        return *this;
    }

    DeviceInfoBuilder &withProductName(std::string_view v) noexcept {
        info_.productName = v;
        return *this;
    }

    DeviceInfo build() const {
        return info_;
    }
};