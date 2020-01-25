<template>
  <div style="width:100%;">
    <modal v-show="addingGroup" @close="cancelGroupAdd">
      <template v-slot:title>Add New Group</template>
      <template v-slot:body>
        <div class="container">
          <div class="row no-gutters">
            <div class="col-3">
              <span class="form-control input-group-text">Parent Object</span>
            </div>
            <div class="col">
              <Autocomplete
                ref="objectPicker"
                :suggestions="getObjectList()"
                @updated="objectPickerUpdated"
              />
            </div>
          </div>
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-secondary" @click="cancelGroupAdd">Cancel</button>
        <button type="button" class="btn btn-primary" @click="completeGroupAdd">Add Group</button>
      </template>
    </modal>

    <div class="text-left">
      <button
        type="button"
        class="btn btn-primary"
        @click="showGroupAdd"
        style="margin: 7px;"
      >Add Group</button>
    </div>

    <div class="card text-left" v-for="group in project.groups" v-bind:key="group.parentPath">
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
          <h5>Group {{ group.uuid }}</h5>
          <h5>Parent Asset {{ group.parentPath }}</h5>
        </div>
      </div>
      <div class="card-body">
        <div class="container"></div>
      </div>
    </div>
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
    objectPickerUpdated(parent) {
      this.selectedParentObject = parent;
    },
    getObjectList() {
      let paths = [];
      this.project.objects.forEach(object =>
        paths.push(object.path + "/" + object.name)
      );
      return paths;
    },
    showGroupAdd() {
      this.$refs.objectPicker.clear();
      this.addingGroup = true;
    },
    cancelGroupAdd() {
      this.addingGroup = false;
    },
    completeGroupAdd() {
      this.addingGroup = false;
      const parentObject = this.project.objects.filter(
        object => object.path + "/" + object.name === this.selectedParentObject
      );
      if (parentObject.length === 1) {
        this.$store.commit("addGroup", parentObject[0]);
      }
    },
    deleteGroup(uuid) {
      this.$store.commit("deleteGroup", uuid);
    }
  }
};
</script>

<style scoped>
</style>
