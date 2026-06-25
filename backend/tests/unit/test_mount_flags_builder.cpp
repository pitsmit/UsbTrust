#include <gtest/gtest.h>
#include <sys/mount.h>

#include "entities/MountMode/MountMode.hpp"
#include "linux/LinuxMountSystem/MountFlagsBuilder.hpp"

TEST(MountFlagsBuilderTest, BuildFromReadOnlyMode) {
    // ARRANGE
    constexpr auto mode = MountMode::ro();
    constexpr auto exp_flags = MS_RDONLY | MS_NOEXEC;

    // ACT
    auto result = MountFlagsBuilder().withFlagsFrom(mode).build();

    // ASSERT
    EXPECT_EQ(result, exp_flags);
}

TEST(MountFlagsBuilderTest, BuildFromReadWriteMode) {
    // ARRANGE
    constexpr auto mode = MountMode::rw();
    constexpr auto exp_flags = 0;

    // ACT
    auto result = MountFlagsBuilder().withFlagsFrom(mode).build();

    // ASSERT
    EXPECT_EQ(result, exp_flags);
}

TEST(MountFlagsBuilderTest, BuildFromReadWriteModeWithRemount) {
    // ARRANGE
    constexpr auto mode = MountMode::rw();
    constexpr auto exp_flags = 0 | MS_REMOUNT;

    // ACT
    auto result = MountFlagsBuilder().withFlagsFrom(mode).withRemount().build();

    // ASSERT
    EXPECT_EQ(result, exp_flags);
}