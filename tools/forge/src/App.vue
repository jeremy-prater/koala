<template>
  <div id="app">
    <NavBar />
    <NavMenu />
  </div>
</template>

<script>
import Vue from "vue";
import BootstrapVue from "bootstrap-vue";
import NavBar from "./components/NavBar.vue";
import NavMenu from "./components/NavMenu.vue";

import "bootstrap/dist/css/bootstrap.css";
import "bootstrap-vue/dist/bootstrap-vue.css";

import "@fortawesome/fontawesome-free/css/all.css";

Vue.use(BootstrapVue);

export default {
  name: "app",
  components: {
    NavBar,
    NavMenu
  }
};
</script>

<style>
#app {
  font-family: "Avenir", Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}
</style>
