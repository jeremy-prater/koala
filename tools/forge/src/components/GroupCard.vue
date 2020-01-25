<template>
  <div>
    <div class="card text-left">
      <div class="card-header text-">
        <div class="text-right" style="float:right;">
          <button
            type="button"
            class="btn btn-danger"
            @click="deleteGroup(this.group.uuid)"
            style="margin: 7px;"
          >Delete</button>
        </div>
        <div>
          <h5>Group {{ this.group.uuid }}</h5>
          <h5>Parent Asset {{ this.group.parentPath }}</h5>
        </div>
      </div>
      <div class="card-body">
        <div class="container"></div>
      </div>
    </div>
  </div>
</template>

<script>
import { mapState } from "vuex";

export default {
  name: "GroupCard",
  data() {
    return {};
  },
  props: ["group"],
  computed: mapState({
    // arrow functions can make the code very succinct!
    project: state => state.project,
    currentWorkspace: state => state.currentWorkspace,
    totalObjects: state => state.project.objects.length
  })
};
</script>
