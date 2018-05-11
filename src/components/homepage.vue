<template>
<div class = "container">
<div v-if="loggedIn">
  <div v-if="inGameStatus">
    <player-playground/>
  </div>
  <div v-else>
    <waiting-page/>
  </div>
</div>
<div v-else>
    <login-register/>
  </div>
</div>
</template>
<script>
  import axios from 'axios'
  import loginRegister from './loginRegister'
  import waitingPage from './waitingPage'
  import playerPlayground from './playerPlayground'
export default{
  name: 'homepage',
  components: {loginRegister,waitingPage, playerPlayground},
  computed: {
    loggedIn:function () {
      return this.$store.getters.loggedIn;
    },
    inGameStatus: function() {
      return this.$store.getters.inGameStatus;

    },
    gameState: function(){
      return this.$store.getters.gameState;
    }
  },

}
</script>
<style scoped>
a:hover{
  text-decoration: none;
}


.header{
  font-size: 5vh;
}
.subHeader{
  font-size: 4vh;
}
.roleOptions{
  font-size: 3vh;
}

</style>
