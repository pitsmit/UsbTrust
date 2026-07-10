#pragma once

#include "storage/DBInitializer/DBInitializer.hpp"
#include "storage/DataBase/DataBase.hpp"
#include "storage/SqlExecutor/SqlExecutor.hpp"

class DataBaseProvider {
    DataBase db;
    SqlExecutor exec;

  public:
    explicit DataBaseProvider() : db(":memory:"), exec(db) {
        DBInitializer::init(exec);
    }

    SqlExecutor &get_executor() {
        return exec;
    }
};