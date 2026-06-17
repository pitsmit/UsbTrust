#include "DBInitializer.hpp"

#include <fstream>
#include <sstream>

#include "DBConnection.hpp"
#include "exceptions/Exceptions.hpp"
#include "infrastructure/config/Config.hpp"

std::string DBInitializer::loadSQL(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw FileException(("Cannot open schema file: " + path.string()).c_str());
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


void DBInitializer::init(DBConnection& db) {
    db.execute("PRAGMA foreign_keys = ON;");
    for (const auto& path : Config::getSchemaPaths())
        db.execute(loadSQL(path));
}