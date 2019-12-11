<template>
  <div>
    It's the dashboard for [{{ project.name }}]
    <br />
    Current workspace : [{{ currentWorkspace }}]
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
    currentWorkspace: state => state.currentWorkspace
  })
};
</script>
