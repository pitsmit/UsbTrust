#pragma once

#include <expected>
#include <format>
#include <string>

#include <systemd/sd-device.h>

#include "exceptions/Exceptions.hpp"

class SDdevView {
  protected:
    sd_device *dev;

  public:
    explicit SDdevView(sd_device *dev_) : dev(dev_) {}
    explicit SDdevView() = default;

    bool isUsbDevice() const {
        sd_device *parent = nullptr;
        return sd_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device", &parent) >=
                   0 &&
               parent != nullptr;
    }

    std::expected<std::string, SDdeviceError> getDevNode() const {
        const char *devNode = nullptr;
        if (sd_device_get_devname(dev, &devNode) >= 0 && devNode) {
            return devNode;
        }
        return std::unexpected<SDdeviceError>("Failed to extract devnode");
    }

    std::expected<std::string, SDdeviceError> getSysAttr(std::string_view attr) const {
        const char *value = nullptr;

        if (sd_device_get_sysattr_value(dev, attr.data(), &value) >= 0 && value)
            return value;

        return std::unexpected<SDdeviceError>(std::format("Failed to get sysattr: {}", attr));
    }

    explicit operator bool() const {
        return dev != nullptr;
    }
};