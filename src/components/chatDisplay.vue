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
    <h2> {{title}} </h2>
    <hr>

    <div class = "msgsBox">
      <div v-for="msg in chatMsgs">
        <div v-if="isCoach(msg.role)">
          <div class="coachMsgWrap">
            <h6 class="coachMsgDisplay"> &nbsp {{msg.text}} &nbsp </h6>
            <br><br>
          </div>

        </div>

        <div v-if="isPlayer(msg.role)">
          <div class="playerMsgWrap">
            <h6 class="playerMsgDisplay"> &nbsp {{msg.text}} &nbsp</h6>
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
  computed: {
    chatMsgs: function(){
      this.getChatMsgs();
      return this.messages;
    },
  },
  methods: {
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

    getChatMsgs: function(){
      axios.get('/api/coachChat').then(response => {
        this.messages = response.data;
        return true;
      }).catch (err => {
      });
      console.log("hey" + this.messages);
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
  },

}
</script>
