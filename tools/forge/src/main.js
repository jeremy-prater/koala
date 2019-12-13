import Vue from 'vue';
import VueRouter from 'vue-router';
import Vuex from 'vuex';

import App from './App.vue';

Vue.config.productionTip = false;

import BootstrapVue from 'bootstrap-vue';
import 'bootstrap/dist/css/bootstrap.css';
import 'bootstrap-vue/dist/bootstrap-vue.css';

import '@fortawesome/fontawesome-free/css/all.css';
import VueDragDrop from 'vue-drag-drop';

import Home from './components/Home.vue';
import HelloWorld from './components/HelloWorld.vue';
import Dashboard from './components/Dashboard.vue';
import KoalaSettings from './components/settings.js';

const remote = require('electron').remote;
const fs = remote.require('fs');
const path = require('path');

Vue.use(BootstrapVue);
Vue.use(VueRouter);
Vue.use(Vuex);
Vue.use(VueDragDrop);

const router = new VueRouter({
    mode: 'history',
    base: __dirname,
    routes: [
        { path: '/', component: Home },
        { path: '/dashboard', component: Dashboard },
        { path: '/objects', component: HelloWorld }
    ]
});

const store = new Vuex.Store({
    state: {
        currentWorkspace: KoalaSettings.settings.workspace,
        project: {
            name: 'Unnamed project'
        }
    },
    mutations: {
        setWorkspace(state, newWorkspace) {
            state.currentWorkspace = newWorkspace;
        },
        loadProject(state) {
            const inputSettings = path.join(
                state.currentWorkspace,
                'projectConfig.json'
            );
            if (fs.existsSync(inputSettings)) {
                console.log(`Loading Project [${inputSettings}]`);
                state.project = JSON.parse(fs.readFileSync(inputSettings));
            } else {
                console.warn(
                    `Project configuration [${inputSettings}] does not exist!`
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
