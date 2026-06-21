#include "Statement.hpp"

#include "exceptions/Exceptions.hpp"

Statement::Statement(sqlite3 *db, std::string_view sql) : db(db) {
    if (sqlite3_prepare_v2(db, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw SqlDataBaseError(sqlite3_errmsg(db));
    }
}

sqlite3_stmt *Statement::get() const noexcept {
    return stmt;
}

Statement::~Statement() {
    sqlite3_finalize(stmt);
}

Statement::Statement(Statement &&other) noexcept {
    stmt = other.stmt;
    db = other.db;
    other.stmt = nullptr;
}

Statement &Statement::operator=(Statement &&other) noexcept {
    if (this != &other) {
        sqlite3_finalize(stmt);
        stmt = other.stmt;
        db = other.db;
        other.stmt = nullptr;
    }
    return *this;
}