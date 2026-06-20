#pragma once

#include "linux/SDdevView/SDdevView.hpp"

class SDdev : public SDdevView {
  public:
    explicit SDdev(std::string_view node);
    ~SDdev();
    SDdevView findUsbDevice() const;
};