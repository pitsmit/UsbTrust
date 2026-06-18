#pragma once

#include <optional>
#include <string>

#include "DeviceInfo.hpp"
#include "types/types.hpp"

struct Device {
    core::Id id;
    DeviceInfo info;
    std::optional<std::string> validTo;
};

class DeviceBuilder {
  private:
    Device device_{};

  public:
    DeviceBuilder &withId(core::Id id) noexcept {
        device_.id = id;
        return *this;
    }

    DeviceBuilder &withInfo(const DeviceInfo &info) noexcept {
        device_.info = info;
        return *this;
    }

    DeviceBuilder &withValidTo(std::string_view validTo) noexcept {
        device_.validTo = validTo;
        return *this;
    }

    Device build() {
        return device_;
    }
};