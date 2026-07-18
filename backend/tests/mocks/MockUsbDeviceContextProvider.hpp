#pragma once

#include <gmock/gmock.h>

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "entities/MountMode/MountMode.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"

class MockUsbDeviceContextProvider : public IUsbDeviceContextProvider {
  public:
    MOCK_METHOD(DeviceInfo, getDeviceInfo, (const core::path &devNode), (override));
    MOCK_METHOD(std::vector<core::path>, getUsbDevNodes, (), (override));
    MOCK_METHOD(core::path, getMountPoint, (const core::path &devNode), (override));
    MOCK_METHOD(MountMode, getMountMode, (const core::path &mountpoint), (override));
    MOCK_METHOD(std::string, getFsType, (const core::path &dev), (override));
};