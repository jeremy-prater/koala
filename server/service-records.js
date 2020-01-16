const ServiceRequestResponse = require('./service-req-resp');

class ServiceRecord extends ServiceRequestResponse {
    constructor() {
        super('ServiceRecords', 20100);
    }

    handler(message) {
        this.logger.info(message.toString());
    }
}

module.exports = ServiceRecord;
