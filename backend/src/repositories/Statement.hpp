#pragma once

#include <sqlite3.h>

#include "exceptions/Exceptions.hpp"

class Statement {
    sqlite3_stmt* stmt = nullptr;
    sqlite3* db = nullptr;

public:
    Statement(sqlite3* db, std::string_view sql) : db(db) {
        if (sqlite3_prepare_v2(db, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw SqlDataBaseError(sqlite3_errmsg(db));
        }
    }

    sqlite3_stmt* get() { return stmt; }

    ~Statement() {
        if (stmt) sqlite3_finalize(stmt);
    }

    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    Statement(Statement&& other) noexcept {
        stmt = other.stmt;
        db = other.db;
        other.stmt = nullptr;
    }

    Statement& operator=(Statement&& other) noexcept {
        if (this != &other) {
            if (stmt) sqlite3_finalize(stmt);
            stmt = other.stmt;
            db = other.db;
            other.stmt = nullptr;
        }
        return *this;
    }
};