const ServiceRequestResponse = require('./service-req-resp');
const ServiceRecordsDB = require('./service-records-db');

class ServiceDebugger extends ServiceRequestResponse {
    constructor() {
        super('Debugger', 20101);
    }

    handler(message) {
        this.logger.info(message.toString());
        this.socket.send(
            JSON.stringify(ServiceRecordsDB.GetServiceRecords())
        );
    }
}

module.exports = ServiceDebugger;
