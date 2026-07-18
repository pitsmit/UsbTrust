#pragma once

#include "ports/IMountSystem.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"

class MountService {
    IMountSystem &sys;
    IUsbDeviceContextProvider &provider;

    enum class FsPermModel { NativePosix, Emulated };

    static FsPermModel classifyPermModel(const std::string &fs);

  public:
    explicit MountService(IMountSystem &s, IUsbDeviceContextProvider &p) : sys(s), provider(p) {}

    void mount(const core::path &devnode, const core::path &mountPoint, MountMode &mode);
    void unmount(const core::path &mountPoint);
    void remount(const core::path &mountPoint, const MountMode &mode);
};