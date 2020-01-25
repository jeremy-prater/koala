<template>
  <div v-observe-visibility="visibilityChanged">
    <h3>It's for sure GLTF</h3>
  </div>
</template>

<script>

import { mapState } from "vuex";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader";

const remote = require("electron").remote;
const fs = remote.require("fs");

export default {
  name: "GroupCardGLTF",
  data() {
    return {
      loaded: false,
      gltf: {}
    };
  },
  props: ["group"],
  computed: mapState({
    // arrow functions can make the code very succinct!
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace,
    parentObject: function(state) {
      return state.project.objects.filter(
        object => object.path + "/" + object.name === this.group.parentPath
      )[0];
    }
  }),
  methods: {
    visibilityChanged(isVisible) {
      if (isVisible) {
        this.loadAsset();
      } else {
        this.unloadAsset();
      }
    },
    loadAsset() {
      if (this.loaded === false) {
        this.loaded = true;
      } else {
        return;
      }

      console.log(`Group [${this.group.uuid}] Loading GLTF asset...`);
      let loader = new GLTFLoader();
      let data = undefined;
      try {
        data = fs.readFileSync(
          this.currentWorkspace + "/" + this.parentObject.source
        );
      } catch (error) {
        console.error(
          `Error loading [${this.parentObject.path}/${this.parentObject.name}] at physical location [${this.parentObject.source}] [${error}]`
        );
        return;
      }
      loader.parse(
        data,
        undefined,
        function(gltf) {
          console.info(
            `Loaded [${this.parentObject.path}/${this.parentObject.name}]`
          );
          this.gltf = gltf;
          console.log(this.gltf);
        }.bind(this),
        function(error) {
          console.error(
            `Error loading [${this.parentObject.path}/${this.parentObject.name}]`
          );
          console.error(error);
        }.bind(this)
      );
    },
    unloadAsset() {
      this.loaded = false;
      this.gltf = undefined;
      console.log(`Group [${this.group.uuid}] Unoading GLTF asset...`);
    }
  }
};
</script>
