class ServiceRecordDB {
    constructor() {
        this.logger = require('./logger');
        if (ServiceRecordDB.records == undefined) {
            this.logger.info('Created Service Record DB');
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

module.exports = new ServiceRecordDB();
