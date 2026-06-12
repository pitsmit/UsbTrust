#pragma once

#include "DBConnection.hpp"

class RepositoryBase {
protected:   
    DBConnection& db;

    explicit RepositoryBase(DBConnection& connection)
        : db(connection) {}
};