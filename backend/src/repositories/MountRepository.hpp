#pragma once

#include "Device.hpp"
#include "DeviceInfo.hpp"
#include "MountRecord.hpp"
#include "DevLogger.hpp"
#include "RepositoryBase.hpp"
#include "DeviceInfoRepository.hpp"

#include <vector>
#include <string>
#include <optional>

class MountRepository : RepositoryBase {
private:
    DeviceInfoRepository& info_rep;

    static std::string modeToStr(MODE m) {
        return m == MODE::RW ? "RW" : "RO";
    }

    static MODE parseMode(const char* v) {
        return std::string(v) == "RW" ? MODE::RW : MODE::RO;
    }

    static DeviceInfo mapDeviceInfo(char** v,
    int vendorIdIdx,
    int productIdIdx,
    int serialIdx,
    int vendorNameIdx,
    int productNameIdx) {
        DeviceInfoBuilder builder;
        if (v[vendorIdIdx]) builder.withVendorId(v[vendorIdIdx]);
        if (v[productIdIdx]) builder.withProductId(v[productIdIdx]);
        if (v[serialIdx]) builder.withSerial(v[serialIdx]);
        if (v[vendorNameIdx]) builder.withVendorName(v[vendorNameIdx]);
        if (v[productNameIdx]) builder.withProductName(v[productNameIdx]);
        return builder.build();
    }

    static MountRecord mapMountRecord(char** v) {
        return MountRecordBuilder()
            .withId(std::stoull(v[0]))
            .withDevNode(v[2] ? v[2] : "")
            .withMountPoint(v[3] ? v[3] : "")
            .withMode(parseMode(v[4]))
            .withInfo(
                mapDeviceInfo(v, 5, 6, 7, 8, 9)
            ).build();
    }
public:
    explicit MountRepository(
        DBConnection& connection,
        DeviceInfoRepository& infoRepo)
        : RepositoryBase(connection),
        info_rep(infoRepo)
    {}

    void add(const MountRecord& record) {
        int deviceInfoId = info_rep.ensure(record.info);

        static constexpr auto sql =
            "INSERT INTO MountRecord (deviceInfoId, devNode, mountPoint, mode) VALUES (?, ?, ?, ?);";

        db.execute(sql, 
            deviceInfoId,
            record.devNode,
            record.mountPoint,
            modeToStr(record.mode)
        );
    }

    void update(const MountRecord& record) {
        static constexpr auto findSql =
            "SELECT id FROM MountRecord WHERE devNode = ? LIMIT 1;";

        auto mountId = db.queryScalar<int>(findSql, record.devNode);
        if (!*mountId) return;

        int deviceInfoId = info_rep.ensure(record.info);

        static constexpr auto sql =
            "UPDATE MountRecord SET deviceInfoId = ?, mountPoint = ?, mode = ? WHERE devNode = ?;";

        db.execute(sql,
            deviceInfoId,
            record.mountPoint,
            modeToStr(record.mode),
            record.devNode
        );
    }

    std::optional<std::string> 
    getMountPointByDevNode(const std::string& devNode) {
        static constexpr auto sql =
            "SELECT mountPoint FROM MountRecord WHERE devNode = ? LIMIT 1;";

        return db.queryScalar<std::string>(sql, devNode);
    }

    std::optional<MountRecord> getById(size_t id) {
        std::string sql =
            "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
            "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
            "FROM Device d "
            "JOIN MountRecord mr ON mr.deviceInfoId = d.deviceInfoId "
            "JOIN DeviceInfo di ON mr.deviceInfoId = di.id "
            "WHERE d.id = " + std::to_string(id) + " LIMIT 1;";

        return db.queryOne<MountRecord>(
            sql,
            mapMountRecord
        );
    }

    void removeByDevNode(const std::string& devNode) {
        db.execute("DELETE FROM MountRecord WHERE devNode = ?;", devNode);
    }

    std::vector<MountRecord> getAll() {
        static constexpr auto SQL =
            "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
            "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
            "FROM MountRecord mr "
            "JOIN DeviceInfo di ON mr.deviceInfoId = di.id;";

        return db.queryAll<MountRecord>(
            SQL,
            mapMountRecord
        );
    }
};