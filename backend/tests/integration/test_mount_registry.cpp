#include "managers/MountRegistryManager/MountRegistryManager.hpp"

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
    // ARRANGE
    const std::string nodes[] = {"/dev/sda1", "/dev/sdb1", "/dev/sdc1"};
    const DeviceInfo infos[] = {DeviceInfoBuilder()
                                    .withProductId("1234")
                                    .withVendorId("ABCD")
                                    .withSerial("ACXDIFTGX6459KOD")
                                    .build(),
                                DeviceInfoBuilder()
                                    .withProductId("1244")
                                    .withVendorId("ABCD")
                                    .withSerial("ACXDIFTGX6459KRD")
                                    .build(),
                                DeviceInfoBuilder()
                                    .withProductId("1254")
                                    .withVendorId("ABCD")
                                    .withSerial("ACXDIFTP86459KOD")
                                    .build()};

    for (int i = 0; i < 3; ++i) {
        registrator->add(MountRecord{.id = i + 1,
                                     .devNode = nodes[i],
                                     .mountPoint = "m" + std::to_string(i + 1),
                                     .info = infos[i],
                                     .mode = MountMode::ro()});
    }

    // ACT
    auto records = registrator->getAll();

    // ASSERT
    ASSERT_EQ(records.size(), 3);

    for (const auto &node : nodes) {
        EXPECT_TRUE(std::any_of(records.begin(), records.end(), [&](const MountRecord &r) {
            return r.devNode == node;
        }));
    }
}