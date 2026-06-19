#pragma once

#include <format>
#include <string.h>

#include <libmount/libmount.h>
#include <sys/stat.h>
#include <systemd/sd-device.h>

#include "entities/DeviceInfo.hpp"
#include "entities/MountRecord.hpp"
#include "ports/IDeviceResolver.hpp"

class UdevDeviceResolver : public IDeviceResolver {
  private:
    template <typename T, auto UnrefFn> struct SdDeleter {
        void operator()(T *p) const noexcept {
            if (p)
                UnrefFn(p);
        }
    };

    template <typename T, auto UnrefFn>
    using SdUniquePtr = std::unique_ptr<T, SdDeleter<T, UnrefFn>>;

  public:
    DeviceInfo resolve(std::string_view devNode) override {
        struct stat st {};
        if (stat(devNode.data(), &st) < 0)
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", devNode));
        sd_device *rawDevice = nullptr;
        if (sd_device_new_from_devnum(&rawDevice, 'b', st.st_rdev) < 0) {
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", devNode));
        }
        using SdDevicePtr = SdUniquePtr<sd_device, sd_device_unref>;
        SdDevicePtr device(rawDevice);
        sd_device *usb = nullptr;
        if (sd_device_get_parent_with_subsystem_devtype(device.get(), "usb", "usb_device", &usb) <
            0) {
            sd_device *disk = nullptr;
            if (sd_device_get_parent_with_subsystem_devtype(device.get(), "block", "disk", &disk) >=
                0) {
                sd_device_get_parent_with_subsystem_devtype(disk, "usb", "usb_device", &usb);
            }
        }
        if (!usb) {
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", devNode));
        }
        auto setIfPresent = [usb](const char *attr, auto setter) {
            const char *value = nullptr;
            if (sd_device_get_sysattr_value(usb, attr, &value) >= 0 && value) {
                setter(value);
            }
        };
        DeviceInfoBuilder builder;
        setIfPresent("idVendor", [&](auto v) { builder.withVendorId(v); });
        setIfPresent("idProduct", [&](auto v) { builder.withProductId(v); });
        setIfPresent("serial", [&](auto v) { builder.withSerial(v); });
        setIfPresent("manufacturer", [&](auto v) { builder.withVendorName(v); });
        setIfPresent("product", [&](auto v) { builder.withProductName(v); });
        return builder.build();
    }

    std::vector<std::string> getUsbDevNodes() override {
        using SdDeviceEnumeratorPtr = SdUniquePtr<sd_device_enumerator, sd_device_enumerator_unref>;
        std::vector<std::string> result;
        sd_device_enumerator *rawEnumerator = nullptr;
        if (sd_device_enumerator_new(&rawEnumerator) < 0)
            return result;
        SdDeviceEnumeratorPtr enumerator(rawEnumerator);
        sd_device_enumerator_add_match_subsystem(enumerator.get(), "block", true);
        sd_device_enumerator_add_match_property(enumerator.get(), "DEVTYPE", "partition");
        for (sd_device *dev = sd_device_enumerator_get_device_first(enumerator.get()); dev;
             dev = sd_device_enumerator_get_device_next(enumerator.get())) {
            sd_device *parent = nullptr;
            if (sd_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device", &parent) <
                0) {
                continue;
            }
            const char *devNode = nullptr;
            if (sd_device_get_devname(dev, &devNode) >= 0 && devNode != nullptr) {
                result.emplace_back(devNode);
            }
        }
        return result;
    }

    std::string getMountPoint(std::string_view devNode) override {
        struct libmnt_table *tb = mnt_new_table_from_file("/proc/self/mountinfo");
        if (!tb)
            throw MountError(std::format("Coud not get mountpoint from devnode: {}", devNode));
        struct libmnt_iter *itr = mnt_new_iter(MNT_ITER_FORWARD);
        struct libmnt_fs *fs = nullptr;
        if (!itr) {
            mnt_free_table(tb);
            throw MountError(std::format("Coud not get mountpoint from devnode: {}", devNode));
        }
        while (mnt_table_next_fs(tb, itr, &fs) == 0) {
            const char *src = mnt_fs_get_source(fs);
            const char *target = mnt_fs_get_target(fs);
            if (!src || !target)
                continue;
            if (devNode == src) {
                std::string mountpoint(target);
                mnt_free_iter(itr);
                mnt_free_table(tb);
                return mountpoint;
            }
        }
        mnt_free_iter(itr);
        mnt_free_table(tb);
        throw MountError(std::format("Coud not get mountpoint from devnode: {}", devNode));
    }

    MountMode getMountMode(std::string_view mountpoint) override {
        libmnt_table *tb = mnt_new_table_from_file("/proc/self/mountinfo");
        if (!tb)
            throw MountError(std::format("Coud not get mountmode from mountpoint: {}", mountpoint));
        libmnt_fs *fs = mnt_table_find_target(tb, mountpoint.data(), MNT_ITER_FORWARD);
        mnt_free_table(tb);
        if (!fs) {
            throw MountError(std::format("Coud not get mountmode from mountpoint: {}", mountpoint));
        }
        const char *opts = mnt_fs_get_options(fs);
        if (!opts) {
            throw MountError(std::format("Coud not get mountmode from mountpoint: {}", mountpoint));
        }
        if (mnt_optstr_get_option(opts, "rw", nullptr, nullptr) == 0) {
            return MountMode::rw();
        } else if (mnt_optstr_get_option(opts, "ro", nullptr, nullptr) == 0) {
            return MountMode::ro();
        }
        return MountMode::ro();
    }
};