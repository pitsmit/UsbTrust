#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include "infrastructure/config/Config.hpp"

#include "../helpers/ConfigFileBuilder.hpp"

class ConfigTest : public ::testing::Test {
  protected:
    inline static std::string originalContent;

    inline static constexpr const char *FILE_NAME = "config.txt";
    inline static constexpr const char *BASE_FILE_NAME = "config.base.txt";

    static void restoreConfig() {
        std::ofstream output(FILE_NAME, std::ios::binary | std::ios::trunc);

        output << originalContent;
    }

    static void SetUpTestSuite() {
        std::ifstream input(BASE_FILE_NAME, std::ios::binary);

        originalContent.assign(std::istreambuf_iterator<char>(input),
                               std::istreambuf_iterator<char>());
    }

    static void TearDownTestSuite() {
        restoreConfig();
    }

    void SetUp() override {
        restoreConfig();
    }
};

TEST_F(ConfigTest, MissingConfigFile) {
    // ARRANGE
    std::remove(FILE_NAME);

    // ACT && ASSERT
    EXPECT_THROW(Config::getHttpPort(), FileException);
}

TEST_F(ConfigTest, MissingHttpPortKey) {
    // ARRANGE
    ConfigFileBuilder().add("db.path", "/tmp/db").add("log.file", "/tmp/log").build();

    // ACT && ASSERT
    EXPECT_THROW(Config::getHttpPort(), FileException);
}

TEST_F(ConfigTest, ValidConfigReturnsHttpPort) {
    // ARRANGE
    constexpr auto port = 8080;

    ConfigFileBuilder()
        .add("http.port", std::to_string(port))
        .add("db.path", "/tmp/db")
        .add("log.file", "/tmp/log")
        .build();

    // ACT
    int result = Config::getHttpPort();

    // ASSERT
    EXPECT_EQ(result, port);
}