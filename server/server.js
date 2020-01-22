const logger = require('./logger');
const ServiceRecordService = require('./service-records');
const DebuggerService = require('./service-debugger');

let recordService = new ServiceRecordService();
let debugService = new DebuggerService();

recordService.start();
debugService.start();

logger.info('All services running...');
