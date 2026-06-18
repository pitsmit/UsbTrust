#pragma once

#include <functional>
#include <string>

#include <sqlite3.h>

#include "exceptions/Exceptions.hpp"

class DBConnection {
  private:
    sqlite3 *db = nullptr;
    using QueryCallback = std::function<void(int, char **, char **)>;

    static void throwSQLiteError(char *errMsg) {
        std::string error = errMsg ? errMsg : "SQLite error";
        sqlite3_free(errMsg);
        throw SqlDataBaseError(error.c_str());
    }

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

  public:
    explicit DBConnection(const std::string &dbPath) {
        if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
            throw SqlDataBaseError("Failed to open SQLite database");
        }
    }

    ~DBConnection() {
        sqlite3_close(db);
    }

    DBConnection(const DBConnection &) = delete;
    DBConnection &operator=(const DBConnection &) = delete;

    void execute(std::string_view sql) {
        char *errMsg = nullptr;
        if (sqlite3_exec(db, sql.data(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throwSQLiteError(errMsg);
        }
    }

    template <typename... Args> void execute(std::string_view sql, const Args &...args) {
        sqlite3_stmt *stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw SqlDataBaseError(sqlite3_errmsg(db));
        }
        int index = 1;
        (bind(stmt, index++, args), ...);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            auto err = std::string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw SqlDataBaseError(err.c_str());
        }
        sqlite3_finalize(stmt);
    }

    void query(std::string_view sql, QueryCallback callback) {
        auto trampoline = [](void *data, int cols, char **values, char **names) -> int {
            auto *cb = static_cast<QueryCallback *>(data);
            (*cb)(cols, values, names);
            return 0;
        };

        char *errMsg = nullptr;
        if (sqlite3_exec(db, sql.data(), trampoline, &callback, &errMsg) != SQLITE_OK) {
            throwSQLiteError(errMsg);
        }
    }

    template <typename T> std::optional<T> queryScalar(const std::string &sql) {
        std::optional<T> result;
        query(sql, [&](int, char **vls, char **) {
            if (!vls || !vls[0])
                return;
            if constexpr (std::is_same_v<T, int>)
                result = std::stoi(vls[0]);
            else if constexpr (std::is_same_v<T, std::string>)
                result = vls[0];
        });
        return result;
    }

    template <typename T, typename... Args>
    std::optional<T> queryScalar(std::string_view sql, const Args &...args) {
        sqlite3_stmt *stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw SqlDataBaseError(sqlite3_errmsg(db));
        }
        try {
            int idx = 1;
            (bind(stmt, idx++, args), ...);
            std::optional<T> result;
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                if constexpr (std::integral<T>) {
                    result = static_cast<T>(sqlite3_column_int64(stmt, 0));
                } else if constexpr (std::same_as<T, std::string>) {
                    auto *txt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));

                    if (txt)
                        result = txt;
                }
            }
            sqlite3_finalize(stmt);
            return result;
        } catch (...) {
            sqlite3_finalize(stmt);
            throw;
        }
    }

    template <typename T, typename Mapper>
    std::vector<T> queryAll(const std::string &sql, Mapper mapper) {
        std::vector<T> result;
        query(sql, [&](int, char **values, char **) {
            if (values)
                result.push_back(mapper(values));
        });
        return result;
    }

    template <typename T, typename Mapper>
    std::optional<T> queryOne(const std::string &sql, Mapper mapper) {
        std::optional<T> result;
        query(sql, [&](int, char **values, char **) {
            if (values && !result)
                result = mapper(values);
        });
        return result;
    }

    template <typename T, typename Mapper, typename... Args>
    std::optional<T> queryOne(std::string_view sql, Mapper mapper, const Args &...args) {
        sqlite3_stmt *stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw SqlDataBaseError(sqlite3_errmsg(db));
        }

        try {
            int idx = 1;
            (bind(stmt, idx++, args), ...);
            std::optional<T> result;
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int cols = sqlite3_column_count(stmt);
                std::vector<char *> values(cols);
                for (int i = 0; i < cols; ++i) {
                    values[i] = const_cast<char *>(
                        reinterpret_cast<const char *>(sqlite3_column_text(stmt, i)));
                }
                result = mapper(values.data());
            }
            sqlite3_finalize(stmt);
            return result;
        } catch (...) {
            sqlite3_finalize(stmt);
            throw;
        }
    }
};