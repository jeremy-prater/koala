<template>
  <div>
    <div class="card text-left">
      <div class="card-header text-">
        <div class="text-right" style="float:right;">
          <button
            type="button"
            class="btn btn-danger"
            @click="deleteGroup(group.uuid)"
            style="margin: 7px;"
          >Delete</button>
        </div>
        <div>
          <h5>Group {{ this.group.uuid }}</h5>
          <h5>Parent Asset [{{ parentObject.parser }}] {{ this.group.parentPath }}</h5>
        </div>
      </div>
      <div class="card-body">
        <div class="container"></div>
        <div
          class="row"
          v-if="parentObject.parser === 'gltf'"
          v-observe-visibility="visibilityChanged"
        >
          <h3>It's for sure GLTF</h3>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { mapState } from "vuex";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader";

const remote = require("electron").remote;
const fs = remote.require("fs");

export default {
  name: "GroupCard",
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
    deleteGroup(uuid) {
      this.$store.commit("deleteGroup", uuid);
    },
    loadAsset() {
      if (this.loaded === false) {
        this.loaded = true;
      } else {
        return;
      }

      console.log(`Group [${this.group.uuid}] Loading primary asset...`);
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
      console.log(`Group [${this.group.uuid}] Unoading primary asset...`);
    }
  }
};
</script>
