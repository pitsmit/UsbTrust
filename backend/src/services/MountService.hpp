#pragma once

#include "ports/IMountSystem.hpp"

class MountService {
  private:
    IMountSystem &sys;

    enum class FsPermModel { NativePosix, Emulated };

    static FsPermModel classifyPermModel(const std::string &fs);

  public:
    explicit MountService(IMountSystem &s) : sys(s) {}

    void mountDevice(std::string_view devnode, std::string_view mountPoint, MountMode mode);
    void handleUnmount(std::string_view mountPoint);
    void remountDevice(std::string_view mountPoint, MountMode mode);
};