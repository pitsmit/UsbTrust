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
};

TEST_F(MountRegistryTest, GetAll_ReturnsAllRecords) {
    auto makeRecord =
        [](int id, std::string_view devNode, std::string_view productId, std::string_view serial) {
            return MountRecord{
                .id = id,
                .devNode = std::string(devNode),
                .mountPoint = "m" + std::to_string(id),
                .info = DeviceInfo{.vendorId = "ABCD",
                                   .productId = productId.data(),
                                   .serial = serial.data(),
                                   .vendorName = "Samsung",
                                   .productName = "SomeUsb"},
                .mode = MountMode::ro(),
            };
        };

    const std::vector<MountRecord> expected = {
        makeRecord(1, "/dev/sda1", "1234", "ACXDIFTGX6459KOD"),
        makeRecord(2, "/dev/sdb1", "1244", "ACXDIFTGX6459KRD"),
        makeRecord(3, "/dev/sdc1", "1254", "ACXDIFTP86459KOD"),
    };

    // ARRANGE
    for (const auto &record : expected) {
        registrator->add(record);
    }

    // ACT
    const auto actual = registrator->getAll();

    // ASSERT
    EXPECT_THAT(actual, ::testing::UnorderedElementsAreArray(expected));
}