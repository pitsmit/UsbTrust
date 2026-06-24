#pragma once

#include <sqlite3.h>

#include "storage/Bind.hpp"

class Statement {
    sqlite3_stmt *stmt = nullptr;
    int last_rc = SQLITE_OK;

  public:
    Statement(sqlite3_stmt *stmt_) : stmt(stmt_){};
    ~Statement();
    Statement(const Statement &) = delete;
    Statement &operator=(const Statement &) = delete;

    sqlite3_stmt *get() const noexcept;
    int eval();
    bool next();
    bool hasRow() const noexcept;
    bool done() const noexcept;

    template <typename... Args> void bind(Args &&...args) {
        Bind::bind_all(stmt, args...);
    }
};