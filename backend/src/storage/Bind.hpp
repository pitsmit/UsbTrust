#pragma once

#include <optional>
#include <sqlite3.h>
#include <string>
#include <string_view>

#include "types/types.hpp"

class Bind {
    static inline void bind(sqlite3_stmt *stmt, int idx, int value) {
        sqlite3_bind_int(stmt, idx, value);
    }

    static inline void bind(sqlite3_stmt *stmt, int idx, const std::string &value) {
        sqlite3_bind_text(stmt, idx, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    static inline void bind(sqlite3_stmt *stmt, int idx, const core::path &value) {
        sqlite3_bind_text(stmt, idx, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    static inline void bind(sqlite3_stmt *stmt, int idx, const char *value) {
        sqlite3_bind_text(stmt, idx, value, -1, SQLITE_TRANSIENT);
    }

    static inline void bind(sqlite3_stmt *stmt, int idx, std::string_view value) {
        sqlite3_bind_text(
            stmt, idx, value.data(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
    }

    static inline void bind(sqlite3_stmt *stmt, int idx, const std::optional<std::string> &value) {
        if (value)
            bind(stmt, idx, *value);
        else
            sqlite3_bind_null(stmt, idx);
    }

  public:
    template <typename... Args>
    static void bind_all([[maybe_unused]] sqlite3_stmt *stmt, Args &&...args) {
        int idx = 1;
        (bind(stmt, idx++, args), ...);
    }
};