<template>
  <div>
    <modal v-show="pickerVisible" @close="cancelPick">
      <template v-slot:title>Pick Object</template>
      <template v-slot:body>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="form-control input-group-text">Object Filter</span>
          </div>
          <input type="text" class="form-control" v-model="objectFilter" />
          <button class="btn btn-secondary" type="button">Browser</button>
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-secondary" @click="cancelPick">Cancel</button>
      </template>
    </modal>
  </div>
</template>

<script>
console.log("Creating Groups Editor");

// in full builds helpers are exposed as Vuex.mapState
import { mapState } from "vuex";
import Modal from "./Modal";

export default {
  name: "ObjectPicker",
  data() {
    return {
      objectFilter: "",
      pickerVisible: true
    };
  },
  components: { Modal },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace
  }),
  methods: {
    showPicker() {
      this.pickerVisible = true;
    },
    cancelPicker() {
      this.pickerVisible = false;
    },
    completePicker() {
      this.pickerVisible = false;
    }
  }
};
</script>

<style scoped>
</style>
