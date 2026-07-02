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
    DeviceInfo info_;

  public:
    DeviceInfoBuilder &withVendorId(std::string_view v) noexcept;
    DeviceInfoBuilder &withProductId(std::string_view v) noexcept;
    DeviceInfoBuilder &withSerial(std::string_view v) noexcept;
    DeviceInfoBuilder &withVendorName(std::string_view v) noexcept;
    DeviceInfoBuilder &withProductName(std::string_view v) noexcept;
    DeviceInfo build() const noexcept;

  private:
    using Setter = DeviceInfoBuilder &(DeviceInfoBuilder::*)(std::string_view);
    struct Mapping {
        std::string_view attr;
        Setter setter;
    };

    inline static const Mapping mappings[] = {
        {"idVendor", &DeviceInfoBuilder::withVendorId},
        {"idProduct", &DeviceInfoBuilder::withProductId},
        {"serial", &DeviceInfoBuilder::withSerial},
        {"manufacturer", &DeviceInfoBuilder::withVendorName},
        {"product", &DeviceInfoBuilder::withProductName},
    };

  public:
    template <hasGetAttrMethod T> static DeviceInfo buildFrom(const T &usb);
};

template <hasGetAttrMethod T> DeviceInfo DeviceInfoBuilder::buildFrom(const T &usb) {
    DeviceInfoBuilder builder;
    for (const auto &[attr, setter] : mappings) {
        auto value = usb.getSysAttr(attr);
        if (!value)
            throw value.error();
        (builder.*setter)(*value);
    }
    return builder.build();
}