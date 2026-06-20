#include "UsbDeviceContextProvider.hpp"

#include "linux/LibMountTab/LibMountTab.hpp"
#include "linux/SDdev/SDdev.hpp"
#include "linux/SDenum/SDenum.hpp"

DeviceInfo UsbDeviceContextProvider::getDeviceInfo(std::string_view devNode) {
    SDdev device(devNode);
    SDdevView usb = device.findUsbDevice();
    return DeviceInfoBuilder::buildFrom(usb);
}

std::vector<std::string> UsbDeviceContextProvider::getUsbDevNodes() {
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

std::string UsbDeviceContextProvider::getMountPoint(std::string_view devNode) {
    auto mp = LibMountTab().findRecordFromDevNode(devNode).and_then(LibMountTab::extractMountPoint);
    if (!mp)
        throw mp.error();
    return *mp;
}

MountMode UsbDeviceContextProvider::getMountMode(std::string_view mountpoint) {
    auto mode = LibMountTab()
                    .findRecordFromMountPoint(mountpoint)
                    .and_then(LibMountTab::getFSopts)
                    .and_then(LibMountTab::extractMode);
    if (!mode)
        throw mode.error();
    return *mode;
}