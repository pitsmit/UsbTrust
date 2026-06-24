#include "Statement.hpp"

Statement::~Statement() {
    sqlite3_finalize(stmt);
}

sqlite3_stmt *Statement::get() const noexcept {
    return stmt;
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