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

    <modal v-show="addingMetadata" @close="cancelAddMetadata">
      <template
        v-slot:title
      >Add metadata to {{ currentObjectMetadata.path }}/{{ currentObjectMetadata.name }}</template>
      <template v-slot:body>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <input type="text" class="form-control input-group-text" v-model="newMetadata.key" />
          </div>
          <input type="text" class="form-control" v-model="newMetadata.value" />
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-secondary" @click="cancelAddMetadata">Cancel</button>
        <button type="button" class="btn btn-primary" @click="completeAddMetadata">Add Metadata</button>
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
        <div class="container">
          <div class="row">
            <div class="col">
              <h5 class="card-title">{{ object.source }}</h5>
              <p class="card-text">Tags : {{ object.tags }}</p>
              <p class="card-text">Parser : {{ object.parser }}</p>
              <p class="card-text">Size : {{ object.size }}</p>
              <p class="card-text">MD5 : {{ object.hash }}</p>
            </div>
            <div class="col-3">
              <ViewerglTF
                v-if="object.parser == 'gltf'"
                style="width: 100%; height:100%; background-color:#333"
                v-bind:object="object"
              />
            </div>
          </div>
          <div class="row">
            <div class="col">
              <h5 class="card-title">
                Metadata
                <button
                  type="button"
                  class="btn btn-secondary"
                  @click="startAddMetadata(object)"
                >Add Metadata</button>
              </h5>

              <div
                v-for="key in getObjectMetadataKeys(object)"
                v-bind:key="`${object.uuid}.${key}`"
                class="input-group mb-3"
              >
                <div class="input-group-prepend">
                  <span class="form-control input-group-text">{{ key }}</span>
                </div>
                <input type="text" class="form-control" v-model="object.metadata[key]" />
                <button
                  class="btn btn-outline-danger"
                  type="button"
                  @click="deleteMetadata(object, key)"
                >Delete</button>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
console.log("Creating Object Viewer");

// in full builds helpers are exposed as Vuex.mapState
import { mapState } from "vuex";
import Modal from "./Modal";
import ViewerglTF from "./viewer-gltf";

export default {
  name: "Assets",
  data() {
    return {
      editorVisible: false,
      filter: "",
      objectsInFilter: [],

      addingMetadata: false,
      currentObjectMetadata: {},
      newMetadata: {
        key: "New Key",
        value: "New Value"
      }
    };
  },
  components: { Modal, ViewerglTF },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace,
    getObjectsInView(state) {
      let assets = [];

      state.project.assets.forEach(object => {
        let add = false;
        if (this.filter == "") {
          add = true;
        }

        if (add) {
          assets.push(object);
        }
      });

      return assets;
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
    },
    getObjectMetadataKeys(object) {
      return Object.keys(object.metadata).sort();
    },
    startAddMetadata(object) {
      this.newMetadata = {
        key: "New Key",
        value: "New Value"
      };
      this.currentObjectMetadata = object;
      this.addingMetadata = true;
    },
    cancelAddMetadata() {
      this.addingMetadata = false;
    },
    completeAddMetadata() {
      this.$store.commit("addMetadata", {
        uuid: this.currentObjectMetadata.uuid,
        key: this.newMetadata.key,
        value: this.newMetadata.value
      });
      this.addingMetadata = false;
    },
    deleteMetadata(object, key) {
      this.$store.commit("deleteMetadata", { uuid: object.uuid, key: key });
      // Do this to kick the vue-renderer update
      this.addingMetadata = true;
      this.addingMetadata = false;
    }
  }
};
</script>

<style scoped>
</style>
