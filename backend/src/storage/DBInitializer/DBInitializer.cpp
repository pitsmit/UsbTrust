#include "DBInitializer.hpp"

#include <format>
#include <fstream>
#include <sstream>

#include "exceptions/Exceptions.hpp"
#include "infrastructure/config/Config.hpp"
#include "storage/SqlExecutor/SqlExecutor.hpp"

std::string DBInitializer::loadSQL(const std::filesystem::path &path) {
    std::ifstream file(path);
    if (!file) {
        throw FileException(std::format("Cannot open schema file: {}", path.string()));
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void DBInitializer::init(SqlExecutor &executor) {
    executor.exec("PRAGMA foreign_keys = ON;");
    for (const auto &path : Config::getSchemaPaths())
        executor.exec(loadSQL(path));
}