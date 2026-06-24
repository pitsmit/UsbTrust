#pragma once

#include <expected>
#include <format>
#include <vector>

#include "exceptions/Exceptions.hpp"
#include "storage/DataBase/DataBase.hpp"
#include "storage/Mapper/Mapper.hpp"
#include "storage/Statement/Statement.hpp"

class SqlExecutor {
    DataBase &db;

  public:
    explicit SqlExecutor(DataBase &db_) : db(db_) {}

    template <typename... Args> void exec(std::string_view sql, const Args &...args) {
        Statement stmt(db.prepareStatement(sql));
        stmt.bind(args...);

        stmt.eval();
        if (!stmt.done()) {
            throw SqlDataBaseError(db.what());
        }
    }

    template <typename T, typename... Args>
    std::expected<T, RecordNotFoundError> scalar(std::string_view sql, const Args &...args) {
        Statement stmt(db.prepareStatement(sql));
        stmt.bind(args...);

        stmt.eval();
        if (stmt.hasRow())
            return stmt.extractValueFromColumn<T>(0);
        if (stmt.done())
            return std::unexpected<RecordNotFoundError>(std::format("no rows for query: {}", sql));
        throw SqlDataBaseError(db.what());
    }

    template <typename T, typename... Args>
    std::vector<T> query(std::string_view sql, const Args &...args) {
        Statement stmt(db.prepareStatement(sql));
        stmt.bind(args...);

        std::vector<T> result;
        while (stmt.next()) {
            result.push_back(Mapper::from<T>(stmt));
        }
        if (!stmt.done())
            throw SqlDataBaseError(db.what());
        return result;
    }
};