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
  <div>
    <h3> Objective: As a player/coach team, win as much money as possible. </h3>

    <form v-on:submit.prevent="addChatMsg" class="form-control noBorder">

      <div class="input-group mb-3">
        <div class="input-group-prepend">
          <button class="btn btn-success" type="submit">Submit</button>
        </div>
        <input type="text" v-model="msgText" class="form-control" placeholder="Your Message" aria-describedby="basic-addon1">
      </div>
      <hr>

    </form>

    <div class = "msgsBox">
      <ul>
        <li v-for="msg in chatMsgs">
          {{msg.role}}: {{msg.text}}
        </li>
      </ul>
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
  computed: {
    chatMsgs: function(){
      this.getChatMsgs();
      return this.messages;
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
  },

}
</script>
