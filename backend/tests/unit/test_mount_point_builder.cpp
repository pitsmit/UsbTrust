#include "services/MountPointBuilder.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <string>

TEST(MountPointBuilderTest, Build_AllFieldsPresent) {
    // ARRANGE
    constexpr auto vendorId = "1234";
    constexpr auto productId = "7856";
    constexpr auto serial = "ACDC456IRHX";
    constexpr auto productName = "Galaxy";
    constexpr auto vendorName = "Samsung";

    auto dev = DeviceInfoBuilder()
                   .withVendorId(vendorId)
                   .withProductId(productId)
                   .withSerial(serial)
                   .withProductName(productName)
                   .withVendorName(vendorName)
                   .build();

    auto expected = std::filesystem::path("/media/dlp") / vendorId / productId / serial;

    // ACT
    auto result = MountPointBuilder::build(dev);

    // ASSERT
    EXPECT_EQ(result, expected);
}

TEST(MountPointBuilderTest, EnsureExists_CreatesDirectory) {
    // ARRANGE
    constexpr auto path = "/tmp/test_mount_point_builder_dir";
    std::filesystem::remove_all(path);

    // ACT
    MountPointBuilder::ensureExists(path);

    // ASSERT
    EXPECT_TRUE(std::filesystem::exists(path));
}

TEST(MountPointBuilderTest, EnsureExists_Idempotent) {
    // ARRANGE
    constexpr auto path = "/tmp/test_mount_point_builder_dir2";
    std::filesystem::remove_all(path);

    // ACT
    MountPointBuilder::ensureExists(path);
    MountPointBuilder::ensureExists(path);

    // ASSERT
    EXPECT_TRUE(std::filesystem::exists(path));
}