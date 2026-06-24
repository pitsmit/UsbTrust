#pragma once

#include <sqlite3.h>

#include "types/types.hpp"

class DataBase {
    sqlite3 *db = nullptr;

  public:
    explicit DataBase(core::path dbPath);
    ~DataBase();

    DataBase(const DataBase &) = delete;
    DataBase &operator=(const DataBase &) = delete;

    std::string what() const;
    sqlite3_stmt *prepareStatement(std::string_view sql);
};