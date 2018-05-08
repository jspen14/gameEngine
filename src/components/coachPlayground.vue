<style scoped>
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
  <div class = "container">
    <h1>Coach 1, Welcome to the Strata</h1>
    <h6>Objective: Coach your player to win as much money as possible</h6>
    <hr>

    <div class = "row">
      <form v-on:submit.prevent="addChatMsg" class="form-control noBorder">
        <div class="input-group mb-3">
          <div class="input-group-prepend">
            <button class="btn btn-success" type="submit">Submit</button>
          </div>
          <input type="text" v-model="msgText" class="form-control" placeholder="Your Message" aria-describedby="basic-addon1">
        </div>
      </form>

      <div class = "msgsBox">
        <ul>
          <li v-for="msg in chatMsgs">
            <h5>{{msg.role}}: {{msg.text}}</h5>
          </li>
        </ul>
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
        msgText: '',
        messages: [],
        role: 'Coach',
      }
    },
    computed: {
      created: function() {
        this.getChatMsgs();
      },
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
        console.log('in add');
        axios.post('/api/coachChat', {
          text: this.msgText,
          role: this.role,
        }).then(response => {
          this.msgText = '',
          this.getChatMsgs();
          return true;
        }).catch(err => {
        });
      }
    },
  }
</script>
