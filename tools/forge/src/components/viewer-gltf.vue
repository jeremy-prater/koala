<template>
  <canvas v-bind:ref="this.object.uuid" style="width: 100%; height:100%; background-color:#333" />
</template>

<script>
console.log("Creating Viewer glTF");

import * as THREE from "three";
// ES modules
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader";

const remote = require("electron").remote;
const fs = remote.require("fs");

export default {
  name: "ViewerglTF",
  data() {
    return {
      localObject: this.object,
      render: {}
    };
  },
  props: ["object"],
  components: {},
  computed: {},
  methods: {
    remapMaterials: function(parent) {
      if (parent.hasOwnProperty("material")) {
        parent.material = new THREE.MeshBasicMaterial({
          color: 0x00ff00,
          wireframe: true
        });
      }
      // Process all children
      parent.children.forEach(child => {
        this.remapMaterials(child);
      });
    },
    doRender: function() {
      requestAnimationFrame(this.doRender);

      this.render.renderer.render(this.render.scene, this.render.camera);

      let object = this.render.scene.children[0];
      if (object !== undefined) {
        object.rotation.x += 0.01;
        object.rotation.y += 0.01;
        object.rotation.z += 0.01;
      }
    }
  },
  mounted: function() {
    let domObject = this.$refs[this.object.uuid];
    this.render.width = domObject.clientWidth;
    this.render.height = domObject.clientHeight;

    console.debug(
      `Creating Viewer-glTF for [${this.object.uuid}]` //  @ [${this.render.width}x${this.render.height}] from source [${this.object.source}]
    );

    this.render.scene = new THREE.Scene();
    this.render.camera = new THREE.PerspectiveCamera(
      75,
      this.render.width / this.render.height,
      0.1,
      1000
    );

    this.render.renderer = new THREE.WebGLRenderer({
      canvas: this.$refs[this.object.uuid]
    });
    this.render.renderer.setSize(this.render.width, this.render.height);

    let loader = new GLTFLoader();
    let data = undefined;
    try {
      data = fs.readFileSync(this.object.source);
    } catch (error) {
      console.error(
        `Error loading [${this.object.path}/${this.object.name}] at physical location [${this.object.source}] [${error}]`
      );
      return;
    }
    loader.parse(
      data,
      undefined,
      function(gltf) {
        console.info(`Loaded [${this.object.path}/${this.object.name}]`);
        this.remapMaterials(gltf.scene);

        this.render.scene.add(gltf.scene);
      }.bind(this),
      function(error) {
        console.error(
          `Error loading [${this.object.path}/${this.object.name}]`
        );
        console.error(error);
      }.bind(this)
    );

    this.render.camera.position.z = 5;

    requestAnimationFrame(this.doRender);
  }
};
</script>

<style scoped>
</style>
