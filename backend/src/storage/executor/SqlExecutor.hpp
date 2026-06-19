#pragma once

#include <vector>

#include <sqlite3.h>

#include "storage/DataBase.hpp"

class SqlExecutor {
    DataBase &db;

    template <typename F> decltype(auto) withStatement(std::string_view sql, F &&f) const;

  public:
    explicit SqlExecutor(DataBase &db_) : db(db_) {}

    template <typename... Args> void exec(std::string_view sql, const Args &...args);

    template <typename T, typename... Args>
    std::optional<T> scalar(std::string_view sql, const Args &...args);

    template <typename T, typename... Args>
    std::vector<T> query(std::string_view sql, const Args &...args);
};