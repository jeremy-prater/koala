import Vue from 'vue';
import VueRouter from 'vue-router';
import Vuex from 'vuex';

import App from './App.vue';

Vue.config.productionTip = false;

import BootstrapVue from 'bootstrap-vue';
import 'bootstrap/dist/css/bootstrap.css';
import 'bootstrap-vue/dist/bootstrap-vue.css';

import '@fortawesome/fontawesome-free/css/all.css';

import Home from './components/Home.vue';
import Objects from './components/Objects.vue';
import Dashboard from './components/Dashboard.vue';
import KoalaSettings from './components/settings.js';

const remote = require('electron').remote;
const fs = remote.require('fs');
const path = require('path');

Vue.use(BootstrapVue);
Vue.use(VueRouter);
Vue.use(Vuex);

const router = new VueRouter({
    mode: 'history',
    base: __dirname,
    routes: [
        { path: '/', component: Home },
        { path: '/dashboard', component: Dashboard },
        { path: '/objects', component: Objects }
    ]
});

const defaultState = function() {
    return {
        name: 'Unnamed project',
        objects: []
    };
};

const store = new Vuex.Store({
    state: {
        currentWorkspace: KoalaSettings.settings.workspace,
        project: defaultState()
    },
    mutations: {
        addObject(state, object) {
            console.log(`Created Object [${object.name}]`);
            state.project.objects.push(object);
        },
        deleteObject(state, uuid) {
            console.log(`Deleting Object [${uuid}]`);
            state.project.objects = state.project.objects.filter(
                object => object.uuid != uuid
            );
        },
        setWorkspace(state, newWorkspace) {
            KoalaSettings.settings.workspace = newWorkspace;
            state.currentWorkspace = newWorkspace;
            KoalaSettings.saveSettings();
        },
        loadProject(state) {
            const inputSettings = path.join(
                KoalaSettings.settings.workspace,
                'projectConfig.json'
            );
            state.project = defaultState();
            if (fs.existsSync(inputSettings)) {
                console.log(`Loading Project [${inputSettings}]`);
                state.project = {
                    ...state.project,
                    ...JSON.parse(fs.readFileSync(inputSettings))
                };
                console.log(
                    `Loaded [${state.project.objects.length}] objects!`
                );
            } else {
                console.warn(
                    `Project configuration [${inputSettings}] does not exist! Using defaults!`
                );
            }
        }
    }
});

new Vue({
    router,
    store,
    render: h => h(App)
}).$mount('#app');
