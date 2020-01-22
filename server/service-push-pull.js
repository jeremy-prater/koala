const zmq = require('zeromq');
const ServiceRecordsDB = require('./service-records-db');

class ServicePushPull {
    constructor(name, port) {
        this.logger = require('./logger');
        this.port = port;
        this.name = name;
        this.running = true;
        this.url = `tcp://127.0.0.1:${this.port}`;
        this.socket = new zmq.Pull();
        this.socket.events.on('accept', event => {
            this.logger.info(`[${this.name}] --> Accept [${event.address}]`);
        });
        this.socket.events.on('disconnect', event => {
            this.logger.info(`[${this.name}] --> Disconnect [${event.address}]`);
        });
        this.logger.info(
            `[${this.name}] --> Created [${this.name}] on port [${this.url}]`
        );
        ServiceRecordsDB.AddServiceRecord(name, port);
    }

    handler(msg) {
        this.logger.warn(
            `[${this.name}] --> Response not implemented... [${msg}]`
        );
    }

    async start() {
        this.logger.info(
            `[${this.name}] --> Worker ready to accept connections...`
        );
        await this.socket.bind(this.url);

        while (this.running) {
            // this.logger.info(`[${this.name}] --> Wait for data...`);
            const [msg] = await this.socket.receive();
            // this.logger.info(`[${this.name}] --> Data in!`);
            this.handler(msg);
        }

        this.logger.info(`[${this.name}] --> Exit worker loop`);
    }
}

module.exports = ServicePushPull;
