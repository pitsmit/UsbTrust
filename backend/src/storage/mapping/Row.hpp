#pragma once

#include <string>
#include <unordered_map>

#include <sqlite3.h>

template <typename T>
concept type = std::same_as<T, std::string> || std::integral<T>;

class Row {
    sqlite3_stmt *stmt;
    std::unordered_map<std::string, int> indexMap;

  public:
    explicit Row(sqlite3_stmt *stmt);

    template <typename T> T get(std::string_view col) const;

    template <type T> static constexpr T extractValue(sqlite3_stmt *stmt, int i) {
        if constexpr (std::same_as<T, std::string>) {
            const unsigned char *txt = sqlite3_column_text(stmt, i);
            return txt ? reinterpret_cast<const char *>(txt) : "";
        }

        return static_cast<T>(sqlite3_column_int64(stmt, i));
    }
};