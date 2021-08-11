<template lang="pug">
  v-app
    v-app-bar(
      app
      color="primary"
      dark
    )
      h3 n8shell v22
    v-main
      router-view
</template>

<script lang="ts">
import Vue from 'vue';

export default Vue.extend({
  name: 'App',

  data: () => ({
    //
  }),
});
</script>

<style lang="stylus">
*
  box-sizing border-box
html
  height 100%
body
  height 100%
  margin 0

#app
  height 100%
  margin 0
  font-family 'Avenir', Helvetica, Arial, sans-serif
  -webkit-font-smoothing antialiased
  -moz-osx-font-smoothing grayscale
  text-align left
  color #2c3e50

  background-color #ddd
  .content, .content > *
    height 100%

  .vertical-spacer
    height 40px

  .rot90
    transform rotateZ(90deg)
  .roty180
    transform rotateY(180deg)
</style>

<style scoped lang="stylus">
.current
  background-color lightgrey
</style>