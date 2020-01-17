class ServiceRecordDB {
    constructor() {
        this.logger = require('./logger');
        this.logger.info('Created Service Record DB');
        if (ServiceRecordDB.records == undefined) {
            ServiceRecordDB.records = [];
        }
    }

    AddServiceRecord(name, port) {
        ServiceRecordDB.records.push({
            name: name,
            port: port
        });
        this.logger.info(`Adding Service Record [${name}] on [${port}]`);
    }

    GetServiceRecords() {
        return ServiceRecordDB.records;
    }
}

module.exports = ServiceRecordDB;
