#include "managers/MountManager/MountManager.hpp"

#include <gtest/gtest.h>

#include "linux/LinuxMountSystem/LinuxMountSystem.hpp"
#include "services/MountService/MountService.hpp"

#include "../helpers/DataBaseProvider.hpp"
#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/MountHelpers.hpp"

#include "../mocks/MockUsbDeviceContextProvider.hpp"

class MountManagerTest : public ::testing::Test {
    std::unique_ptr<DataBaseProvider> dbProvider;
    std::unique_ptr<LinuxMountSystem> linux_system;
    std::unique_ptr<MountService> mount_service;

  protected:
    std::unique_ptr<DeviceManager> dev_manager;
    std::unique_ptr<MockUsbDeviceContextProvider> info_provider;
    std::unique_ptr<MountManager> manager;

    void SetUp() override {
        LoggerTestHelper().disable();
        dbProvider = std::make_unique<DataBaseProvider>();
        info_provider = std::make_unique<MockUsbDeviceContextProvider>();
        linux_system = std::make_unique<LinuxMountSystem>();
        mount_service = std::make_unique<MountService>(*linux_system, *info_provider);
        dev_manager = std::make_unique<DeviceManager>(dbProvider->get_executor());
        manager = std::make_unique<MountManager>(*dev_manager, *mount_service, *info_provider);
    }

    void TearDown() override {
        manager.reset();
        dev_manager.reset();
        mount_service.reset();
        linux_system.reset();
        info_provider.reset();
        dbProvider.reset();
        LoggerTestHelper().restore();
    }
};

using ::testing::_;
using ::testing::Return;

TEST_F(MountManagerTest, CorrectMountReadOnlyTest) {
    // ARRANGE
    const auto info = DeviceInfo{.vendorId = "1234",
                                 .productId = "5678",
                                 .serial = "ABCDEF",
                                 .vendorName = "Samsung",
                                 .productName = "Flash Drive"};
    auto lp = createLoopFs("vfat");

    EXPECT_CALL(*info_provider, getDeviceInfo(_)).WillOnce(Return(info));
    EXPECT_CALL(*info_provider, getFsType(_)).WillOnce(Return("vfat"));

    // ACT
    auto record = manager->mount(lp.device);

    // ASSERT
    EXPECT_EQ(info, record.info);
    EXPECT_TRUE(record.mode.isReadOnly());
    EXPECT_FALSE(record.device_id.has_value());
    EXPECT_STREQ(record.devNode.c_str(), lp.device.c_str());
}

TEST_F(MountManagerTest, CorrectMountReadWriteTest) {
    // ARRANGE
    const auto info = DeviceInfo{.vendorId = "1234",
                                 .productId = "5678",
                                 .serial = "ABCDEF",
                                 .vendorName = "Samsung",
                                 .productName = "Flash Drive"};
    auto id = dev_manager->addToWhitelist(info);
    auto lp = createLoopFs("vfat");

    EXPECT_CALL(*info_provider, getDeviceInfo(_)).WillOnce(Return(info));
    EXPECT_CALL(*info_provider, getFsType(_)).WillOnce(Return("vfat"));

    // ACT
    auto record = manager->mount(lp.device);

    // ASSERT
    EXPECT_EQ(info, record.info);
    EXPECT_TRUE(record.mode.isReadWrite());
    EXPECT_TRUE(record.device_id.has_value() && *record.device_id == id);
    EXPECT_STREQ(record.devNode.c_str(), lp.device.c_str());
    dev_manager->removeFromWhitelist(id);
}

TEST_F(MountManagerTest, FakeNodeMountTest) {
    // ARRANGE
    const auto info = DeviceInfo{.vendorId = "1234",
                                 .productId = "5678",
                                 .serial = "ABCDEF",
                                 .vendorName = "Samsung",
                                 .productName = "Flash Drive"};

    EXPECT_CALL(*info_provider, getDeviceInfo(_)).WillOnce(Return(info));
    EXPECT_CALL(*info_provider, getFsType(_)).WillOnce(Return("vfat"));

    // ACT && ASSERT
    EXPECT_THROW(manager->mount("fake/node"), MountError);
}

TEST_F(MountManagerTest, CorrectUnmountTest) {
    // ARRANGE
    auto lp = createLoopFs("vfat");
    lp.mnt();

    // ACT
    manager->unmount(lp.mountPoint);

    // ASSERT
    EXPECT_FALSE(lp.isMounted());
}

TEST_F(MountManagerTest, FakeMountPointUnmountTest) {
    // ACT && ASSERT
    EXPECT_THROW(manager->unmount("fake"), UnMountError);
}

TEST_F(MountManagerTest, CorrectRemountToReadWriteTest) {
    // ARRANGE
    auto lp = createLoopFs("vfat");
    lp.mnt();
    auto record = MountRecord{.mountPoint = lp.mountPoint, .mode = MountMode::rw()};

    // ACT
    manager->remount(record);

    // ASSERT
    EXPECT_TRUE(lp.currentMountMode().isReadWrite());
    EXPECT_STREQ(lp.currentMountPoint().c_str(), lp.mountPoint.c_str());
}

TEST_F(MountManagerTest, FakeRecordRemountTest) {
    // ACT && ASSERT
    EXPECT_THROW(manager->remount(MountRecord{.mountPoint = "fake", .mode = MountMode::rw()}),
                 MountError);
}