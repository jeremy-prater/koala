<template>
  <div v-observe-visibility="visibilityChanged">
    <div class="input-group mb-3">
      <button
        type="button"
        class="btn btn-primary"
        @click="selectAll"
        style="margin-left: 7px; margin-right: 7px;"
      >Select All</button>
      <button
        type="button"
        class="btn btn-secondary"
        @click="selectVisible"
        style="margin-left: 7px; margin-right: 7px;"
      >Select Visible</button>
      <button
        type="button"
        class="btn btn-danger"
        @click="selectNone"
        style="margin-left: 7px; margin-right: 7px;"
      >Select None</button>

      <div class="input-group-prepend">
        <span class="form-control input-group-text">Node Filter</span>
      </div>
      <input type="text" class="form-control" v-model="nodeFilter" @input="updateNodeVisibility()" />
      <button
        type="button"
        class="btn btn-success"
        @click="assignSelected"
        style="margin-left: 7px; margin-right: 7px;"
      >Assign Selected</button>
    </div>
    <ul>
      <li v-for="node in visibleNodes" v-bind:key="node.uuid">
        <input type="checkbox" class="form-check-input" v-model="node.selected" />
        <span>Node: {{ node.name }}</span>
      </li>
    </ul>
  </div>
</template>

<script>
import { mapState } from "vuex";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader";

const remote = require("electron").remote;
const fs = remote.require("fs");
const uuidv1 = require("uuid/v1");

export default {
  name: "GroupCardGLTF",
  data() {
    return {
      loaded: false,
      nodes: [],
      nodeFilter: "",
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
    },
    visibleNodes() {
      let nodes = [];
      this.nodes.forEach(node => {
        if (node.filtered) nodes.push(node);
      });
      return nodes;
    }
  }),
  methods: {
    selectAll() {
      this.nodes.forEach(node => {
        node.selected = true;
      });
    },
    selectNone() {
      this.nodes.forEach(node => {
        node.selected = false;
      });
    },
    selectVisible() {
      this.nodes.forEach(node => {
        node.selected = node.filtered;
      });
    },
    assignSelected() {},
    updateNodeVisibility() {
      this.nodes.forEach(node => {
        if (this.nodeFilter) {
          node.filtered = node.name.includes(this.nodeFilter);
        } else {
          node.filtered = true;
        }
      });
    },
    getNodes() {
      if (this.gltf.scene === undefined) {
        return [];
      }
      this.nodes = [];
      this.gltf.scene.children.forEach(child => {
        this.nodes.push(...this.getChildNodes(child, ""));
      });

      this.updateNodeVisibility();

      this.nodes = this.nodes.sort();
    },
    getChildNodes(parent, path) {
      let nodes = [];
      parent.children.forEach(child => {
        nodes.push(...this.getChildNodes(child, path + parent.name + "/"));
      });
      nodes.push({
        name: path + parent.name,
        uuid: uuidv1(),
        selected: false,
        filtered: false
      });
      return nodes;
    },
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
          this.getNodes();
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
