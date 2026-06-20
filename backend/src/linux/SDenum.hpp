#pragma once

#include <systemd/sd-device.h>

#include "exceptions/Exceptions.hpp"
#include "linux/SDdevView.hpp"

class SDenum {
    sd_device_enumerator *enumerator = nullptr;

  public:
    explicit SDenum() {
        if (sd_device_enumerator_new(&enumerator) < 0 || !enumerator)
            throw DeviceEnumeratorError("Failed to create device enumerator");
    }

    ~SDenum() {
        sd_device_enumerator_unref(enumerator);
    }

    void applyFilter() {
        sd_device_enumerator_add_match_subsystem(enumerator, "block", 1);
        sd_device_enumerator_add_match_property(enumerator, "DEVTYPE", "partition");
    }

    SDdevView first() const {
        return SDdevView(sd_device_enumerator_get_device_first(enumerator));
    }

    SDdevView next() const {
        return SDdevView(sd_device_enumerator_get_device_next(enumerator));
    }
};