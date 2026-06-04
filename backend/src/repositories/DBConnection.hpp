#pragma once

#include <string>
#include <sqlite3.h>
#include <stdexcept>
#include <functional>

#include "Exceptions.hpp"

class DBConnection {
private:
    sqlite3* db = nullptr;
    using QueryCallback = std::function<void(int, char**, char**)>;

    static void throwSQLiteError(char* errMsg) {
        std::string error = errMsg ? errMsg : "SQLite error";
        sqlite3_free(errMsg);
        throw SqlDataBaseError(error.c_str());
    }

public:
    explicit DBConnection(const std::string& dbPath) {
        if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
            throw SqlDataBaseError("Failed to open SQLite database");
        }
    }

    ~DBConnection() {
        sqlite3_close(db);
    }

    DBConnection(const DBConnection&) = delete;
    DBConnection& operator=(const DBConnection&) = delete;

    void execute(std::string_view sql) {
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.data(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            throwSQLiteError(errMsg);
        }   
    }

    void query(std::string_view sql, QueryCallback callback) {
        auto trampoline = [](void* data, int cols, char** values, char** names) -> int {
            auto* cb = static_cast<QueryCallback*>(data);
            (*cb)(cols, values, names);
            return 0;
        };

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.data(), trampoline, &callback, &errMsg) != SQLITE_OK) {
            throwSQLiteError(errMsg);
        }
    }

    int lastInsertId() {
        return static_cast<int>(sqlite3_last_insert_rowid(db));
    }

    template<typename T>
    std::optional<T> queryScalar(const std::string& sql) {
        std::optional<T> result;
        query(sql,
            [&](int, char** vls, char**) {
                if (!vls || !vls[0]) return;
                if constexpr (std::is_same_v<T, int>)
                    result = std::stoi(vls[0]);
                else if constexpr (std::is_same_v<T, std::string>)
                    result = vls[0];
            });
        return result;
    }

    template<typename T, typename Mapper>
    std::vector<T> queryAll(
        const std::string& sql,
        Mapper mapper)
    {
        std::vector<T> result;
        query(sql,
            [&](int, char** values, char**) {
                if (values)
                    result.push_back(mapper(values));
            });
        return result;
    }

    template<typename T, typename Mapper>
    std::optional<T> queryOne(
        const std::string& sql,
        Mapper mapper)
    {
        std::optional<T> result;
        query(sql,
            [&](int, char** values, char**) {
                if (values && !result)
                    result = mapper(values);
            });
        return result;
    }
};