<style scoped>
  ul {
    list-style: none;
  }

  li {
    background: #fff;
    width: 500px;
    min-height: 30px;
    padding: 10px;
    margin-bottom: 10px;
    font-size: 2vh;
    display: flex;
    align-items: center;
  }

  .noBorder{
    border: 0;
    box-shadow: none;
  }

  .msgsBox{
    overflow-y: scroll;
    height: 65vh;

  }
</style>

<template>
  <div class="container">
    <h3> Objective: Coach your player to win as much money as possible. </h3>
    <hr>

    <div class="row">


      <div class="col-lg-5 col-md-5 col-sm-5 col-xs-5">
        <chatDisplay></chatDisplay>
      </div>

      <div class="col-lg-2 col-md-2 col-sm-2 col-xs-2"></div>

      <div class='col-lg-5 col-md-5 col-sm-5 col-xs-5'>

          <game-board/>
          <earnings/>
          <br>
          <button class="btn btn-success" v-show="isLastRound" @click="gotoEndGame()">Finish Game</button>
      </div>

    </div>

  </div>
</template>

<script>
import axios from 'axios'
export default{
  data() {
    return {
      title: 'Chat Display',
      msgText: '',
      messages: [],
      role: 'coach',
    }
  },
  created: function(){
    this.update();
  },
  computed: {
    chatMsgs: function(){
      this.getChatMsgs();
      return this.messages;
    },
    isLastRound: function(){
        return this.$store.getters.gameState==='isLastRound';
      },
  },
  methods: {
  getChatMsgs: function(){
    axios.get('/api/coachChat').then(response => {
      this.messages = response.data;
      return true;
    }).catch (err => {
    });
  },

  addChatMsg: function(){
    axios.post('/api/coachChat/', {
      text: this.msgText,
      role: this.role,
    }).then(response => {
      this.msgText = '',
      this.getChatMsgs();
      return true;
    }).catch(err => {
    });
  },
  update(){
    this.$store.dispatch('updateCoach');
  },
  gotoEndGame: function(){
        this.$store.dispatch('gotoEndGame');
      },
  },

}
</script>
