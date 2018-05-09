<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
      {{playerID}}
  <br>
  <img src="/static/images/ajax-loader.gif"/>
</div>
</template>

<script>
export default{
  data() {
    return {

    }
  },
  computed: {
    playerID: function(){
      return this.$store.getters.user.id;
    },
    name: function(){
      return this.$store.getters.user.name;
    }
  },
  methods: {

  },


}
</script>
