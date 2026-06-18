#pragma once

#include <string>
#include <unordered_map>

#include <sqlite3.h>

#include "exceptions/Exceptions.hpp"

class Row {
    sqlite3_stmt* stmt;
    std::unordered_map<std::string, int> indexMap;

public:
    Row(sqlite3_stmt* stmt) : stmt(stmt) {
        int cols = sqlite3_column_count(stmt);
        for (int i = 0; i < cols; ++i) {
            const char* name = sqlite3_column_name(stmt, i);
            if (name)
                indexMap[name] = i;
        }
    }

    template <typename T>
    T get(std::string_view col) const {
        auto it = indexMap.find(std::string(col));
        if (it == indexMap.end())
            throw SqlDataBaseError("Unknown column: " + std::string(col));

        int i = it->second;

        if constexpr (std::same_as<T, std::string>) {
            const unsigned char* txt = sqlite3_column_text(stmt, i);
            return txt ? reinterpret_cast<const char*>(txt) : "";
        }

        if constexpr (std::integral<T>) {
            return static_cast<T>(sqlite3_column_int64(stmt, i));
        }

        static_assert(!sizeof(T*), "Unsupported type");
    }

    bool has(std::string_view col) const {
        auto it = indexMap.find(std::string(col));
        return it != indexMap.end() &&
               sqlite3_column_text(stmt, it->second) != nullptr;
    }
};