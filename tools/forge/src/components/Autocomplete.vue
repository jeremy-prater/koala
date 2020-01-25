<script>
export default {
  data() {
    return {
      selection: "",
      open: false,
      current: 0
    };
  },

  props: {
    suggestions: {
      type: Array,
      required: true
    }
  },

  computed: {
    matches() {
      return this.suggestions.filter(str => {
        return str.indexOf(this.selection) >= 0;
      });
    },

    openSuggestion() {
      return (
        this.selection !== "" && this.matches.length != 0 && this.open === true
      );
    }
  },

  methods: {
    clear() {
      this.selection = "";
    },
    enter() {
      if (this.open === true) {
        this.selection = this.matches[this.current];
        this.$emit("updated", this.selection);
        this.open = false;
      }
    },

    up() {
      if (this.current > 0) this.current--;
    },

    down() {
      if (this.current < this.matches.length - 1) this.current++;
    },

    isActive(index) {
      return index === this.current;
    },

    change() {
      if (this.open == false) {
        this.open = true;
        this.current = 0;
      }
      this.$emit("updated", this.selection);
    },

    suggestionClick(index) {
      this.selection = this.matches[index];
      this.$emit("updated", this.selection);
      this.open = false;
    }
  }
};
</script>

<template>
  <div v-bind:class="{'open':openSuggestion}">
    <input
      class="form-control"
      type="text"
      v-model="selection"
      @keydown.enter="enter"
      @keydown.down="down"
      @keydown.up="up"
      @input="change"
    />
    <div class v-if="openSuggestion">
      <a
        class="dropdown-item"
        v-for="(suggestion, index) in matches"
        v-bind:key="suggestion"
        v-bind:class="{'active': isActive(index)}"
        @click="suggestionClick(index)"
      >{{ suggestion }}</a>
    </div>
  </div>
</template>