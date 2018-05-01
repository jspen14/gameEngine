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

  .boardHeader{
    min-height: 5vh;
    border: solid 1px black;
  }

  .boardRow{
    min-height: 10vh;
    border: solid 1px black;
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
    <h1> Player 1, Welcome to the Strata</h1>
    <h6> Objective: Win as much money as possible </h6>
    <hr>
    <div class="row">


      <div class="col-lg-5 col-md-5 col-sm-5 col-xs-5">
        <br>

        <form v-on:submit.prevent="addChatMsg" class="form-control noBorder">

          <h2>Chat with Your Coach</h2>
          <br>

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

      <div class="col-lg-2 col-md-2 col-sm-2 col-xs-2"></div>

      <div class='col-lg-5 col-md-5 col-sm-5 col-xs-5'>
          <br>
          <h2>Game Board</h2>
          <hr>
          <div class = "row">
            <div class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardHeader" >

            </div>

            <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader">
              <h2>A</h2>
            </div>

            <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader">
              <h2>B</h2>
            </div>
          </div>


            <div>
              <div class = "row">
                <div class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow" >
                  <button class="btn btn-success" v-on:click="submitOption(1)"><h2>1</h2></button>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: + $.10</h5>
                  <h5> Opponent: + $.10</h5>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: - $.02</h5>
                  <h5> Opponent: + $.07</h5>
                </div>
              </div>

              <div class = "row">
                <div class = "col-lg-2 col-md-2 col-sm-5 col-xs-5 boardRow" >

                  <button class="btn btn-success" v-on:click="submitOption(2)"><h2>2</h2></button>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: + $.07</h5>
                  <h5> Opponent: - $.02</h5>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: + $.05</h5>
                  <h5> Opponent: + $.05</h5>
                </div>
              </div>
            </div>


          <br><br>
          <div class="row">
            <div class="col-lg-4 col-md-4"></div>
            <div class="col-lg-4 col-md-4">
              <h3>Earnings </h3>
              <hr>
            </div>
            <div class="col-lg-4 col-md-4"></div>
          </div>

          <div class="row">
            <div class="col-lg-4 col-md-4 boardHeader">
              <h4>Round </h4>
            </div>
            <div class="col-lg-4 col-md-4 boardHeader">
              <h4>Average </h4>
            </div>
            <div class="col-lg-4 col-md-4 boardHeader">
              <h4>Total </h4>
            </div>
          </div>
          <div class="row">
            <div class="col-lg-4 col-md-4 boardHeader">
              <h4>  ${{roundEarnings}}</h4>
            </div>
            <div class="col-lg-4 col-md-4 boardHeader">
              <h4> ${{averageEarnings}} </h4>
            </div>
            <div class="col-lg-4 col-md-4 boardHeader">
              <h4> ${{totalEarnings}}</h4>
            </div>
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
        roundNumber: 1,
        roundEarnings: 0,
        averageEarnings: 0,
        totalEarnings: 0,
        playerOption: '',
        msgText: '',
        messages: [],
        role: 'Player',

      }
    },
    created: function() {
      this.getChatMsgs();
      this.getRoundEarnings();
      this.getAverageEarnings();
      this.getTotalEarnings();
    },
    computed: {
      chatMsgs: function(){
        this.getChatMsgs();
        return this.messages;
      },

    },
    methods: {
      submitOption: function(param){
      console.log(param);
        axios.put('/api/p1roundOption',{
          roundOption: param,
        });
        this.getRoundEarnings();
        this.getAverageEarnings();
        this.getTotalEarnings();
      },

      getRoundEarnings: function(){
        axios.get('/api/p1roundEarnings').then(response => {
          this.roundEarnings = response.data;
          return true;
        });
        //.catch (err => {});
      },

      getAverageEarnings: function(){
        axios.get('/api/p1averageEarnings').then(response => {
          this.averageEarnings = response.data.toFixed(2);
          return true;
        });
        //.catch (err => {});
      },

      getTotalEarnings: function(){
        axios.get('/api/p1totalEarnings').then(response => {
          this.totalEarnings = response.data;
          this.totalEarnings = this.totalEarnings.toFixed(2);
          return true;
        });
        //.catch (err => {});
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
      }
    },
  }
</script>
