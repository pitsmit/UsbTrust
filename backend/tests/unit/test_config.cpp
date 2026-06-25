#include "infrastructure/config/Config.hpp"

#include <gtest/gtest.h>

#include "../helpers/ConfigFileBuilder.hpp"

#include <cstdio>
#include <format>
#include <fstream>
#include <string>

#include "exceptions/Exceptions.hpp"

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
        .add("db.path", "/tmp/db")
        .add("http.port", std::to_string(port))
        .add("log.file", "/tmp/log")
        .build();

    // ACT
    int result = Config::getHttpPort();

    // ASSERT
    EXPECT_EQ(result, port);
}

TEST_F(ConfigTest, WebSocketPortIsaTrashString) {
    // ARRANGE
    constexpr auto port = "some trash";
    constexpr auto name = "websocket.port";

    ConfigFileBuilder()
        .add("db.path", "/tmp/db")
        .add(name, port)
        .add("log.file", "/tmp/log")
        .build();

    // ACT && ASSERT
    try {
        Config::getWebSocketPort();
        FAIL();
    } catch (const FileException &e) {
        const auto expected_message = std::format("{} in config is not a number!", name);
        EXPECT_STREQ(e.what(), expected_message.c_str());
    } catch (...) {
        FAIL() << "Expected FileException";
    }
}

TEST_F(ConfigTest, HttpPortIsaBigNumber) {
    // ARRANGE
    constexpr auto port = "9999999999999999999999999999999999";
    constexpr auto name = "http.port";

    ConfigFileBuilder()
        .add("db.path", "/tmp/db")
        .add(name, port)
        .add("log.file", "/tmp/log")
        .build();

    // ACT && ASSERT
    try {
        Config::getHttpPort();
        FAIL();
    } catch (const FileException &e) {
        const auto expected_message = std::format("{} in config is out of range!", name);
        EXPECT_STREQ(e.what(), expected_message.c_str());
    } catch (...) {
        FAIL() << "Expected FileException";
    }
}

TEST_F(ConfigTest, ValidConfigReturnsSchemaPaths) {
    // ARRANGE
    constexpr auto base = "test";
    std::filesystem::create_directories(base);
    const auto f1 = std::filesystem::path(base) / "test1.sql";
    const auto f2 = std::filesystem::path(base) / "test2.sql";
    const auto f3 = std::filesystem::path(base) / "test3.sql";
    std::vector<std::filesystem::path> expected{f1, f2, f3};

    std::ofstream{f1};
    std::ofstream{f2};
    std::ofstream{f3};

    ConfigFileBuilder()
        .add("db.path", "/tmp/db")
        .add("db.schema.dir", base)
        .add("log.file", "/tmp/log")
        .build();

    // ACT
    auto result = Config::getSchemaPaths();

    // ASSERT
    EXPECT_EQ(result, expected);
    std::filesystem::remove_all(base);
}

TEST_F(ConfigTest, ValidConfigReturnsNoPaths) {
    // ARRANGE
    constexpr auto base = "test";
    std::filesystem::create_directories(base);
    std::vector<std::filesystem::path> expected{};
    ConfigFileBuilder()
        .add("db.path", "/tmp/db")
        .add("db.schema.dir", base)
        .add("log.file", "/tmp/log")
        .build();

    // ACT
    auto result = Config::getSchemaPaths();

    // ASSERT
    EXPECT_EQ(result, expected);
    std::filesystem::remove_all(base);
}

TEST_F(ConfigTest, ValidConfigButFakeFolder) {
    // ARRANGE
    constexpr auto base = "test";
    ConfigFileBuilder()
        .add("db.path", "/tmp/db")
        .add("db.schema.dir", base)
        .add("log.file", "/tmp/log")
        .build();

    // ACT && ASSERT
    EXPECT_THROW(Config::getSchemaPaths(), FileException);
}