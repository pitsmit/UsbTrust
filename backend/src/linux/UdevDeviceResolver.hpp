#pragma once

#include <format>
#include <memory>

#include <libmount/libmount.h>
#include <sys/stat.h>
#include <systemd/sd-device.h>

#include "entities/DeviceInfo.hpp"
#include "entities/MountMode.hpp"
#include "exceptions/Exceptions.hpp"
#include "linux/LibMountTab.hpp"
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
        auto mp =
            LibMountTab().findRecordFromDevNode(devNode).and_then(LibMountTab::extractMountPoint);
        if (!mp)
            throw mp.error();
        return *mp;
    }

    MountMode getMountMode(std::string_view mountpoint) override {
        auto mode = LibMountTab()
                        .findRecordFromMountPoint(mountpoint)
                        .and_then(LibMountTab::getFSopts)
                        .and_then(LibMountTab::extractMode);
        if (!mode)
            throw mode.error();
        return *mode;
    }
};