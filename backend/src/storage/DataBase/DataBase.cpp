#include "DataBase.hpp"

#include "exceptions/Exceptions.hpp"

DataBase::DataBase(core::path dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw SqlDataBaseError("Failed to open SQLite database");
    }
}

DataBase::~DataBase() {
    sqlite3_close(db);
}

std::string DataBase::what() const {
    return sqlite3_errmsg(db);
}

sqlite3_stmt *DataBase::prepareStatement(std::string_view sql) {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw SqlDataBaseError(what());
    }
    return stmt;
}