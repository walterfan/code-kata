<template>
    <div class="container mt-5">
      <h2>Props Configuration Form</h2>
      <hr/>
      <form @submit.prevent="submitForm" class="needs-validation" novalidate>
        <div class="form-group">
          <label for="pipelineName">Pipeline Name</label>
          <input type="text" class="form-control" id="pipelineName" v-model="form.pipeline_name" required>
        </div>
        <div class="form-group">
          <label for="elementName">Element Name</label>
          <input type="text" class="form-control" id="elementName" v-model="form.element_name" required>
        </div>
        <div class="form-group">
          <label for="propKey">Property Key</label>
          <input type="text" class="form-control" id="propKey" v-model="form.prop_key" required>
        </div>
        <div class="form-group">
          <label for="propValue">Property Value</label>
          <input type="text" class="form-control" id="propValue" v-model="form.prop_value" required>
        </div>
        <div class="form-group">
          <label for="propType">Property Type</label>
          <select class="form-control" id="propType" v-model="form.prop_type" required>
            <option value="">Select a type</option>
            <option value="string">String</option>
            <option value="number">Number</option>
            <option value="boolean">Boolean</option>
          </select>
        </div>
        <button type="submit" class="btn btn-primary">Submit</button>
      </form>
    </div>
  </template>
  
  <script>
  export default {
    data() {
      return {
        form: {
          pipeline_name: '',
          element_name: '',
          prop_key: '',
          prop_value: '',
          prop_type: '',
        }
      };
    },
    methods: {
      submitForm() {
        console.log('Form submitted:', this.form);
        // 在这里添加提交表单的逻辑
      }
    }
  };
  </script>
  
  <style scoped>
  /* 你可以在这里添加自定义样式 */
  </style>
