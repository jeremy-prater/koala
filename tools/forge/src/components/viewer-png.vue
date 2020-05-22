<template>
  <img
    v-bind:ref="this.object.uuid"
    style="width: 100%; height:100%; background-color:#333"
    v-observe-visibility="visibilityChanged"
  />
</template>

<script>
console.log("Creating Viewer PNG");

// ES modules
import { mapState } from "vuex";
import { Base64 } from "js-base64";

const remote = require("electron").remote;
const fs = remote.require("fs");

export default {
  name: "ViewerPNG",
  data() {
    return {
      localObject: this.object,
      render: {},
      loaded: false
    };
  },
  props: ["object"],
  components: {},
  computed: mapState({
    currentWorkspace: state => state.currentWorkspace
  }),
  methods: {
    visibilityChanged(isVisible) {
      if (isVisible === true) {
        this.load();
      } else {
        this.unload();
      }
    },
    load() {
      if (this.loaded === false) {
        this.loaded = true;
        console.info(`Loading [${this.object.path}/${this.object.name}]`);
      } else {
        return;
      }

      let data = undefined;
      try {
        data = fs.readFileSync(
          this.currentWorkspace + "/" + this.object.source
        );
      } catch (error) {
        console.error(
          `Error loading [${this.object.path}/${this.object.name}] at physical location [${this.object.source}] [${error}]`
        );
        return;
      }

      let domObject = this.$refs[this.object.uuid];
      domObject.src = "data:image/png;base64," + Base64.encode(data);
    },
    unload() {
      this.loaded = false;
      this.render = {};
      console.info(`Unloaded [${this.object.path}/${this.object.name}]`);
      let domObject = this.$refs[this.object.uuid];
      domObject.src = "";
    }
  }
};
</script>

<style scoped>
</style>
