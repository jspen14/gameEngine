<template>
<div class = "container">
  <div v-if="loggedIn">
    <div v-if="inGameStatus">
      <div v-if="isPlayer">
        <end-game-page v-if="gameState==='endGame'"/>
        <player-playground v-else/>
      </div>
      <div v-else>
        <!--Add v-if/else to make it so coach-playground deactivates when game is over -->
        <coach-playground/>
      </div>
    </div>
    <div v-else><waiting-page/></div>
  </div>
  <div v-else><login-register/></div>
</div>
</template>
<script>
  import axios from 'axios'
  import loginRegister from './loginRegister'
  import waitingPage from './waitingPage'
  import playerPlayground from './playerPlayground'
  import endGamePage from './endGamePage'
  import coachPlayground from './coachPlayground'
export default{
  name: 'homepage',
  components: {loginRegister,waitingPage, playerPlayground, endGamePage, coachPlayground},
  computed: {
    loggedIn:function () {
      return this.$store.getters.loggedIn;
    },
    inGameStatus: function() {
      return this.$store.getters.inGameStatus;

    },
    gameState: function(){
      return this.$store.getters.gameState;
    },
    isPlayer: function(){
      if(this.$store.getters.user.role=='Player')
        return true;
      return false;
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
