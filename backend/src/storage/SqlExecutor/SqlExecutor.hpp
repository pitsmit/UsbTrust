#pragma once

#include <vector>

#include <sqlite3.h>

#include "exceptions/Exceptions.hpp"
#include "storage/Bind.hpp"
#include "storage/DataBase.hpp"
#include "storage/Mapper/Mapper.hpp"
#include "storage/Row/Row.hpp"
#include "storage/Statement/Statement.hpp"

class SqlExecutor {
    DataBase &db;

    template <typename F> decltype(auto) withStatement(std::string_view sql, F &&f) const {
        return std::forward<F>(f)(Statement(db.get(), sql).get());
    }

  public:
    explicit SqlExecutor(DataBase &db_) : db(db_) {}

    template <typename... Args> void exec(std::string_view sql, const Args &...args) {
        withStatement(sql, [&](sqlite3_stmt *stmt) {
            db::bind::bind_all(stmt, args...);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw SqlDataBaseError(sqlite3_errmsg(db.get()));
            }
        });
    }

    template <typename T, typename... Args>
    std::optional<T> scalar(std::string_view sql, const Args &...args) {
        return withStatement(sql, [&](sqlite3_stmt *stmt) -> std::optional<T> {
            db::bind::bind_all(stmt, args...);
            if (sqlite3_step(stmt) != SQLITE_ROW)
                return std::nullopt;
            return Row::extractValue<T>(stmt, 0);
        });
    }

    template <typename T, typename... Args>
    std::vector<T> query(std::string_view sql, const Args &...args) {
        std::vector<T> result;
        withStatement(sql, [&](sqlite3_stmt *stmt) {
            db::bind::bind_all(stmt, args...);
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                result.push_back(Mapper::from<T>(Row(stmt)));
            }
        });
        return result;
    }
};