#pragma once

#include <filesystem>
#include <format>
#include <string>
#include <unordered_map>

#include <sqlite3.h>

#include "exceptions/Exceptions.hpp"

class Row {
    sqlite3_stmt *stmt;
    std::unordered_map<std::string, int> indexMap;

  public:
    explicit Row(sqlite3_stmt *stmt) : stmt(stmt) {
        auto cols = sqlite3_column_count(stmt);
        for (auto i = 0; i < cols; ++i) {
            auto name = sqlite3_column_name(stmt, i);
            if (name)
                indexMap[name] = i;
        }
    }

    template <typename T> T get(std::string_view col) const {
        auto it = indexMap.find(std::string(col));
        if (it == indexMap.end())
            throw SqlDataBaseError(std::format("Unknown column: {}", col));
        return extractValue<T>(stmt, it->second);
    }

    template <typename T> constexpr static T extractValue(sqlite3_stmt *stmt, int i) {
        if constexpr (std::same_as<T, std::string> || std::same_as<T, std::filesystem::path>) {
            const unsigned char *txt = sqlite3_column_text(stmt, i);
            return txt ? reinterpret_cast<const char *>(txt) : "";
        } else if constexpr (std::integral<T>) {
            return static_cast<T>(sqlite3_column_int64(stmt, i));
        } else {
            static_assert(sizeof(T) == 0, "Unsupported type for SQLite extraction");
        }
    }
};