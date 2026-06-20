#include "SDdevView.hpp"

#include <format>

bool SDdevView::isUsbDevice() const {
    sd_device *parent = nullptr;
    return sd_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device", &parent) >= 0 &&
           parent != nullptr;
}

std::expected<std::string, SDdeviceError> SDdevView::getDevNode() const {
    const char *devNode = nullptr;
    if (sd_device_get_devname(dev, &devNode) >= 0 && devNode) {
        return devNode;
    }
    return std::unexpected<SDdeviceError>("Failed to extract devnode");
}

std::expected<std::string, SDdeviceError> SDdevView::getSysAttr(std::string_view attr) const {
    const char *value = nullptr;

    if (sd_device_get_sysattr_value(dev, attr.data(), &value) >= 0 && value)
        return value;

    return std::unexpected<SDdeviceError>(std::format("Failed to get sysattr: {}", attr));
}

sd_device_action_t SDdevView::getAction() const {
    sd_device_action_t action;
    if (sd_device_get_action(dev, &action) < 0)
        return _SD_DEVICE_ACTION_INVALID;
    return action;
}

SDdevView::operator bool() const {
    return dev != nullptr;
}