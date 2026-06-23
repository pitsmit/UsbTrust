#include "SDdev.hpp"

#include <format>

SDdev::SDdev(const core::path &node) {
    struct stat st {};
    if (stat(node.c_str(), &st) < 0)
        throw ResolveInfoError(
            std::format("Failed to create SDdev from devnode: {}", node.c_str()));
    if (sd_device_new_from_devnum(&dev, 'b', st.st_rdev) < 0 || !dev) {
        throw ResolveInfoError(
            std::format("Failed to create SDdev from devnode: {}", node.c_str()));
    }
}

SDdev::~SDdev() {
    if (dev)
        sd_device_unref(dev);
}

SDdevView SDdev::findUsbDevice() const {
    sd_device *usb = nullptr;
    if (sd_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device", &usb) < 0 || !usb) {
        sd_device *disk = nullptr;
        if (sd_device_get_parent_with_subsystem_devtype(dev, "block", "disk", &disk) >= 0 && disk) {
            sd_device_get_parent_with_subsystem_devtype(disk, "usb", "usb_device", &usb);
        }
    }
    if (!usb) {
        throw ResolveInfoError("No usb device found!");
    }

    return SDdevView(usb);
}