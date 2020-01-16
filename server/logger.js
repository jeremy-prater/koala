const pino = require('pino');
const logger = pino({
    prettyPrint: {
        levelFirst: true
    },
    prettifier: require('pino-pretty')
});

module.exports = logger;
