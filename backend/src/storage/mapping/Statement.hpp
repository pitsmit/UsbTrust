#pragma once

#include <string_view>

#include <sqlite3.h>

class Statement {
    sqlite3_stmt *stmt = nullptr;
    sqlite3 *db = nullptr;

  public:
    Statement(sqlite3 *db, std::string_view sql);
    ~Statement();
    Statement(const Statement &) = delete;
    Statement &operator=(const Statement &) = delete;

    Statement(Statement &&other) noexcept;
    Statement &operator=(Statement &&other) noexcept;
    sqlite3_stmt *get() const noexcept;
};