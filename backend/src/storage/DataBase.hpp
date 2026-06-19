#pragma once

#include <sqlite3.h>

#include "exceptions/Exceptions.hpp"

class DataBase {
    sqlite3 *db = nullptr;

  public:
    explicit DataBase(std::string_view dbPath) {
        if (sqlite3_open(dbPath.data(), &db) != SQLITE_OK) {
            throw SqlDataBaseError("Failed to open SQLite database");
        }
    }

    ~DataBase() {
        sqlite3_close(db);
    }

    sqlite3 *get() {
        return db;
    }

    DataBase(const DataBase &) = delete;
    DataBase &operator=(const DataBase &) = delete;
};