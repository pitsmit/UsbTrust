#pragma once

#include <systemd/sd-device.h>

#include "linux/SDdevView/SDdevView.hpp"

class SDenum {
    sd_device_enumerator *enumerator = nullptr;

  public:
    explicit SDenum();
    ~SDenum();

    void applyFilter();
    SDdevView first() const;
    SDdevView next() const;
};