<template>
  <div style="width:100%;">
    <modal v-show="editorVisible" @close="closeEditor">
      <template v-slot:title>Editing Object []</template>
      <template v-slot:body>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object UUID</span>
          </div>
          <span type="text" class="form-control disabled">1234</span>
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object MD5</span>
          </div>
          <span type="text" class="form-control disabled">1234</span>
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-secondary" @click="closeEditor">Cancel</button>
        <button type="button" class="btn btn-primary" @click="closeEditor">Save Object</button>
      </template>
    </modal>

    <div class="card text-left" v-for="object in getObjectsInView" v-bind:key="object.uuid">
      <div class="card-header">
        <div class="text-right" style="float:right;">
          <button type="button" class="btn btn-info" style="margin: 7px;">Edit</button>
          <button
            type="button"
            class="btn btn-danger"
            @click="deleteObject(object.uuid)"
            style="margin: 7px;"
          >Delete</button>
        </div>
        <div>
          <h5>{{ object.path }}/{{ object.name }}</h5>
          <h6>{{ object.uuid }}</h6>
        </div>
      </div>
      <div class="card-body">
        <h5 class="card-title">{{ object.source }}</h5>
        <p class="card-text">Tags : {{ object.tags }}</p>
        <p class="card-text">Parse : {{ object.parser }}</p>
        <p class="card-text">Size : {{ object.size }}</p>
        <p class="card-text">MD5 : {{ object.hash }}</p>
      </div>
    </div>
  </div>
</template>

<script>
console.log("Creating Object Viewer");

// in full builds helpers are exposed as Vuex.mapState
import { mapState } from "vuex";
import Modal from "./Modal";

export default {
  name: "Objects",
  data() {
    return {
      editorVisible: false,
      filter: "",
      objectsInFilter: []
    };
  },
  components: { Modal },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace,
    getObjectsInView(state) {
      let objects = [];

      state.project.objects.forEach(object => {
        let add = false;
        if (this.filter == "") {
          add = true;
        }

        if (add) {
          objects.push(object);
        }
      });

      return objects;
    }
  }),
  methods: {
    openEditor() {
      this.editorVisible = true;
    },
    closeEditor() {
      this.editorVisible = false;
    },
    deleteObject(uuid) {
      this.$store.commit("deleteObject", uuid);
    }
  }
};
</script>

<style scoped>
</style>
