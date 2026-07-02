#include "Device.hpp"

DeviceBuilder &DeviceBuilder::withId(core::Id id) noexcept {
    device_.id = id;
    return *this;
}

DeviceBuilder &DeviceBuilder::withInfo(const DeviceInfo &info) noexcept {
    device_.info = info;
    return *this;
}

DeviceBuilder &DeviceBuilder::withValidTo(std::string_view validTo) noexcept {
    device_.validTo = validTo;
    return *this;
}

Device DeviceBuilder::build() const noexcept {
    return device_;
}