<template>
  <div>
    <modal v-show="addingGroup" @close="cancelGroupAdd">
      <template v-slot:title>Add New Group</template>
      <template v-slot:body>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="form-control input-group-text">Parent Object</span>
          </div>
          <input type="text" class="form-control" v-model="newGroupdata.parentPath" />
          <button class="btn btn-secondary" @click="showObjectBrowser" type="button">Browser</button>
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-secondary" @click="cancelGroupAdd">Cancel</button>
        <button type="button" class="btn btn-primary" @click="completeGroupAdd">Add Group</button>
      </template>
    </modal>

    <button
      type="button"
      class="btn btn-primary"
      @click="showGroupAdd"
      style="margin: 7px;"
    >Add Group</button>
  </div>
</template>

<script>
console.log("Creating Groups Editor");

// in full builds helpers are exposed as Vuex.mapState
import { mapState } from "vuex";
import Modal from "./Modal";

export default {
  name: "Groups",
  data() {
    return {
      newGroupdata: {},
      addingGroup: false
    };
  },
  components: { Modal },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace
  }),
  methods: {
    showGroupAdd() {
      this.addingGroup = true;
    },
    cancelGroupAdd() {
      this.addingGroup = false;
    },
    completeGroupAdd() {
      this.addingGroup = false;
    }
  }
};
</script>

<style scoped>
</style>
