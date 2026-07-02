#pragma once

#include <optional>
#include <string>

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "types/types.hpp"

struct Device {
    core::Id id;
    DeviceInfo info;
    std::optional<std::string> validTo;
};

class DeviceBuilder {
    Device device_{};

  public:
    DeviceBuilder &withId(core::Id id) noexcept;
    DeviceBuilder &withInfo(const DeviceInfo &info) noexcept;
    DeviceBuilder &withValidTo(std::string_view validTo) noexcept;
    Device build() const noexcept;
};