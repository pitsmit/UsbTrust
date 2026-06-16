#pragma once

#include <vector>
#include <string>
#include <optional>

#include "entities/MountRecord.hpp"
#include "RepositoryBase.hpp"
#include "DeviceInfoRepository.hpp"
#include "mappers/MountRecordMapper.hpp"

class MountRepository : public RepositoryBase {
private:
    DeviceInfoRepository info_rep;

public:
    explicit MountRepository(
        DBConnection& connection)
        : RepositoryBase(connection),
        info_rep(connection)
    {}

    void add(const MountRecord& record) {
        auto deviceInfoId = info_rep.ensure(record.info);

        static constexpr auto sql =
            "INSERT INTO MountRecord (deviceInfoId, devNode, mountPoint, mode) VALUES (?, ?, ?, ?);";

        db.execute(sql, 
            deviceInfoId,
            record.devNode,
            record.mountPoint,
            record.mode.toStringUpper()
        );
    }

    void update(const MountRecord& record) {
        auto deviceInfoId = info_rep.ensure(record.info);

        static constexpr auto sql =
            "UPDATE MountRecord SET deviceInfoId = ?, mountPoint = ?, mode = ? WHERE devNode = ?;";

        db.execute(sql,
            deviceInfoId,
            record.mountPoint,
            record.mode.toStringUpper(),
            record.devNode
        );
    }

    std::optional<std::string> 
    getMountPointByDevNode(std::string_view devNode) {
        static constexpr auto sql =
            "SELECT mountPoint FROM MountRecord WHERE devNode = ? LIMIT 1;";

        return db.queryScalar<std::string>(sql, devNode);
    }

    std::optional<MountRecord> getById(core::Id id) {
        static constexpr auto sql =
            "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
            "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
            "FROM Device d "
            "JOIN MountRecord mr ON mr.deviceInfoId = d.deviceInfoId "
            "JOIN DeviceInfo di ON mr.deviceInfoId = di.id "
            "WHERE d.id = ? LIMIT 1;";

        return db.queryOne<MountRecord>(
            sql,
            MountRecordMapper::fromRow,
            id
        );
    }

    void removeByDevNode(std::string_view devNode) {
        static constexpr auto sql =
            "DELETE FROM MountRecord WHERE devNode = ?;";
        db.execute(sql, devNode);
    }

    std::vector<MountRecord> getAll() {
        static constexpr auto sql =
            "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
            "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
            "FROM MountRecord mr "
            "JOIN DeviceInfo di ON mr.deviceInfoId = di.id;";

        return db.queryAll<MountRecord>(
            sql,
            MountRecordMapper::fromRow
        );
    }
};