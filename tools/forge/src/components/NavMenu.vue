<template>
  <div>
    <modal v-show="saveInProgress" @close="closeProjectSave" :key="saveProgress.width">
      <template v-slot:title>Exporting workspace</template>
      <template v-slot:body>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Source File</span>
          </div>
          <span type="text" class="form-control disabled">{{ currentObject.source }}</span>
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Target Object</span>
          </div>
          <span type="text" class="form-control disabled">{{ currentObject.uuid }}</span>
        </div>
        <div class="progress">
          <div
            class="progress-bar progress-bar-striped progress-bar-animated"
            role="progressbar"
            v-bind:style="saveProgress"
          ></div>
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-danger" @click="cancelProjectSave">Abort</button>
      </template>
    </modal>

    <modal v-show="editingWorkspace" @close="closeWorkspaceEditor">
      <template v-slot:title>Open new workspace</template>
      <template v-slot:body>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Workspace</span>
          </div>
          <input type="text" class="form-control" v-model="newWorkspace" />
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-secondary" @click="closeWorkspaceEditor">Cancel</button>
        <button type="button" class="btn btn-primary" @click="openWorkspace">Open Workspace</button>
      </template>
    </modal>

    <modal v-show="isAddFileVisible" @close="skipToNextFile">
      <template v-slot:title>Adding file {{ nextFile }}</template>
      <template v-slot:body>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object UUID</span>
          </div>
          <span type="text" class="form-control disabled">{{ newObjectProperties.uuid }}</span>
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object MD5</span>
          </div>
          <span type="text" class="form-control disabled">{{ newObjectProperties.hash }}</span>
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object Source</span>
          </div>
          <span type="text" class="form-control disabled">{{ newObjectProperties.source }}</span>
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object Size</span>
          </div>
          <span type="text" class="form-control disabled">{{ newObjectProperties.size }}</span>
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object Name</span>
          </div>
          <input type="text" class="form-control" v-model="newObjectProperties.name" />
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object Path</span>
          </div>
          <input type="text" class="form-control" v-model="newObjectProperties.path" />
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Object Tags</span>
          </div>
          <input type="text" class="form-control" v-model="newObjectProperties.tags" />
        </div>
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <span class="input-group-text" id="inputGroup-sizing-default">Parser</span>
          </div>
          <input type="text" class="form-control" v-model="newObjectProperties.parser" />
        </div>
      </template>
      <template v-slot:footer>
        <button type="button" class="btn btn-secondary" @click="skipToNextFile">Cancel</button>
        <button type="button" class="btn btn-primary" @click="addObject">Add Object</button>
      </template>
    </modal>

    <nav class="navbar navbar-dark fixed-top bg-dark flex-md-nowrap p-0 shadow">
      <a
        v-if="!editingName"
        class="navbar-brand col-sm-3 col-md-2 mr-0"
        v-on:click="editName(true)"
      >{{ project.name }}</a>
      <input
        v-if="editingName"
        v-on:blur="editName(false)"
        v-on:keyup.enter="editName(false)"
        class="navbar-brand col-sm-3 col-md-2 mr-0"
        v-model="project.name"
      />
      <ul class="navbar-nav list-group-horizontal px-3">
        <li class="nav-item text-nowrap">
          <span
            class="nav-link"
            v-bind:class="{ 'active': droppingEnabled }"
            @dragenter="onDragEvent"
            @dragover="onDragEvent"
            @dragend="onDragEvent"
            @dragleave="onDragEvent"
            @drop="onDragEvent"
          >
            <i class="fas fa-plus-square fa-2x navIcon" />
          </span>
        </li>
        <li class="nav-item text-nowrap openbutton">
          <a class="nav-link" v-on:click="showWorkspaceEditor">
            <i class="fas fa-folder-open fa-2x navIcon" />
          </a>
        </li>
      </ul>
      <input
        class="form-control form-control-dark w-100"
        type="text"
        placeholder="Search"
        aria-label="Search"
      />
      <ul class="navbar-nav list-group-horizontal px-3">
        <li class="nav-item text-nowrap">
          <a class="nav-link-eject" v-on:click="saveProject">
            <i class="fas fa-download fa-2x navIcon" />
          </a>
        </li>
      </ul>
    </nav>

    <div class="container-fluid">
      <div class="row">
        <nav class="col-md-2 d-none d-md-block bg-light sidebar">
          <div class="sidebar-sticky">
            <h6
              class="sidebar-heading d-flex justify-content-between align-items-center px-3 mt-4 mb-1 text-muted"
            >
              <span>Engine - Assets</span>
            </h6>
            <ul class="nav flex-column">
              <li class="nav-item">
                <router-link to="/dashboard" class="nav-link" exact>
                  <i class="fas fa-project-diagram" />
                  Project
                </router-link>
              </li>
              <li class="nav-item">
                <router-link to="/objects" class="nav-link" exact>
                  <i class="fas fa-object-group" />
                  Objects
                </router-link>
              </li>
              <li class="nav-item">
                <router-link to="/groups" class="nav-link" exact>
                  <i class="fas fa-layer-group" />
                  Groups
                </router-link>
              </li>
              <li class="nav-item">
                <router-link to="/scripts" class="nav-link" exact>
                  <i class="fas fa-scroll" />
                  Scripts
                </router-link>
              </li>
              <li class="nav-item">
                <router-link to="/settings" class="nav-link" exact>
                  <i class="fas fa-cogs" />
                  Settings
                </router-link>
              </li>
            </ul>

            <h6
              class="sidebar-heading d-flex justify-content-between align-items-center px-3 mt-4 mb-1 text-muted"
            >
              <span>Engine - Runtime</span>
            </h6>
            <ul class="nav flex-column">
              <li class="nav-item">
                <router-link to="/runtime-connect" class="nav-link" exact>
                  <i class="fas fa-cogs" />
                  Connect to engine
                </router-link>
              </li>
              <li class="nav-item">
                <router-link to="/runtime-memory" class="nav-link" exact>
                  <i class="fas fa-microchip" />
                  Memory
                </router-link>
              </li>
              <li class="nav-item">
                <router-link to="/runtime-speed" class="nav-link" exact>
                  <i class="fas fa-tachometer-alt" />
                  Performance
                </router-link>
              </li>
              <li class="nav-item">
                <router-link to="/runtime-browse" class="nav-link" exact>
                  <i class="fas fa-copy" />
                  Scene browser
                </router-link>
              </li>
            </ul>
          </div>
        </nav>

        <main role="main" class="col-md-9 ml-sm-auto col-lg-10 px-4">
          <div
            class="d-flex justify-content-between flex-wrap flex-md-nowrap align-items-center pt-3 pb-2 mb-3 border-bottom"
          >
            <router-view></router-view>
          </div>
        </main>
      </div>
    </div>
  </div>
</template>

<script>
console.log("Creating NavMenu");

// in full builds helpers are exposed as Vuex.mapState
import { mapState } from "vuex";
import Modal from "./Modal";

const remote = require("electron").remote;
const fs = remote.require("fs");
const path = require("path");
const uuidv1 = require("uuid/v1");
const md5File = require("md5-file");

export default {
  name: "NavMenu",
  data() {
    return {
      editingName: false,
      editingWorkspace: false,
      droppingEnabled: false,
      isAddFileVisible: false,
      nextFile: "",
      newWorkspace: "",
      newObjectProperties: {},
      addFileLists: [],
      saveInProgress: false,
      saveProgress: { width: "0%" },
      cancelSave: false,
      currentObject: {}
    };
  },
  components: { Modal },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace
  }),
  methods: {
    editName: function(editing) {
      console.log(`Edit project name [${editing}]`);
      this.editingName = editing;
    },
    updateSaveProgress: function(pos, max) {
      let value = (pos / max) * 100;
      console.log(`Save Progress [${value}]`);
      this.saveProgress = {
        width: `${value}%`
      };
    },
    saveProject: function() {
      this.cancelSave = false;
      const totalCount = this.project.objects.length + 1;
      let currentCount = 0;
      let outputFile = path.join(this.currentWorkspace, "projectConfig.json");

      this.saveInProgress = true;

      setTimeout(() => {
        console.log(`Saving Project [${outputFile}]`);

        const saveObject = function() {
          if (currentCount != totalCount - 1) {
            let object = this.project.objects[currentCount];
            this.updateSaveProgress(++currentCount, totalCount);
            let outputFile = path.join(this.currentWorkspace, object.uuid);
            console.log(`Copying [${object.source}] ==> [${outputFile}]`);
            fs.copyFileSync(object.source, outputFile);
            object.hash = md5File.sync(object.source);
            object.size = fs.statSync(object.source)["size"];

            if (this.cancelSave) {
              this.cancelSave = false;
              this.currentObject = {
                source: "Export Aborted",
                uuid: "Export Aborted"
              };
            } else {
              setTimeout(saveObject, 0);
            }
          } else {
            const projectConfig = JSON.stringify(this.project, null, 4);
            fs.writeFileSync(outputFile, projectConfig);

            this.updateSaveProgress(++currentCount, totalCount);

            this.currentObject = {
              source: "Export Complete",
              uuid: "Export Complete"
            };
          }
        }.bind(this);

        setTimeout(saveObject, 0);
      }, 0);
    },
    onDragEvent: function(e) {
      e.preventDefault();
      e.dataTransfer.dropEffect = "copy";
      if (e.type == "dragover") {
        this.droppingEnabled = true;
      } else {
        this.droppingEnabled = false;
      }
      if (e.type == "drop") {
        this.addFileLists = [];
        e.dataTransfer.files.forEach(file => {
          const sourceFile = file.path; //= path.join(file.path, file.name);
          // console.log(`Adding Dropped file ==> [${sourceFile}]`);
          this.addFileLists.push(sourceFile);
        });
      }

      if (this.addFileLists.length > 0) {
        this.nextFile = this.addFileLists.pop();
        this.showAddFileModal();
      }

      return false;
    },
    getRelativePath: function(parentPath) {
      console.log("getRelativePath");

      // Split by / and remove sequence...
      // relative path!
      console.log(this.currentWorkspace);
      console.log(parentPath);

      return parentPath;
    },
    showAddFileModal() {
      // console.log(`Showing add dialog for [${this.nextFile}]`);
      const filename = path.basename(this.nextFile);
      const lastDot = filename.lastIndexOf(".");
      const name = filename.substring(0, lastDot);
      const parser = filename.substring(lastDot + 1);
      this.newObjectProperties = {
        uuid: uuidv1(),
        tags: "default",
        name: name,
        path: "/default",
        parser: parser,
        hash: md5File.sync(this.nextFile),
        source: this.getRelativePath(this.nextFile),
        size: fs.statSync(this.nextFile)["size"],
        metadata: {}
      };
      this.isAddFileVisible = true;
    },
    closeModal() {
      this.isAddFileVisible = false;
    },
    skipToNextFile() {
      this.closeModal();
      this.nextFile = this.addFileLists.pop();
      if (this.nextFile != undefined) {
        this.showAddFileModal();
      }
    },
    addObject() {
      //console.log(`Adding Object [${this.nextFile}]`);
      this.$store.commit("addObject", this.newObjectProperties);
      this.skipToNextFile();
    },
    showWorkspaceEditor: function() {
      console.log("Open new workspace");
      this.newWorkspace = this.currentWorkspace;
      this.editingWorkspace = true;
    },
    closeWorkspaceEditor() {
      this.editingWorkspace = false;
    },
    openWorkspace() {
      this.$store.commit("setWorkspace", this.newWorkspace);
      this.$store.commit("loadProject");
      this.closeWorkspaceEditor();
    },
    cancelProjectSave() {
      console.log("Aboring Project Save...");
      this.cancelSave = true;
    },
    closeProjectSave() {
      this.saveInProgress = false;
    }
  }
};
</script>

<style scoped>
body {
  font-size: 0.875rem;
}

.objectEdit {
  width: 100%;
}
.feather {
  width: 16px;
  height: 16px;
  vertical-align: text-bottom;
}

/*
 * Sidebar
 */

.sidebar {
  position: fixed;
  top: 0;
  bottom: 0;
  left: 0;
  z-index: 100; /* Behind the navbar */
  padding: 48px 0 0; /* Height of navbar */
  box-shadow: inset -1px 0 0 rgba(0, 0, 0, 0.1);
}

.sidebar-sticky {
  position: relative;
  top: 0;
  height: calc(100vh - 48px);
  padding-top: 0.5rem;
  overflow-x: hidden;
  overflow-y: auto; /* Scrollable contents if viewport is shorter than content. */
}

@supports ((position: -webkit-sticky) or (position: sticky)) {
  .sidebar-sticky {
    position: -webkit-sticky;
    position: sticky;
  }
}

.sidebar .nav-link {
  font-weight: 500;
  color: #333;
}

.sidebar .nav-link .feather {
  margin-right: 4px;
  color: #999;
}

.sidebar .nav-link.active {
  color: #007bff;
}

.sidebar .router-link-active {
  color: #007bff;
}

.sidebar .nav-link:hover .feather,
.sidebar .nav-link.active .feather {
  color: inherit;
}

.sidebar-heading {
  font-size: 0.75rem;
  text-transform: uppercase;
}

.nav-link-eject {
  color: #d33;
}
.nav-link-eject:hover {
  color: #f55;
}

.openbutton {
  border: 10px;
}
/*
 * Content
 */

[role="main"] {
  padding-top: 133px; /* Space for fixed navbar */
}

@media (min-width: 768px) {
  [role="main"] {
    padding-top: 48px; /* Space for fixed navbar */
  }
}

/*
 * Navbar
 */

.navIcon {
  margin-left: 10px;
  margin-right: 10px;
}

.navbar-brand {
  padding-top: 0.75rem;
  padding-bottom: 0.75rem;
  font-size: 1rem;
  background-color: rgba(0, 0, 0, 0.25);
  box-shadow: inset -1px 0 0 rgba(0, 0, 0, 0.25);
}

.navbar .form-control {
  padding: 0.75rem 1rem;
  border-width: 0;
  border-radius: 0;
}

.form-control-dark {
  color: #fff;
  background-color: rgba(255, 255, 255, 0.1);
  border-color: rgba(255, 255, 255, 0.1);
}

.form-control-dark:focus {
  border-color: transparent;
  box-shadow: 0 0 0 3px rgba(255, 255, 255, 0.25);
}
</style>