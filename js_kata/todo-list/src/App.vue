<template>
  <v-app>
    <TodoList />
  </v-app>
</template>

<script>
import TodoList from './components/TodoList.vue';

export default {
  name: 'App',
  components: {
    TodoList
  }
};
</script>

<style>
@import '~vuetify/dist/vuetify.min.css';
</style>
