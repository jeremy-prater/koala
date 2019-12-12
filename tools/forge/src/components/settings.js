const remote = require('electron').remote;
const fs = remote.require('fs');
const path = require('path');
const os = require('os');

const configDir = path.join(os.homedir(), '.forge');
const configFile = path.join(configDir, 'forge-config.json');

class KoalaSettings {
    constructor() {
        if (!fs.existsSync(configDir)) {
            console.warn(
                `Configuration directory [${configDir}] does not exists!`
            );
            fs.mkdirSync(configDir);
        }

        if (!fs.existsSync(configFile)) {
            console.warn(`Configuration [${configFile}] does not exists!`);
            this.settings = {
                workspace: '/'
            };
        } else {
            console.info(`Loaded Configuration file [${configFile}]`);
            this.settings = JSON.parse(fs.readFileSync(configFile));
        }
    }

    saveSettings() {
        console.log(`Saving Settings to [${configFile}]`);
        const configString = JSON.stringify(this.settings);
        fs.writeFileSync(configFile, configString);
    }
}

export default new KoalaSettings();
