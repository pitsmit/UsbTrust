#pragma once

#include "ports/IMountSystem.hpp"

class MountService {
  private:
    IMountSystem &sys;

    enum class FsPermModel { NativePosix, Emulated };

    static FsPermModel classifyPermModel(const std::string &fs);

  public:
    explicit MountService(IMountSystem &s) : sys(s) {}

    void mount(const core::path &devnode, const core::path &mountPoint, MountMode mode);
    void unmount(const core::path &mountPoint);
    void remount(const core::path &mountPoint, MountMode mode);
};