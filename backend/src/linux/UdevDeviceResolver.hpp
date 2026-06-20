#pragma once

#include <systemd/sd-device.h>

#include "entities/DeviceInfo.hpp"
#include "linux/SDdev.hpp"
#include "linux/SDenum.hpp"
#include "linux/libmount/LibMountTab.hpp"
#include "ports/IDeviceResolver.hpp"

class UdevDeviceResolver : public IDeviceResolver {
  public:
    DeviceInfo resolve(std::string_view devNode) override {
        SDdev device(devNode);
        SDdevView usb = device.findUsbDevice();
        return DeviceInfoBuilder().buildFrom(usb);
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