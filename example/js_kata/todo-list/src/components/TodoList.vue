<template>
    <v-container>
      <v-card class="mx-auto" max-width="600">
        <v-toolbar color="primary" dark>
          <v-toolbar-title>To-Do List</v-toolbar-title>
        </v-toolbar>
        <v-card-text>
          <v-form @submit.prevent="addTodo">
            <v-text-field
              v-model="newTodo"
              label="Add a new to-do"
              prepend-icon="mdi-plus"
              @keyup.enter="addTodo"
            ></v-text-field>
            <v-btn color="primary" @click="addTodo">Add To-Do</v-btn>
          </v-form>
          <v-list>
            <v-list-item
              v-for="(todo, index) in todos"
              :key="index"
              class="d-flex justify-space-between"
            >
              <v-list-item-content>
                <v-checkbox v-model="todo.completed" hide-details></v-checkbox>
                <v-list-item-title :class="{ 'completed-task': todo.completed }">{{ todo.text }}</v-list-item-title>
              </v-list-item-content>
              <v-btn icon @click="removeTodo(index)">
                <v-icon>mdi-delete</v-icon>
              </v-btn>
            </v-list-item>
          </v-list>
        </v-card-text>
      </v-card>
    </v-container>
  </template>
  
  <script>
  export default {
    data() {
      return {
        newTodo: '',
        todos: []
      };
    },
    methods: {
      addTodo() {
        if (this.newTodo.trim()) {
          this.todos.push({ text: this.newTodo, completed: false });
          this.newTodo = '';
        }
      },
      removeTodo(index) {
        this.todos.splice(index, 1);
      }
    }
  };
  </script>
  
  <style scoped>
  .completed-task {
    text-decoration: line-through;
  }
  </style>
  