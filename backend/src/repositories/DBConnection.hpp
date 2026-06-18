#pragma once

#include <string>
#include <vector>

#include <sqlite3.h>

#include "exceptions/Exceptions.hpp"
#include "repositories/Row.hpp"
#include "repositories/Statement.hpp"

class DBConnection {
  private:
    sqlite3 *db = nullptr;

    static void bind(sqlite3_stmt *stmt, int idx, int value) {
        sqlite3_bind_int(stmt, idx, value);
    }

    static void bind(sqlite3_stmt *stmt, int idx, const std::string &value) {
        sqlite3_bind_text(stmt, idx, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    static void bind(sqlite3_stmt *stmt, int idx, const char *value) {
        sqlite3_bind_text(stmt, idx, value, -1, SQLITE_TRANSIENT);
    }

    static void bind(sqlite3_stmt *stmt, int idx, std::string_view value) {
        sqlite3_bind_text(
            stmt, idx, value.data(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
    }

    static void bind(sqlite3_stmt *stmt, int idx, const std::optional<std::string> &value) {
        if (value)
            bind(stmt, idx, *value);
        else
            sqlite3_bind_null(stmt, idx);
    }

    template <typename F>
    decltype(auto) withStatement(std::string_view sql, F&& f) {
        Statement stmt(db, sql);
        return std::forward<F>(f)(stmt.get());
    }

  public:
    explicit DBConnection(std::string_view dbPath) {
        if (sqlite3_open(dbPath.data(), &db) != SQLITE_OK) {
            throw SqlDataBaseError("Failed to open SQLite database");
        }
    }

    ~DBConnection() {
        sqlite3_close(db);
    }

    DBConnection(const DBConnection &) = delete;
    DBConnection &operator=(const DBConnection &) = delete;

    template <typename... Args>
    void exec(std::string_view sql, const Args&... args) {
        withStatement(sql, [&](sqlite3_stmt* stmt) {
            int idx = 1;
            (bind(stmt, idx++, args), ...);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw SqlDataBaseError(sqlite3_errmsg(db));
            }
        });
    }

    template <typename T, typename... Args>
    std::optional<T> scalar(std::string_view sql, const Args&... args) {
        return withStatement(sql, [&](sqlite3_stmt* stmt) -> std::optional<T> {
            int idx = 1;
            (bind(stmt, idx++, args), ...);

            if (sqlite3_step(stmt) != SQLITE_ROW)
                return std::nullopt;

            if constexpr (std::integral<T>) {
                return static_cast<T>(sqlite3_column_int64(stmt, 0));
            }

            if constexpr (std::same_as<T, std::string>) {
                const unsigned char* txt = sqlite3_column_text(stmt, 0);
                if (!txt) return std::nullopt;
                return std::string(reinterpret_cast<const char*>(txt));
            }

            static_assert(!sizeof(T*), "Unsupported scalar type");
        });
    }

    template <typename T, typename Mapper, typename... Args>
    std::vector<T> query(std::string_view sql, Mapper mapper, const Args&... args) {
        std::vector<T> result;

        withStatement(sql, [&](sqlite3_stmt* stmt) {
            int idx = 1;
            (bind(stmt, idx++, args), ...);

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                Row row(stmt);
                result.push_back(mapper(row));
            }
        });

        return result;
    }
};