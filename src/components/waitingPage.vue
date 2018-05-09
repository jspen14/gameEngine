<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
      {{playerID}}
  <br>
  <img src="/static/images/ajax-loader.gif"/>
  <br>
  <button v-on:click="logout()" class="btn btn-primary">Log out</button>
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
    logout: function(){
      this.$store.dispatch('logout');
    }
  },


}
</script>
