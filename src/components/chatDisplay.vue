<style>
  ul {
    list-style: none;
  }
  li {
    background: #fff;
    width: 500px;
    min-height: 30px;
    padding: 10px;
    margin-bottom: 10px;
    font-size: 1.5vh;
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
    word-wrap: break-word;
  }
  .coachMsgWrap{
    float: left;
    clear:left;
    max-width: 75%;
    min-width: 75%;
  }
  .coachMsgDisplay{
    float: left;
    clear:left;
    border-radius: 6px;
    background-color: #2165DA;
    color: white;
  }
  .playerMsgWrap{
    float: right;
    clear:right;
    max-width: 75%;
    min-width: 75%;
  }
  .playerMsgDisplay{
    float: right;
    clear:right;
    border-radius:6px;
    background-color:#2AA745;
    color: white;
  }
</style>

<template>
  <div>
    <br>
    <h2>{{name}}'s {{title}} </h2>

    <hr>

    <div class = "msgsBox" id = "chatDisplayContainer">
      <div v-for="msg in chatMsgs">


        <div v-if="!isMe(msg.userID)">
          <div class="coachMsgWrap">
            <h6 class="coachMsgDisplay"> &nbsp {{msg.message}} &nbsp </h6>
            <br><br>
          </div>

        </div>

        <div v-else>
          <div class="playerMsgWrap">
            <h6 class="playerMsgDisplay"> &nbsp {{msg.message}} &nbsp</h6>
            <br><br>
          </div>

        </div>

      </div>
    </div>

    <form v-on:submit.prevent="addChatMsg" class="form-control noBorder">

      <div class="input-group mb-3">
        <input type="text" v-model="msgText" class="form-control" placeholder="Your Message" aria-describedby="basic-addon1">
        <div class="input-group-prepend">
          <button class="btn btn-success" style="border-bottom-right-radius: 5px; border-top-right-radius: 5px" type="submit">Submit</button>
        </div>
      </div>


    </form>

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
      role: '',
    }
  },
  created: function(){
    this.updateData();
    //this.$store.dispatch('updateData');
    //this.$store.dispatch('getCoachChatID'); // I'm not sure If I need to do this
  },
  computed: {
    name: function(){
      return this.$store.getters.user.name;
    },
    chatMsgs: function(){
      return this.$store.getters.coachChatMsgs;
    },
  },
  methods: {
    updateData: function(){
      let timerID = setInterval(() => {
        // JSpencer update calls
        //this.overflowScroll();
        this.$store.dispatch('getCoachChatID');
        if(this.$store.getters.coachChatMsgsSize = 0){
          this.$store.dispatch('getCoachChatMsgsSize'); // this still needs to be tested
        }
        if(this.$store.getters.coachChatMsgs.length > 0){ //this is the problem
          this.$store.dispatch('getCoachChatMsgs');
        }
      }, 3000);
    },
    test: function(){
      this.$store.dispatch('getCoachChatID');
    },
    isMe: function(msgUserID){
      if(msgUserID == this.$store.getters.user.id){
        console.log("success");
        return true;
      }
      else {
        return false;
      }
    },
    isCoach: function(type){
      if (type == "coach"){
        return true;
      }
      else {
        return false;
      }
    },
    isPlayer: function(type){
      if (type == "player"){
        return true;
      }
      else {
        return false;
      }
    },
    addChatMsg: function(){
      this.$store.dispatch('addChatMsg', {
        text: this.msgText,
      });
      this.msgText = '';
      this.$store.dispatch('getCoachChatMsgs');
    },
  },
}
</script>
