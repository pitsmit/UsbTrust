#pragma once

#include <systemd/sd-device.h>

#include "entities/DeviceEvent.hpp"
#include "linux/SDdevView/SDdevView.hpp"

class SDact {
    sd_device_action_t act;

  public:
    explicit SDact(SDdevView dev_) {
        act = dev_.getAction();
    }

    bool isValid() const;
    std::optional<DeviceEvent> toEvent(const SDdevView &dev) const;
};