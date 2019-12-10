import Vue from 'vue';
import VueRouter from 'vue-router';

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

const router = new VueRouter({
    mode: 'history',
    base: __dirname,
    routes: [
        { path: '/', component: Home },
        { path: '/dashboard', component: Dashboard },
        { path: '/objects', component: HelloWorld }
    ]
});

new Vue({
    router,
    render: h => h(App)
}).$mount('#app');
