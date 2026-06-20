#pragma once

#include "ports/IMountSystem.hpp"

class MountService {
  private:
    IMountSystem &sys;

    enum class FsPermModel { NativePosix, Emulated };

    static FsPermModel classifyPermModel(const std::string &fs);

  public:
    explicit MountService(IMountSystem &s) : sys(s) {}

    void mount(std::string_view devnode, std::string_view mountPoint, MountMode mode);
    void unmount(std::string_view mountPoint);
    void remount(std::string_view mountPoint, MountMode mode);
};