#include "DeviceRepository.hpp"

#include "entities/DeviceInfo.hpp"

core::Id DeviceRepository::add(const DeviceInfo &dev) {
    auto deviceInfoId = info_rep.ensure(dev);
    static constexpr auto sql =
        "INSERT INTO Device (deviceInfoId, validTo) VALUES (?, NULL) RETURNING id;";
    auto id = executor.scalar<core::Id>(sql, deviceInfoId);
    if (!id)
        throw id.error();
    return *id;
}

std::vector<Device> DeviceRepository::getAll() {
    static constexpr auto sql = "SELECT d.id, di.vendorId, di.productId, di.serial, "
                                "di.productName, di.vendorName, d.validTo "
                                "FROM Device d "
                                "JOIN DeviceInfo di ON d.deviceInfoId = di.id;";

    return executor.query<Device>(sql);
}

void DeviceRepository::remove(core::Id id) {
    static constexpr auto sql = "DELETE FROM Device WHERE id = ?";
    executor.exec(sql, id);
}

void DeviceRepository::updateValidTo(core::Id id, std::optional<std::string> validTo) {
    static constexpr auto sql = "UPDATE Device SET validTo = ? WHERE id = ?";
    executor.exec(sql, validTo, id);
}

std::optional<core::Id> DeviceRepository::findActiveId(const DeviceInfo &info) {
    static constexpr auto sql = "SELECT d.id FROM Device d "
                                "JOIN DeviceInfo di ON d.deviceInfoId = di.id "
                                "WHERE di.vendorId = ? AND "
                                "di.productId = ? AND "
                                "di.serial = ?"
                                " AND (d.validTo IS NULL "
                                "OR d.validTo >= date('now'))"
                                " LIMIT 1;";

    auto id = executor.scalar<core::Id>(sql, info.vendorId, info.productId, info.serial);
    if (!id)
        return std::nullopt;
    return *id;
}