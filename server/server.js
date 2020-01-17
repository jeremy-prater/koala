const logger = require("./logger");
const ServiceRecordService = require('./service-records');

let service = new ServiceRecordService();

service.start();

logger.info("All services running...");