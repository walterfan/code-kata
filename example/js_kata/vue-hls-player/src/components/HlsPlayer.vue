<template>
    <div class="hls-player">
      <video
        id="video"
        ref="videoRef"
        width="100%"
        controls
        class="video-js vjs-default-skin"
      ></video>
    </div>
  </template>
  
  <script setup>
  import { onMounted, ref } from 'vue';
  import Hls from 'hls.js';
  
  const videoRef = ref(null);
  
  onMounted(() => {
    const videoElement = videoRef.value;
  
    // Check if HLS is supported by the browser
    if (Hls.isSupported()) {
      const hls = new Hls();
      hls.loadSource('/public/snapshot_20240814191125.m3u8');
      hls.attachMedia(videoElement);
  
      hls.on(Hls.Events.MANIFEST_PARSED, function() {
        videoElement.play();
      });
  
      hls.on(Hls.Events.ERROR, function(event, data) {
        if (data.fatal) {
          switch(data.type) {
            case Hls.ErrorTypes.NETWORK_ERROR:
              // try to recover network error
              hls.startLoad();
              break;
            case Hls.ErrorTypes.MEDIA_ERROR:
              hls.recoverMediaError();
              break;
            default:
              // cannot recover
              hls.destroy();
              break;
          }
        }
      });
    } else if (videoElement.canPlayType('application/vnd.apple.mpegurl')) {
      videoElement.src = 'https://example.com/path/to/your/video.m3u8';
      videoElement.addEventListener('loadedmetadata', function() {
        videoElement.play();
      });
    } else {
      console.error('HLS or native M3U8 playback not supported.');
    }
  });
  </script>
  
  <style>
  .video-js {
    width: 100%;
    height: auto;
  }
  </style>