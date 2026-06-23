#include "MountRepository.hpp"

#include "entities/MountRecord.hpp"

void MountRepository::add(const MountRecord &record) {
    auto deviceInfoId = info_rep.ensure(record.info);

    static constexpr auto sql =
        "INSERT INTO MountRecord (deviceInfoId, devNode, mountPoint, mode) VALUES (?, ?, ?, ?);";

    executor.exec(
        sql, deviceInfoId, record.devNode, record.mountPoint, record.mode.toStringUpper());
}

void MountRepository::update(const MountRecord &record) {
    auto deviceInfoId = info_rep.ensure(record.info);

    static constexpr auto sql =
        "UPDATE MountRecord SET deviceInfoId = ?, mountPoint = ?, mode = ? WHERE devNode = ?;";

    executor.exec(
        sql, deviceInfoId, record.mountPoint, record.mode.toStringUpper(), record.devNode);
}

std::optional<core::path> MountRepository::getMountPointByDevNode(const core::path &devNode) {
    static constexpr auto sql = "SELECT mountPoint FROM MountRecord WHERE devNode = ? LIMIT 1;";

    return executor.scalar<core::path>(sql, devNode);
}

std::optional<MountRecord> MountRepository::getById(core::Id id) {
    static constexpr auto sql =
        "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
        "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
        "FROM Device d "
        "JOIN MountRecord mr ON mr.deviceInfoId = d.deviceInfoId "
        "JOIN DeviceInfo di ON mr.deviceInfoId = di.id "
        "WHERE d.id = ? LIMIT 1;";

    return executor.query<MountRecord>(sql, id).front();
}

void MountRepository::removeByDevNode(const core::path &devNode) {
    static constexpr auto sql = "DELETE FROM MountRecord WHERE devNode = ?;";
    executor.exec(sql, devNode);
}

std::vector<MountRecord> MountRepository::getAll() {
    static constexpr auto sql =
        "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
        "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
        "FROM MountRecord mr "
        "JOIN DeviceInfo di ON mr.deviceInfoId = di.id;";

    return executor.query<MountRecord>(sql);
}