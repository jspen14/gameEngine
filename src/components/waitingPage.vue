<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
      {{userID}}


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
  created: function(){
    this.updateData();
    
  },
  computed: {
    name: function(){
      return this.$store.getters.user.name;
    },
    userID: function(){
      return this.$store.getters.user.id;
    },

  },
  methods: {
    
    updateData: function(){
     
        this.$store.dispatch('updateData');
    },
    logout: function(){
      this.$store.dispatch('logout');
    },
    
  },


}
</script>
