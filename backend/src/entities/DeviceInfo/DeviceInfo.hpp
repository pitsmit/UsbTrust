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
};

template <typename T>
concept hasGetAttrMethod = requires(T v, std::string_view s) { v.getSysAttr(s); };

class DeviceInfoBuilder {
    using StringMember = std::string DeviceInfo::*;
    using OptionalMember = std::optional<std::string> DeviceInfo::*;

    inline static constexpr struct {
        std::string_view attr;
        StringMember member;
    } requiredMappings[] = {
        {"idVendor", &DeviceInfo::vendorId},
        {"idProduct", &DeviceInfo::productId},
        {"serial", &DeviceInfo::serial},
    };

    inline static constexpr struct {
        std::string_view attr;
        OptionalMember member;
    } optionalMappings[] = {
        {"manufacturer", &DeviceInfo::vendorName},
        {"product", &DeviceInfo::productName},
    };

  public:
    template <hasGetAttrMethod T> DeviceInfo build(const T &usb) {
        DeviceInfo info;
        for (const auto &[attr, member] : requiredMappings) {
            auto value = usb.getSysAttr(attr);
            if (!value)
                throw value.error();
            info.*member = std::move(*value);
        }
        for (const auto &[attr, member] : optionalMappings) {
            if (auto value = usb.getSysAttr(attr))
                info.*member = std::move(*value);
        }
        return info;
    }
};