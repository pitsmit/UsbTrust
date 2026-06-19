#pragma once

#include <optional>
#include <sqlite3.h>
#include <string>
#include <string_view>

namespace db::bind {

inline void bind(sqlite3_stmt *stmt, int idx, int value) {
    sqlite3_bind_int(stmt, idx, value);
}

inline void bind(sqlite3_stmt *stmt, int idx, const std::string &value) {
    sqlite3_bind_text(stmt, idx, value.c_str(), -1, SQLITE_TRANSIENT);
}

inline void bind(sqlite3_stmt *stmt, int idx, const char *value) {
    sqlite3_bind_text(stmt, idx, value, -1, SQLITE_TRANSIENT);
}

inline void bind(sqlite3_stmt *stmt, int idx, std::string_view value) {
    sqlite3_bind_text(stmt, idx, value.data(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
}

inline void bind(sqlite3_stmt *stmt, int idx, const std::optional<std::string> &value) {
    if (value)
        bind(stmt, idx, *value);
    else
        sqlite3_bind_null(stmt, idx);
}

template <typename... Args> void bind_all(sqlite3_stmt *stmt, Args &&...args) {
    int idx = 1;
    (db::bind::bind(stmt, idx++, std::forward<Args>(args)), ...);
}

} // namespace db::bind