#pragma once

#include <optional>
#include <libudev.h>
#include <libmount/libmount.h>
#include <sys/stat.h>
#include <string.h>

#include "MountRecord.hpp"
#include "DeviceInfo.hpp"
#include "IDeviceResolver.hpp"

class UdevDeviceResolver : public IDeviceResolver {
public:
    DeviceInfo resolve(std::string_view devNode)
    {
        struct stat st{};
        if (stat(devNode.data(), &st) < 0) {
            throw ResolveInfoError(("Coud not extract deviceInfo from devnode: " + devNode).c_str());
        }

        struct udev* udev = udev_new();
        if (!udev) {
            throw ResolveInfoError(("Coud not extract deviceInfo from devnode: " + devNode).c_str());
        }

        struct udev_device* dev =
            udev_device_new_from_devnum(
                udev,
                'b',
                st.st_rdev);
        if (!dev) {
            udev_unref(udev);
            throw ResolveInfoError(("Coud not extract deviceInfo from devnode: " + devNode).c_str());
        }

        struct udev_device* usb =
            udev_device_get_parent_with_subsystem_devtype(
                dev,
                "usb",
                "usb_device");

        if (!usb) {
            struct udev_device* disk =
                udev_device_get_parent_with_subsystem_devtype(
                    dev,
                    "block",
                    "disk");

            if (disk) {
                usb =
                    udev_device_get_parent_with_subsystem_devtype(
                        disk,
                        "usb",
                        "usb_device");
            }
        }

        DeviceInfoBuilder builder;
        if (usb) {
            if (const char* vid = udev_device_get_sysattr_value(usb, "idVendor"))
                builder.withVendorId(vid);
            if (const char* pid = udev_device_get_sysattr_value(usb, "idProduct"))
                builder.withProductId(pid);
            if (const char* serial = udev_device_get_sysattr_value(usb, "serial"))
                builder.withSerial(serial);
            if (const char* vendorName = udev_device_get_sysattr_value(usb, "manufacturer"))
                builder.withVendorName(vendorName);
            if (const char* productName = udev_device_get_sysattr_value(usb, "product"))
                builder.withProductName(productName);
        } else {
            throw ResolveInfoError(("Coud not extract deviceInfo from devnode: " + devNode).c_str());
        }
        
        udev_device_unref(dev);
        udev_unref(udev);
        return builder.build();
    }

    bool isUsbDevice(struct udev_device* dev)
    {
        struct udev_device* parent =
            udev_device_get_parent_with_subsystem_devtype(
                dev, "usb", "usb_device");

        return parent != nullptr;
    }

    std::vector<std::string> getUsbDevNodes()
    {
        std::vector<std::string> result;
        struct udev* udev = udev_new();
        if (!udev)
            return result;
        struct udev_enumerate* enumerate =
            udev_enumerate_new(udev);
        if (!enumerate) {
            udev_unref(udev);
            return result;
        }
        udev_enumerate_add_match_subsystem(
            enumerate,
            "block");
        udev_enumerate_add_match_property(
            enumerate,
            "DEVTYPE",
            "partition");
        udev_enumerate_scan_devices(enumerate);
        struct udev_list_entry* devices =
            udev_enumerate_get_list_entry(enumerate);
        struct udev_list_entry* entry;

        udev_list_entry_foreach(entry, devices) {
            const char* path =
                udev_list_entry_get_name(entry);
            struct udev_device* dev =
                udev_device_new_from_syspath(
                    udev,
                    path);
            if (!dev)
                continue;
            if (!isUsbDevice(dev)) {
                udev_device_unref(dev);
                continue;
            }
            const char* devNode =
                udev_device_get_devnode(dev);
            if (devNode)
                result.emplace_back(devNode);
            udev_device_unref(dev);
        }
        udev_enumerate_unref(enumerate);
        udev_unref(udev);
        return result;
    }


    std::string getMountPoint(std::string_view devNode)
    {
        struct libmnt_table* tb = mnt_new_table_from_file("/proc/self/mountinfo");
        if (!tb)
            throw MountError(("Coud not get mountpoint from devnode: {}", devNode));
        struct libmnt_iter* itr = mnt_new_iter(MNT_ITER_FORWARD);
        struct libmnt_fs* fs = nullptr;
        if (!itr) {
            mnt_free_table(tb);
            throw MountError(std::format("Coud not get mountpoint from devnode: {}", devNode));
        }
        while (mnt_table_next_fs(tb, itr, &fs) == 0) {
            const char* src = mnt_fs_get_source(fs);
            const char* target = mnt_fs_get_target(fs);
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


    MountMode getMountMode(std::string_view mountpoint)
    {
        libmnt_table* tb =
            mnt_new_table_from_file("/proc/self/mountinfo");
        if (!tb)
            throw MountError(std::format("Coud not get mountmode from mountpoint: {}", mountpoint));
        libmnt_fs* fs =
            mnt_table_find_target(
                tb,
                mountpoint.data(),
                MNT_ITER_FORWARD);
        mnt_free_table(tb);
        if (!fs) {
            throw MountError(std::format("Coud not get mountmode from mountpoint: {}", mountpoint));
        }
        const char* opts = mnt_fs_get_options(fs);
        if (!opts) {
            throw MountError(std::format("Coud not get mountmode from mountpoint: {}", mountpoint));
        }
        if (mnt_optstr_get_option(opts, "rw", nullptr, nullptr) == 0)
        {
            return MountMode::rw();
        }
        else if (mnt_optstr_get_option(opts, "ro", nullptr, nullptr) == 0)
        {
            return MountMode::ro();
        }
    }
};