<template>
  <div>
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
          <span class="nav-link">
            <i class="fas fa-plus-square fa-2x navIcon" />
          </span>
        </li>
        <li class="nav-item text-nowrap openbutton">
          <a class="nav-link" v-on:click="openFolder">
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
                <router-link to="/" class="nav-link" exact>
                  <i class="fas fa-layer-group" />
                  Groups
                </router-link>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <i class="fas fa-scroll" />
                  Scripts
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <i class="fas fa-cogs" />
                  Settings
                </a>
              </li>
            </ul>

            <h6
              class="sidebar-heading d-flex justify-content-between align-items-center px-3 mt-4 mb-1 text-muted"
            >
              <span>Saved reports</span>
              <a class="d-flex align-items-center text-muted" href="#">
                <span data-feather="plus-circle"></span>
              </a>
            </h6>
            <ul class="nav flex-column mb-2">
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file-text"></span>
                  Current month
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file-text"></span>
                  Last quarter
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file-text"></span>
                  Social engagement
                </a>
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
const remote = require("electron").remote;
const fs = remote.require("fs");
const path = require("path");

export default {
  name: "NavMenu",
  data() {
    return {
      editingName: false
    };
  },
  computed: mapState({
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace
  }),
  methods: {
    editName: function(editing) {
      console.log(`Edit project name [${editing}]`);
      this.editingName = editing;
    },
    openFolder: function() {
      console.log(`Open new workspace`);
      this.$store.commit('loadProject');
    },
    loadProject: function() {
      this.$store.commit("loadProject");
    },
    saveProject: function() {
      const projectConfig = JSON.stringify(this.project);
      const outputFile = path.join(this.currentWorkspace, "projectConfig.json");
      console.log(`Saving Project [${outputFile}]`);
      fs.writeFileSync(outputFile, projectConfig);
    }
  }
};
</script>

<style scoped>
body {
  font-size: 0.875rem;
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