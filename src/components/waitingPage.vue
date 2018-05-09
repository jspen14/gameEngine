<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
      {{userID}}

      <div v-if="inGameStatus == true">
        <!-- If we need to pass things to new component, we can send parameters in router-link -->
        <router-link to="/playerPlayground">
          <button class="btn btn-lg btn-success">
            Continue to Game!
          </button>

        </router-link>


      </div>

      {{inGameStatus}}

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
      inGameStatus: 'false',
    }
  },
  created: function(){
    this.updateData();
    this.$store.dispatch('updateData');
  },
  computed: {
    name: function(){
      return this.$store.getters.user.name;
    },
    userID: function(){
      return this.$store.getters.user.id;
    },
    inGame: function(){
      if(this.$store.getters.inGameStatus == true){
        return true;
      }
      else {return false;}
    },

  },
  methods: {
    updateData: function(){
      let timerID = setInterval(() => {
        this.inGameStatus = this.$store.getters.inGameStatus;

      }, 3000);
    },


    playerID: function(){
      return this.$store.getters.user.id;
    },

  },
  methods: {
    logout: function(){
      this.$store.dispatch('logout');
    }

  },


}
</script>
