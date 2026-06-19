#include "Row.hpp"

#include <format>

#include "exceptions/Exceptions.hpp"

Row::Row(sqlite3_stmt *stmt) : stmt(stmt) {
    auto cols = sqlite3_column_count(stmt);
    for (auto i = 0; i < cols; ++i) {
        auto name = sqlite3_column_name(stmt, i);
        if (name)
            indexMap[name] = i;
    }
}

template <typename T> T Row::get(std::string_view col) const {
    auto it = indexMap.find(std::string(col));
    if (it == indexMap.end())
        throw SqlDataBaseError(std::format("Unknown column: {}", col));
    return extractValue<T>(stmt, it->second);
}