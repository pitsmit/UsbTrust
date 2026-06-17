#pragma once

#include <string>
#include <filesystem>

class DBConnection;

class DBInitializer {
private:
    static std::string loadSQL(const std::filesystem::path& path);

public:
    static void init(DBConnection& db);
};