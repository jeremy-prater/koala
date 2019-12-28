<template>
  <canvas v-bind:ref="this.object.uuid" @mousemove="render" style="width: 100%; height:100%; background-color:#333" />
</template>

<script>
console.log("Creating Viewer glTF");

import * as THREE from "three";

export default {
  name: "ViewerglTF",
  data() {
    return {
      localObject: this.object,
      renderer: {}
    };
  },
  props: ["object"],
  components: {},
  computed: {},
  methods: {
      render: function() {
        console.log("Render");
        this.renderer.renderer.render(this.renderer.scene, this.renderer.camera);
      }
  },
  mounted: function() {
    console.log(`Creating Viewer-glTF for [${this.object.uuid}]`);

    this.renderer.scene = new THREE.Scene();
    this.renderer.camera = new THREE.PerspectiveCamera(
      75,
      this.$refs[this.object.uuid].innerWidth /
        this.$refs[this.object.uuid].innerHeight,
      0.1,
      1000
    );

    this.renderer.renderer = new THREE.WebGLRenderer({
        canvas: this.$refs[this.object.uuid]
    });
    this.renderer.renderer.setSize(
      this.$refs[this.object.uuid].innerWidth,
      this.$refs[this.object.uuid].innerHeight
    );

    this.renderer.geometry = new THREE.BoxGeometry(1, 1, 1);
    this.renderer.material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
    this.renderer.cube = new THREE.Mesh(
      this.renderer.geometry,
      this.renderer.material
    );
    this.renderer.scene.add(this.renderer.cube);

    this.renderer.camera.position.z = 5;
  },
};
</script>

<style scoped>
</style>
