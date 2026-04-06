
<template>
  <div id="app">
    <PropsConfigForm />
  </div>
</template>

<script>
import PropsConfigForm from './components/PropsConfigForm.vue';

export default {
  name: 'App',
  components: {
    PropsConfigForm
  }
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: left;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
