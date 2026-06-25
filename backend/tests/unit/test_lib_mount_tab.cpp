#include "linux/LibMountTab/LibMountTab.hpp"

#include <gtest/gtest.h>

#include "../helpers/MountHelpers.hpp"

TEST(LibMountTabTest, FindRecordFromExistingDevNode) {
    // ARRANGE
    auto lp = createLoopFs("vfat");
    lp.mnt();

    // ACT
    const auto fs = LibMountTab().findRecordFromDevNode(lp.device);

    // ASSERT
    EXPECT_TRUE(fs);
}

TEST(LibMountTabTest, FindRecordFromFakeDevNode) {
    // ARRANGE
    constexpr auto fake = "node";

    // ACT
    const auto fs = LibMountTab().findRecordFromDevNode(fake);

    // ASSERT
    EXPECT_FALSE(fs);
}

TEST(LibMountTabTest, FindRecordFromExistingMountPoint) {
    // ARRANGE
    auto lp = createLoopFs("vfat");
    lp.mnt();

    // ACT
    const auto fs = LibMountTab().findRecordFromMountPoint(lp.mountPoint);

    // ASSERT
    EXPECT_TRUE(fs);
}

TEST(LibMountTabTest, FindRecordFromFakeMountPoint) {
    // ARRANGE
    constexpr auto fake = "mountpoint";

    // ACT
    const auto fs = LibMountTab().findRecordFromMountPoint(fake);

    // ASSERT
    EXPECT_FALSE(fs);
}