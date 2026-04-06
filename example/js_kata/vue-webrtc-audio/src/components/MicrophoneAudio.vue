<template>
    <div>
      <h1>Microphone Audio Input</h1>
      <button @click="startRecording">Start Recording</button>
      <button @click="stopRecording">Stop Recording</button>
      <p v-if="error">{{ error }}</p>
      <canvas ref="canvas" width="300" height="150"></canvas>
    </div>
  </template>
  
  <script>
  export default {
    data() {
      return {
        audioContext: null,
        mediaStream: null,
        mediaRecorder: null,
        error: null,
      };
    },
    methods: {
      async startRecording() {
        try {
          this.mediaStream = await navigator.mediaDevices.getUserMedia({ audio: true });
          this.audioContext = new (window.AudioContext || window.webkitAudioContext)();
          const source = this.audioContext.createMediaStreamSource(this.mediaStream);
          const analyser = this.audioContext.createAnalyser();
          source.connect(analyser);
          this.draw(analyser);
        } catch (err) {
          this.error = `Error accessing microphone: ${err.message}`;
        }
      },
      stopRecording() {
        if (this.mediaStream) {
          this.mediaStream.getTracks().forEach(track => track.stop());
          this.mediaStream = null;
        }
        if (this.audioContext) {
          this.audioContext.close();
          this.audioContext = null;
        }
      },
      draw(analyser) {
        const canvas = this.$refs.canvas;
        const canvasCtx = canvas.getContext('2d');
        const bufferLength = analyser.frequencyBinCount;
        const dataArray = new Uint8Array(bufferLength);
  
        const draw = () => {
          requestAnimationFrame(draw);
          analyser.getByteTimeDomainData(dataArray);
  
          canvasCtx.fillStyle = 'rgb(200, 200, 200)';
          canvasCtx.fillRect(0, 0, canvas.width, canvas.height);
  
          canvasCtx.lineWidth = 2;
          canvasCtx.strokeStyle = 'rgb(0, 0, 0)';
          canvasCtx.beginPath();
  
          const sliceWidth = (canvas.width * 1.0) / bufferLength;
          let x = 0;
  
          for (let i = 0; i < bufferLength; i++) {
            const v = dataArray[i] / 128.0;
            const y = (v * canvas.height) / 2;
  
            if (i === 0) {
              canvasCtx.moveTo(x, y);
            } else {
              canvasCtx.lineTo(x, y);
            }
  
            x += sliceWidth;
          }
  
          canvasCtx.lineTo(canvas.width, canvas.height / 2);
          canvasCtx.stroke();
        };
  
        draw();
      }
    }
  };
  </script>
  
  <style scoped>
  canvas {
    border: 1px solid black;
  }
  </style>
