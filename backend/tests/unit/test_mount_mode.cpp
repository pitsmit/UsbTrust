#include "entities/MountMode/MountMode.hpp"

#include <gtest/gtest.h>

TEST(MountModeTest, CreateROmode) {
    // ACT
    auto result = MountMode::ro();

    // ASSERT
    EXPECT_TRUE(result.isReadOnly());
}

TEST(MountModeTest, CreateRWmode) {
    // ACT
    auto result = MountMode::rw();

    // ASSERT
    EXPECT_TRUE(result.isReadWrite());
}

TEST(MountModeTest, CheckComparisonTrue) {
    // ARRANGE
    constexpr auto m1 = MountMode::ro();
    constexpr auto m2 = MountMode::ro();

    // ACT
    auto result = m1 == m2;

    // ASSERT
    EXPECT_TRUE(result);
}

TEST(MountModeTest, CheckComparisonFalse) {
    // ARRANGE
    constexpr auto m1 = MountMode::ro();
    constexpr auto m2 = MountMode::rw();

    // ACT
    auto result = m1 == m2;

    // ASSERT
    EXPECT_FALSE(result);
}

TEST(MountModeTest, CheckFromBool) {
    // ARRANGE
    constexpr auto bl = true;

    // ACT
    auto result = MountMode::fromBool(bl);

    // ASSERT
    EXPECT_TRUE(result.isReadWrite());
}

TEST(MountModeTest, ToString) {
    // ARRANGE
    constexpr auto m1 = MountMode::ro();
    constexpr auto expected = "ro";

    // ACT
    auto result = m1.toString();

    // ASSERT
    EXPECT_STREQ(result.c_str(), expected);
}

TEST(MountModeTest, Parse) {
    // ARRANGE
    constexpr auto md = "ro";

    // ACT
    auto result = MountMode::parse(md);

    // ASSERT
    EXPECT_TRUE(result.isReadOnly());
}