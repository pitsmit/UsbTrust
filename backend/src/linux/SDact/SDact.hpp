#pragma once

#include <optional>

#include <systemd/sd-device.h>

#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "linux/SDdevView/SDdevView.hpp"

class SDact {
    sd_device_action_t act;
    SDdevView dev;

  public:
    explicit SDact(SDdevView dev_) {
        act = dev_.getAction();
        dev = dev_;
    }

    std::optional<DeviceEvent> toEvent() const;
};