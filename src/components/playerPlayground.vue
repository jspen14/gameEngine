<style scoped>
  .boardHeader{
    min-height: 5vh;
    border: solid 1px black;
  }

  .boardRow{
    min-height: 10vh;
    border: solid 1px black;
  }
</style>

<template>

  <div class="container">
    <h3> {{name}} Objective: Win as much money as possible. </h3>
    <hr>
    <div class="row">


      <div class="col-lg-5 col-md-5 col-sm-5 col-xs-5">
        <chatDisplay></chatDisplay>
      </div>

      <div class="col-lg-2 col-md-2 col-sm-2 col-xs-2"></div>

      <div class='col-lg-5 col-md-5 col-sm-5 col-xs-5'>

          <gameBoard></gameBoard> 
          <earnings></earnings>
          <br>
          <div v-show="gameState==='done'">
            <button v-if="isLastRound" class="btn btn-success" v-on:click="gotoEndGame()">Finish Game</button>
            <button v-else class="btn btn-success" v-on:click="readyForNextRound()">Go to Next Round</button>
          </div>

          <h3 v-show="gameState==='submitted'">Waiting for other player...</h3>
          <h3 v-show="gameState==='isReady'">Waiting for other player...</h3>
          <br>
          <button class="btn btn-danger" v-on:click="logout()">Logout</button>

      </div>
    </div>
  </div>


</template>

<script>
import axios from 'axios'

  export default {
    name: 'playerPlayground',

    data () {
      return {
        playerOption: '',
        msgText: '',
        messages: [],

      }
    },

    created: function() {

      this.updateGame();

    },
    computed: {
      gameState: function() {
        return this.$store.getters.gameState;
      },
      isLastRound: function(){
        return this.$store.getters.currentRound==this.$store.getters.numberOfRounds;
      },
      name: function(){
        return this.$store.getters.user.name;
      }
    },
    methods: {
      gotoEndGame: function(){
        this.$store.dispatch('gotoEndGame');
      },
      updateGame: function(){
        this.$store.dispatch('updateGame');
    },
      submitOption: function(param){
      console.log(param);
        axios.put('/api/p1roundOption',{
          roundOption: param,
        });
        this.getRoundEarnings();
        this.getAverageEarnings();
        this.getTotalEarnings();
      },


      getChatMsgs: function(){
        axios.get('/api/p1coachChat').then(response => {
          this.messages = response.data;
          return true;
        }).catch (err => {
        });
      },

      addChatMsg: function(){
        axios.post('/api/p1coachChat/', {
          text: this.msgText,
          role: this.role,
        }).then(response => {
          this.msgText = '',
          this.getChatMsgs();
          return true;
        }).catch(err => {
        });
      },

      readyForNextRound: function(){
        this.$store.dispatch('readyForNextRound');
      },
      logout: function(){
        this.$store.dispatch('logout');
      }

  }
  
}
</script>
