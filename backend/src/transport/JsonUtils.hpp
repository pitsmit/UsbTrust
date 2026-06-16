#pragma once

#include <string>
#include <optional>

#include <nlohmann/json.hpp>

#include "entities/DeviceInfo.hpp"
#include "entities/Device.hpp"
#include "entities/MountRecord.hpp"
#include "entities/MountMode.hpp"

using json = nlohmann::json;

template <typename T>
std::optional<T> get_optional(const json& j, const std::string& key)
{
    if (j.contains(key) && !j.at(key).is_null())
        return j.at(key).get<T>();
    return std::nullopt;
}

template <typename T>
T get_value(const json& j, const std::string& key, const T& defaultValue)
{
    if (j.contains(key) && !j.at(key).is_null())
        return j.at(key).get<T>();
    return defaultValue;
}

template <typename T>
void set_optional(json& j, const std::string& key, const std::optional<T>& value)
{
    if (value.has_value())
        j[key] = *value;
    else
        j[key] = nullptr;
}

inline void to_json(json& j, const DeviceInfo& info)
{
    j = json::object();

    j["vendorId"] = info.vendorId;
    j["productId"] = info.productId;
    j["serial"] = info.serial;
    set_optional(j, "vendorName", info.vendorName);
    set_optional(j, "productName", info.productName);
}

inline void from_json(const json& j, DeviceInfo& info)
{
    info.vendorId = get_value<std::string>(j, "vendorId", "");
    info.productId = get_value<std::string>(j, "productId", "");
    info.serial = get_value<std::string>(j, "serial", "");
    info.vendorName = get_optional<std::string>(j, "vendorName");
    info.productName = get_optional<std::string>(j, "productName");
}

inline void to_json(json& j, const Device& d)
{
    j = json::object();

    j["id"] = d.id;
    j["info"] = d.info;
    set_optional(j, "validTo", d.validTo);
}

inline void from_json(const json& j, Device& d)
{
    d.id = get_value<int>(j, "id", 0);
    d.info = get_value<DeviceInfo>(j, "info", DeviceInfo{});
    d.validTo = get_optional<std::string>(j, "validTo");
}

inline void to_json(json& j, const MountRecord& m)
{
    j = json::object();

    set_optional(j, "id", m.id);
    j["devNode"] = m.devNode;
    j["mountPoint"] = m.mountPoint;
    j["info"] = m.info;
    j["mode"] = m.mode.toStringUpper();
}

inline void from_json(const json& j, MountRecord& m)
{
    m.id = get_value<size_t>(j, "id", 0);
    m.devNode = get_value<std::string>(j, "devNode", "");
    m.mountPoint = get_value<std::string>(j, "mountPoint", "");
    m.info = get_value<DeviceInfo>(j, "info", DeviceInfo{});
    m.mode = get_value<MountMode>(j, "mode", MountMode::ro());
}

inline void from_json(const json& j, MountMode& m)
{
    m = MountMode::parse(j.get<std::string>());
}