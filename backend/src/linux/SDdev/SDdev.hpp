#pragma once

#include "linux/SDdevView/SDdevView.hpp"
#include "types/types.hpp"

class SDdev : public SDdevView {
  public:
    explicit SDdev(const core::path &node);
    ~SDdev();
    SDdevView findUsbDevice() const;
};