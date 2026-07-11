#pragma once

#include <string>
#include <unordered_map>

#include <sqlite3.h>

class Statement {
    sqlite3_stmt *stmt = nullptr;
    int last_rc = SQLITE_OK;
    std::unordered_map<std::string, int> indexMap;

  public:
    Statement(sqlite3_stmt *stmt_);
    ~Statement();
    Statement(const Statement &) = delete;
    Statement &operator=(const Statement &) = delete;

    int eval();
    bool next();
    bool hasRow() const noexcept;
    bool done() const noexcept;

    template <typename T> T get(std::string_view col) const;
    template <typename T> constexpr T extractValueFromColumn(int i) const;
    template <typename... Args> void bind(Args &&...args);
};

#include <format>

#include "exceptions/Exceptions.hpp"
#include "storage/Binder/Binder.hpp"

template <typename T> T Statement::get(std::string_view col) const {
    auto it = indexMap.find(std::string(col));
    if (it == indexMap.end())
        throw SqlDataBaseError(std::format("Unknown column: {}", col));
    return extractValueFromColumn<T>(it->second);
}

template <typename> struct is_optional : std::false_type {};

template <typename U> struct is_optional<std::optional<U>> : std::true_type {};
template <typename T> using Value = typename T::value_type;

template <typename T> constexpr T Statement::extractValueFromColumn(int i) const {
    if constexpr (std::same_as<T, std::string> || std::same_as<T, core::path>) {
        const unsigned char *txt = sqlite3_column_text(stmt, i);
        return txt ? reinterpret_cast<const char *>(txt) : "";
    } else if constexpr (std::integral<T>) {
        return static_cast<T>(sqlite3_column_int64(stmt, i));
    } else if constexpr (is_optional<T>::value && std::integral<Value<T>>) {
        if (sqlite3_column_type(stmt, i) == SQLITE_NULL)
            return std::nullopt;

        return static_cast<Value<T>>(sqlite3_column_int64(stmt, i));
    } else {
        static_assert(sizeof(T) == 0, "Unsupported type for SQLite extraction");
    }
}

template <typename... Args> void Statement::bind(Args &&...args) {
    Binder::bind_all(stmt, args...);
}
