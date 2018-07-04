<style scoped>

</style>

<template>

<div class="container">
  <h3>  Objective: Win as much money as possible. </h3>
  <hr>
  <div v-if="gameAborted === true">
    <h2>You have been logged out of the game by the administrator. Thank you for playing.</h2>
  </div>

  <div v-if="gameAborted != true">
    <div class="row" v-if="!ptcChatEnabled && ptpChatEnabled">

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

        <div class='col-lg-5 col-md-5 col-sm-5 col-xs-5'>

            <gameBoard1></gameBoard1>

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

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

        <div class="col-lg-4 col-md-4 col-sm-4 col-xs-4">
          <ptpChatDisplay></ptpChatDisplay>
        </div>

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>


    </div>

    <div class="row" v-else-if="ptcChatEnabled && !ptpChatEnabled">

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

        <div class="col-lg-4 col-md-4 col-sm-4 col-xs-4">
          <ptcChatDisplay></ptcChatDisplay>
        </div>

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

        <div class='col-lg-5 col-md-5 col-sm-5 col-xs-5'>

            <gameBoard1></gameBoard1>
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

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

    </div>

    <div class="row" v-else-if ="!ptcChatEnabled && !ptpChatEnabled">
        <div class="col-lg-2 col-md-2 col-sm-2 col-xs-2"></div>

        <div class='col-lg-8 col-md-8 col-sm-8 col-xs-8'>

            <gameBoard1></gameBoard1>
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

        <div class="col-lg-2 col-md-2 col-sm-2 col-xs-2"></div>
    </div>

    <div class="row" v-else>

        <div class="col-lg-3 col-md- col-sm-3 col-xs-3">
          <ptcChatDisplay></ptcChatDisplay>
        </div>

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

        <div class='col-lg-4 col-md-4 col-sm-4 col-xs-4'>

            <gameBoard1></gameBoard1>
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

        <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

        <div class="col-lg-3 col-md-3 col-sm-3 col-xs-3">
          <ptpChatDisplay></ptpChatDisplay>
        </div>

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
        isAborted: false,

      }
    },
    created: function() {
      this.updateGame();

    },
    computed: {
      player1: function() {
        if (this.$store.getters.whichPlayer == "0"){
          return true;
        }
        else {
          return false;
        }
      },

      gameState: function() {
        return this.$store.getters.gameState;
      },

      ptcChatEnabled: function(){
        if(this.$store.getters.coachChatID == -1){
          return false;
        }
        else{
          return true;
        }
      },

      ptpChatEnabled: function(){
        return this.$store.getters.ptpChatEnabled;
      },


      isLastRound: function(){
        return this.$store.getters.currentRound==this.$store.getters.numberOfRounds;
      },

      gameAborted: function(){
        return this.$store.getters.gameAborted;
      },

      name: function(){
        return this.$store.getters.user.name;
      },

    },
    methods: {
      updatePTPchatEnabled: function(){
        var timer = setInterval(()=>{
          this.ptpChatDisplay = this.$store.getters.ptpChatEnabled;
        }, 1000);
      },
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
