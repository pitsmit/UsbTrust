#include "managers/DeviceManager/DeviceManager.hpp"

#include <gtest/gtest.h>

#include "entities/DeviceInfo/DeviceInfo.hpp"

#include "../helpers/DataBaseProvider.hpp"

class DeviceManagerTest : public ::testing::Test {
    std::unique_ptr<DataBaseProvider> dbProvider;

  protected:
    std::unique_ptr<DeviceManager> manager;

    void SetUp() override {
        dbProvider = std::make_unique<DataBaseProvider>();
        manager = std::make_unique<DeviceManager>(dbProvider->get_executor());
    }

    void TearDown() override {
        manager.reset();
        dbProvider.reset();
    }

    DeviceInfo makeInfo(const std::string &productId, const std::string &serial) {
        return DeviceInfo{.vendorId = "ABCD",
                          .productId = productId,
                          .serial = serial,
                          .vendorName = "Samsung",
                          .productName = "SomeUsb"};
    }
};

TEST_F(DeviceManagerTest, GetEmtyWhiteList) {
    // ACT
    auto res = manager->getWhitelist();

    // ASSERT
    EXPECT_TRUE(res.empty());
}

TEST_F(DeviceManagerTest, AddDeviceToWhitelist) {
    // ARRANGE
    auto info = makeInfo("0001", "SERIAL1");

    // ACT
    auto id = manager->addToWhitelist(info);

    // ASSERT
    auto list = manager->getWhitelist();
    EXPECT_GT(id, 0);
    ASSERT_EQ(list.size(), 1);

    EXPECT_EQ(list[0].id, id);
    EXPECT_EQ(list[0].info, info);
}

TEST_F(DeviceManagerTest, RemoveDeviceFromWhitelist) {
    // ARRANGE
    auto id = manager->addToWhitelist(makeInfo("0001", "SERIAL1"));

    // ACT
    manager->removeFromWhitelist(id);

    // ASSERT
    EXPECT_TRUE(manager->getWhitelist().empty());
}

TEST_F(DeviceManagerTest, IsAllowedReturnsIdForExistingDevice) {
    // ARRANGE
    auto info = makeInfo("0001", "SERIAL1");
    auto id = manager->addToWhitelist(info);

    // ACT
    auto res = manager->isAllowed(info);

    // ASSERT
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(*res, id);
}

TEST_F(DeviceManagerTest, IsAllowedReturnsNulloptForUnknownDevice) {
    // ARRANGE
    manager->addToWhitelist(makeInfo("0001", "SERIAL1"));

    // ACT
    auto res = manager->isAllowed(makeInfo("0002", "SERIAL2"));

    // ASSERT
    EXPECT_FALSE(res.has_value());
}

TEST_F(DeviceManagerTest, ExpiredDeviceIsNotAllowed) {
    // ARRANGE
    auto info = makeInfo("0001", "SERIAL1");
    auto id = manager->addToWhitelist(info);

    manager->patchValidTo(id, "2000-01-01 00:00:00");

    // ACT
    auto res = manager->isAllowed(info);

    // ASSERT
    EXPECT_FALSE(res.has_value());
}

TEST_F(DeviceManagerTest, GetWhitelistReturnsAllDevices) {
    // ARRANGE
    manager->addToWhitelist(makeInfo("0001", "SERIAL1"));
    manager->addToWhitelist(makeInfo("0002", "SERIAL2"));

    // ACT
    auto list = manager->getWhitelist();

    // ASSERT
    EXPECT_EQ(list.size(), 2);
}