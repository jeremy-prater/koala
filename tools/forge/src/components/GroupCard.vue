<template>
  <div>
    <div class="card text-left">
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
          <h5>Group {{ this.group.uuid }}</h5>
          <h5>Parent Asset [{{ parentObject.parser }}] {{ this.group.parentPath }}</h5>
        </div>
      </div>
      <div class="card-body">
        <div class="container"></div>
        <div
          class="row"
          v-if="parentObject.parser === 'gltf'"
          v-observe-visibility="visibilityChanged"
        >
          <h3>It's for sure GLTF</h3>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { mapState } from "vuex";

export default {
  name: "GroupCard",
  data() {
    return {
      isVisible: false
    };
  },
  props: ["group"],
  computed: mapState({
    // arrow functions can make the code very succinct!
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace,
    parentObject: function(state) {
      return state.project.objects.filter(
        object => object.path + "/" + object.name === this.group.parentPath
      )[0];
    }
  }),
  methods: {
    visibilityChanged(isVisible) {
      this.isVisible = isVisible;
    },
    deleteGroup(uuid) {
      this.$store.commit("deleteGroup", uuid);
    }
  }
};
</script>
