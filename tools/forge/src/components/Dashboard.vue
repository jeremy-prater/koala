<template>
  <div class="container">
    <div class="row">
      <div class="col-sm">
        <div class="card" style="width: 100%;">
          <div class="card-body">
            <h5 class="card-title">{{ project.name }}</h5>
            <h6 class="card-subtitle mb-2 text-muted">Worspace : [{{ currentWorkspace }}]</h6>
            <p class="card-text">{{ totalObjects }} assets</p>
          </div>
        </div>
      </div>
      <div class="col-sm">
        <div class="card" style="width: 100%;">
          <div class="card-body">
            <h5 class="card-title">{{ totalObjects }} assets</h5>
            <h6 class="card-subtitle mb-2 text-muted">Tags</h6>
            <p class="card-text">
              <span class="badge badge-primary" v-for="tag in objectTags" v-bind:key="tag">{{ tag }}</span>
            </p>
            <h6 class="card-subtitle mb-2 text-muted">Parsers</h6>
            <p class="card-text">
              <span
                class="badge badge-primary"
                v-for="parser in objectParsers"
                v-bind:key="parser"
              >{{ parser }}</span>
            </p>
            <h6 class="card-subtitle mb-2 text-muted">Paths</h6>
            <p class="card-text">
              <span
                class="badge badge-primary"
                v-for="path in objectPaths"
                v-bind:key="path"
              >{{ path }}</span>
            </p>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { mapState } from "vuex";
console.log("Creating Dashboard");

export default {
  name: "Dashboard",
  data() {
    return {
      editingName: false
    };
  },
  computed: mapState({
    // arrow functions can make the code very succinct!
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace,
    totalObjects: state => state.project.assets.length,
    objectTags(state) {
      let tags = [];
      state.project.assets.forEach(object => {
        tags.push(...object.tags.split(" "));
      });

      return [...new Set(tags)];
    },
    objectParsers(state) {
      let parsers = [];
      state.project.assets.forEach(object => {
        parsers.push(object.parser);
      });

      return [...new Set(parsers)];
    },
    objectPaths(state) {
      let paths = [];
      state.project.assets.forEach(object => {
        paths.push(object.path);
      });

      return [...new Set(paths)];
    }
  })
};
</script>
