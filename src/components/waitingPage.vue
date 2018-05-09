<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
  <br><br>

  <div v-if="inGameStatus == true">
    <!-- If we need to pass things to new component, we can send parameters in router-link -->
    <div v-if="role == 'Player'">
      <router-link to="/playerPlayground">
        <button class="btn btn-success btn-block">
          Continue to Game
        </button>

      </router-link>
    </div>

    <div v-if="role == 'Coach'">
      <router-link to="/coachPlayground">
        <button class="btn btn-success btn-block">
          Continue to Game
        </button>

      </router-link>
    </div>

  </div>


  <div v-if="inGameStatus != true">
    <br>
    <br>
    <img src="/static/images/ajax-loader.gif"/>
  </div>
  <br>
  <br>

  <button v-on:click="logout()" class="btn btn-danger btn-block">Log out</button>

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
    role: function(){
      return this.$store.getters.user.role
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
    logout: function(){
      this.$store.dispatch('logout');
    }
  },



}
</script>
