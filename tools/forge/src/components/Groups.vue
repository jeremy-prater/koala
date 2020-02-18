<template>
  <div style="width:100%;">
    <modal v-show="addingGroup" @close="cancelGroupAdd">
      <template v-slot:title>Add New Group</template>
      <template v-slot:body>
        <div class="container">
          <div class="row no-gutters">
            <div class="col-3">
              <span class="form-control input-group-text">Group Name</span>
            </div>
            <div class="col">
              <input type="text" class="form-control" v-model="newGroupName" />
            </div>
          </div>
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
      <div class="input-group mb-3">
        <button
          type="button"
          class="btn btn-primary"
          @click="showGroupAdd"
          style="margin-right: 7px;"
        >Add Group</button>

        <div class="input-group-prepend">
          <span class="form-control input-group-text">Group Filter</span>
        </div>
        <input type="text" class="form-control" v-model="groupFilter" />
      </div>
    </div>

    <GroupCard v-for="group in filteredGroups" v-bind:key="group.uuid" :group="group" />
  </div>
</template>

<script>
console.log("Creating Groups Editor");

// in full builds helpers are exposed as Vuex.mapState
import { mapState } from "vuex";
import Modal from "./Modal";
import Autocomplete from "./Autocomplete";
import GroupCard from "./GroupCard";

export default {
  name: "Groups",
  data() {
    return {
      selectedParentObject: "",
      addingGroup: false,
      groupFilter: "",
      newGroupName: ""
    };
  },
  components: { Modal, Autocomplete, GroupCard },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace,
    filteredGroups() {
      let groups = [];
      this.project.groups.forEach(group => {
        if (this.groupFilter) {
          if (group.parentPath.includes(this.groupFilter)) {
            groups.push(group);
          }
        } else groups.push(group);
      });
      return groups;
    }
  }),
  methods: {
    objectPickerUpdated(parent) {
      this.selectedParentObject = parent;
      this.newGroupName = parent;
    },
    getObjectList() {
      let paths = [];
      this.project.assets.forEach(object =>
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
      const parentObject = this.project.assets.filter(
        object => object.path + "/" + object.name === this.selectedParentObject
      );
      if (parentObject.length === 1) {
        this.$store.commit("addGroup", {
          parentObject: parentObject[0],
          name: this.newGroupName
        });
      }
    },
    getObjectByPath(path) {
      console.log(path);
      this.project.assets.forEach(object => {
        const computedPath = object.path + "/" + object.name;
        console.log(computedPath);
        if (path === computedPath) return object;
      });
      console.log("no object found");
      return undefined;
    }
  }
};
</script>

<style scoped>
</style>
