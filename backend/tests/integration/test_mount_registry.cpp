#include "managers/MountRegistryManager/MountRegistryManager.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "entities/MountRecord/MountRecord.hpp"

#include "../helpers/DataBaseProvider.hpp"

class MountRegistryTest : public ::testing::Test {
    std::unique_ptr<DataBaseProvider> dbProvider;

  protected:
    std::unique_ptr<MountRegistryManager> registrator;

    void SetUp() override {
        dbProvider = std::make_unique<DataBaseProvider>();
        registrator = std::make_unique<MountRegistryManager>(dbProvider->get_executor());
    }

    void TearDown() override {
        registrator.reset();
        dbProvider.reset();
    }

    MountRecord makeRecord(int mountpoint_modificator,
                           const std::string &devNode,
                           const std::string &productId,
                           const std::string &serial) {
        return MountRecord{
            .id = std::nullopt,
            .devNode = std::string(devNode),
            .mountPoint = "m" + std::to_string(mountpoint_modificator),
            .info = DeviceInfo{.vendorId = "ABCD",
                               .productId = productId,
                               .serial = serial,
                               .vendorName = "Samsung",
                               .productName = "SomeUsb"},
            .mode = MountMode::ro(),
        };
    }
};

TEST_F(MountRegistryTest, GetAll_ReturnsAllRecords) {
    // ARRANGE
    const std::vector<MountRecord> expected = {
        makeRecord(1, "/dev/sda1", "1234", "ACXDIFTGX6459KOD"),
        makeRecord(2, "/dev/sdb1", "1244", "ACXDIFTGX6459KRD"),
        makeRecord(3, "/dev/sdc1", "1254", "ACXDIFTP86459KOD"),
    };

    for (const auto &record : expected) {
        registrator->add(record);
    }

    // ACT
    const auto actual = registrator->getAll();

    // ASSERT
    EXPECT_THAT(actual, ::testing::UnorderedElementsAreArray(expected));
}

TEST_F(MountRegistryTest, AfterRemoveRecordIsNoRecords) {
    // ARRANGE
    constexpr auto node = "/dev/sda1";
    registrator->add(makeRecord(1, node, "1234", "ACXDIFTGX6459KOD"));

    // ACT
    registrator->removeByDevNode(node);

    // ASSERT
    EXPECT_TRUE(registrator->getAll().empty());
}

TEST_F(MountRegistryTest, GetByIdEmpty) {
    // ARRANGE
    constexpr auto id = 1;

    // ACT
    auto rec = registrator->getById(id);

    // ASSERT
    EXPECT_FALSE(rec);
}

TEST_F(MountRegistryTest, UpdateMountPointAndModeInRecord) {
    // ARRANGE
    auto record = makeRecord(1, "/dev/sda1", "1234", "ACXDIFTGX6459KOD");
    registrator->add(record);
    record.mountPoint = "some/new/mount/point";
    record.mode = MountMode::rw();

    // ACT
    registrator->refresh(record);

    // ASSERT
    auto actual = registrator->getAll().front();
    EXPECT_EQ(actual, record);
}