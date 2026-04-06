<template>
  <div id="app">
    <MicrophoneAudio />
  </div>
</template>

<script>
import MicrophoneAudio from './components/MicrophoneAudio.vue';

export default {
  name: 'App',
  components: {
    MicrophoneAudio
  }
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
