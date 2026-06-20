#pragma once

#include <expected>
#include <string>

#include <systemd/sd-device.h>

#include "exceptions/Exceptions.hpp"

class SDdev {
    sd_device *dev;

  public:
    explicit SDdev(sd_device *dev_) : dev(dev_) {}

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

    explicit operator bool() const {
        return dev != nullptr;
    }
};