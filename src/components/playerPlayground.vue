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
    <h3>  Objective: Win as much money as possible. </h3>
    <h3>  User: {{name}} </h3>
    <hr>
    <div v-if="gameAborted === true">
      <h2>You have been logged out of the game by the administrator. Thank you for playing.</h2>
    </div>

    <div v-if="gameAborted != true" class="row">

      <div class="col-lg-3 col-md-3 col-sm-3 col-xs-3">
        <div v-if="hasCoach">
          <ptcChatDisplay></ptcChatDisplay>
        </div>
        <div v-else>
          <br><br><br><hr>
          <img src="http://www.civilorganizations.com/wp-content/uploads/2017/07/0bf2a93.jpg" class="img-fluid">
          <br><br>
          <h4>Unfortunately, your coach didn't show up to work today. :/</h4>
        </div>

      </div>

      <div class="col-lg-1 col-md-1 col-sm-1 col-xs-1"></div>

      <div class='col-lg-4 col-md-4 col-sm-4 col-xs-4'>
        <div v-if="player1">
          <gameBoard1></gameBoard1>
        </div>
        <div v-else>
          <gameBoard2></gameBoard2>
        </div>
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

      <!-- <div v-if="ptpChatEnabled"> -->
        <div v-show="ptpChatEnabled " class="col-lg-3 col-md-3 col-sm-3 col-xs-3">
            <ptpChatDisplay></ptpChatDisplay>

        </div>
      <!-- </div> -->

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
        ptpChatEnabled: '',
      }
    },
    created: function() {
      this.updateGame()
      .then(this.$store.dispatch('retrievePTPChatEnabled'))
      .then(this.ptpChatEnabled = this.$store.getters.ptpChatEnabled);
    },
    computed: {
      player1: function() {
        console.log("Which player: " + this.$store.getters.whichPlayer);
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

      hasCoach: function(){
        if(this.$store.getters.coachChatID == -1){
          return false;
        }
        else{
          return true;
        }

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
