const ServiceRequestResponse = require('./service-req-resp');
const ServiceRecordsDB = require('./service-records-db');

class ServiceRecord extends ServiceRequestResponse {
    constructor() {
        super('ServiceRecords', 20100);
    }

    handler(message) {
        this.logger.info(message.toString());
        this.socket.send(
            JSON.stringify(ServiceRecordsDB.GetServiceRecords())
        );
    }
}

module.exports = ServiceRecord;
