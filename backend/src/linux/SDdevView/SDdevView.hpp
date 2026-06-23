#pragma once

#include <expected>
#include <string>

#include <systemd/sd-device.h>

#include "exceptions/Exceptions.hpp"
#include "types/types.hpp"

class SDdevView {
  protected:
    sd_device *dev;

  public:
    explicit SDdevView(sd_device *dev_) : dev(dev_) {}
    explicit SDdevView() = default;

    bool isUsbDevice() const;
    std::expected<core::path, SDdeviceError> getDevNode() const;
    std::expected<std::string, SDdeviceError> getSysAttr(std::string_view attr) const;
    sd_device_action_t getAction() const;
    explicit operator bool() const;
};