#pragma once

#include <format>

#include "linux/SDdevView.hpp"

class SDdev : public SDdevView {
  public:
    explicit SDdev(std::string_view node) {
        struct stat st {};
        if (stat(node.data(), &st) < 0)
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", node));
        if (sd_device_new_from_devnum(&dev, 'b', st.st_rdev) < 0 || !dev) {
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", node));
        }
    }

    ~SDdev() {
        if (dev)
            sd_device_unref(dev);
    }

    SDdevView findUsbDevice() const {
        sd_device *usb = nullptr;
        if (sd_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device", &usb) < 0 ||
            !usb) {
            sd_device *disk = nullptr;
            if (sd_device_get_parent_with_subsystem_devtype(dev, "block", "disk", &disk) >= 0 &&
                disk) {
                sd_device_get_parent_with_subsystem_devtype(disk, "usb", "usb_device", &usb);
            }
        }
        if (!usb) {
            throw ResolveInfoError("Could not extract deviceInfo from devnode");
        }

        return SDdevView(usb);
    }
};