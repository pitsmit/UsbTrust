#include "MountRepository.hpp"

#include "entities/MountRecord/MountRecord.hpp"

void MountRepository::add(const MountRecord &record) {
    auto deviceInfoId = info_rep.ensure(record.info);

    static constexpr auto sql =
        "INSERT INTO MountRecord (deviceInfoId, devNode, mountPoint, mode) VALUES (?, ?, ?, ?);";

    executor.exec(sql, deviceInfoId, record.devNode, record.mountPoint, record.mode.toString());
}

void MountRepository::update(const MountRecord &record) {
    auto deviceInfoId = info_rep.ensure(record.info);

    static constexpr auto sql =
        "UPDATE MountRecord SET deviceInfoId = ?, mountPoint = ?, mode = ? WHERE devNode = ?;";

    executor.exec(sql, deviceInfoId, record.mountPoint, record.mode.toString(), record.devNode);
}

std::optional<MountRecord> MountRepository::getByDevice(core::Id id) {
    static constexpr auto sql =
        "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
        "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
        "FROM Device d "
        "JOIN MountRecord mr ON mr.deviceInfoId = d.deviceInfoId "
        "JOIN DeviceInfo di ON mr.deviceInfoId = di.id "
        "WHERE d.id = ? LIMIT 1;";

    auto record = executor.record<MountRecord>(sql, id);
    if (!record)
        return std::nullopt;
    return *record;
}

void MountRepository::removeByDevNode(const core::path &devNode) {
    static constexpr auto sql = "DELETE FROM MountRecord WHERE devNode = ?;";
    executor.exec(sql, devNode);
}

std::vector<MountRecord> MountRepository::getAll() {
    static constexpr auto sql =
        "SELECT d.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
        "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
        "FROM MountRecord mr "
        "JOIN DeviceInfo di ON mr.deviceInfoId = di.id "
        "LEFT JOIN Device d ON d.deviceInfoId = mr.deviceInfoId;";

    return executor.query<MountRecord>(sql);
}