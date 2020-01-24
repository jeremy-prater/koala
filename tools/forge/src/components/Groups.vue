<template>
  <div>
    <modal v-show="addingGroup" @close="cancelGroupAdd">
      <template v-slot:title>Add New Group</template>
      <template v-slot:body>
        <div class="container">
          <div class="row no-gutters">
            <div class="col-3">
              <span class="form-control input-group-text">Parent Object</span>
            </div>
            <div class="col">
              <Autocomplete :suggestions="getObjectList()" :selection.sync="selectedParentObject" />
            </div>
          </div>
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
import Autocomplete from "./Autocomplete";

export default {
  name: "Groups",
  data() {
    return {
      selectedParentObject: "",
      addingGroup: false
    };
  },
  components: { Modal, Autocomplete },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace
  }),
  methods: {
    getObjectList() {
      let paths = [];
      this.project.objects.forEach(object =>
        paths.push(object.path + "/" + object.name)
      );
      return paths;
    },
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
