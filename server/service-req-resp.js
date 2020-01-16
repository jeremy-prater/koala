const zmq = require('zeromq');

class ServiceRequestResponse {
    constructor(name, port) {
        this.logger = require('./logger');
        this.port = port;
        this.name = name;
        this.running = true;
        this.url = `tcp://127.0.0.1:${this.port}`;
        this.socket = new zmq.Reply();
        this.socket.events.on('accept', event => {
            this.logger.info(`New Connection from [${event.address}]`);
        });
        this.logger.info(
            `Created RequestResponse Service [${this.name}] on port [${this.url}]`
        );
    }

    handler(msg) {
        this.logger.warn(`[${this.name}] Response not implemented... [${msg}]`);
    }

    async start() {
        await this.socket.bind(this.url);

        while (this.running) {
            const [msg] = await this.socket.receive();
            this.handler(msg);
        }
    }
}

module.exports = ServiceRequestResponse;
