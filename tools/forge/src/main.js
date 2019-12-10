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
import HelloWorld from './components/HelloWorld.vue';
import Dashboard from './components/Dashboard.vue';

Vue.use(BootstrapVue);
Vue.use(VueRouter);
Vue.use(Vuex);

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
    count: 0,
    currentWorkspace: __dirname,
    project: {
        name: "Unnamed project"
    }
  },
  mutations: {
    increment (state) {
      state.count++
    }
  }
})

new Vue({
    router,
    store,
    render: h => h(App)
}).$mount('#app');
