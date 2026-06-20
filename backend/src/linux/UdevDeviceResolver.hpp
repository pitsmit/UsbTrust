#pragma once

#include <format>

#include <sys/stat.h>
#include <systemd/sd-device.h>

#include "entities/DeviceInfo.hpp"
#include "entities/MountMode.hpp"
#include "exceptions/Exceptions.hpp"
#include "linux/SDdev.hpp"
#include "linux/SDenum.hpp"
#include "linux/libmount/LibMountTab.hpp"
#include "ports/IDeviceResolver.hpp"

class UdevDeviceResolver : public IDeviceResolver {
  public:
    DeviceInfo resolve(std::string_view devNode) override {
        struct stat st {};
        if (stat(devNode.data(), &st) < 0)
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", devNode));

        sd_device *device = nullptr;

        if (sd_device_new_from_devnum(&device, 'b', st.st_rdev) < 0 || !device) {
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", devNode));
        }

        sd_device *usb = nullptr;

        if (sd_device_get_parent_with_subsystem_devtype(device, "usb", "usb_device", &usb) < 0 ||
            !usb) {

            sd_device *disk = nullptr;

            if (sd_device_get_parent_with_subsystem_devtype(device, "block", "disk", &disk) >= 0 &&
                disk) {

                sd_device_get_parent_with_subsystem_devtype(disk, "usb", "usb_device", &usb);
            }
        }

        if (!usb) {
            sd_device_unref(device);
            throw ResolveInfoError(
                std::format("Could not extract deviceInfo from devnode: {}", devNode));
        }

        auto setIfPresent = [&](const char *attr, auto setter) {
            const char *value = nullptr;
            if (sd_device_get_sysattr_value(usb, attr, &value) >= 0 && value) {
                setter(value);
            }
        };

        DeviceInfoBuilder builder;

        setIfPresent("idVendor", [&](const char *v) { builder.withVendorId(v); });
        setIfPresent("idProduct", [&](const char *v) { builder.withProductId(v); });
        setIfPresent("serial", [&](const char *v) { builder.withSerial(v); });
        setIfPresent("manufacturer", [&](const char *v) { builder.withVendorName(v); });
        setIfPresent("product", [&](const char *v) { builder.withProductName(v); });

        sd_device_unref(device);

        return builder.build();
    }

    std::vector<std::string> getUsbDevNodes() override {
        SDenum enumerator;
        enumerator.applyFilter();

        std::vector<std::string> result;
        for (auto dev = enumerator.first(); dev; dev = enumerator.next()) {
            if (dev.isUsbDevice()) {
                if (auto devNode = dev.getDevNode())
                    result.emplace_back(*devNode);
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