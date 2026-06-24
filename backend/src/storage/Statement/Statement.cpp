#include "Statement.hpp"

Statement::Statement(sqlite3_stmt *stmt_) : stmt(stmt_) {
    auto cols = sqlite3_column_count(stmt);
    for (auto i = 0; i < cols; ++i) {
        auto name = sqlite3_column_name(stmt, i);
        if (name)
            indexMap[name] = i;
    }
};

Statement::~Statement() {
    sqlite3_finalize(stmt);
}

int Statement::eval() {
    return last_rc = sqlite3_step(stmt);
}

bool Statement::next() {
    return eval() == SQLITE_ROW;
}

bool Statement::hasRow() const noexcept {
    return last_rc == SQLITE_ROW;
}

bool Statement::done() const noexcept {
    return last_rc == SQLITE_DONE;
}