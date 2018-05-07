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
    <h1> Objective: Win as much money as possible </h1>
    <hr>
    <div class="row">


      <div class="col-lg-5 col-md-5 col-sm-5 col-xs-5">
        <chatDisplay></chatDisplay>
      </div>

      <div class="col-lg-2 col-md-2 col-sm-2 col-xs-2"></div>

      <div class='col-lg-5 col-md-5 col-sm-5 col-xs-5'>
<<<<<<< HEAD:src/components/player1Playground.vue
=======
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
                  <h5>You: {{p1payout[0]}}</h5>
                  <h5> Opponent: {{p2payout[0]}}</h5>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: {{p1payout[1]}}</h5>
                  <h5> Opponent: {{p2payout[1]}}</h5>
                </div>
              </div>

              <div class = "row">
                <div class = "col-lg-2 col-md-2 col-sm-5 col-xs-5 boardRow" >

                  <button class="btn btn-success" v-on:click="submitOption(2)"><h2>2</h2></button>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: {{p1payout[2]}}</h5>
                  <h5> Opponent: {{p2payout[2]}}</h5>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: {{p1payout[3]}}</h5>
                  <h5> Opponent: {{p2payout[3]}}</h5>
                </div>
              </div>
            </div>

>>>>>>> jspencer1:src/components/playerPlayground.vue

          <game-board></game-board>
          <earnings></earnings>

      </div>
    </div>
  </div>


</template>

<script>
import axios from 'axios'
import GameBoard from './GameBoard'
import Earnings from './Earnings'
  export default {
    name: 'playerPlayground',
    components: {GameBoard, Earnings},
    data () {
      return {
        roundNumber: 1,
        roundEarnings: 0,
        averageEarnings: 0,
        totalEarnings: 0,
        playerOption: '',
        msgText: '',
        messages: [],
<<<<<<< HEAD:src/components/player1Playground.vue
        role: 'Player',

      }
    },

    created: function() {
      //this.getChatMsgs();
      //this.getRoundEarnings();
      //this.getAverageEarnings();
      //this.getTotalEarnings();
      this.getMatrix();


    },
=======
        p1payout: [1,2,3,4],
        p2payout: [5,6,7,8],

      }
    },
>>>>>>> jspencer1:src/components/playerPlayground.vue
    computed: {

    },
    methods: {
<<<<<<< HEAD:src/components/player1Playground.vue
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
      },
      getMatrix: function(){
          this.$store.dispatch('getMatrix', this.$store.getters.roundID);
          console.log(this.$store.getters.matrix);
      },


=======
      getRandomPayouts: function() {
       axios.post("/api/payouts", {}).then(response => {
      this.p1payout=response.data[0];
      this.p2payout=response.data[1];

      return true;
       }).catch(err => {
       });
     },
      getPayouts: function() {
             axios.get("/api/payouts/1").then(response => {
         this.p1payout= response.data;
             }).then(response => {
              this.p2payout=response.data;
             }).catch(err => { console.log("error: getPayoutsj failed!")
             });
          },
>>>>>>> jspencer1:src/components/playerPlayground.vue
  }
}
</script>
