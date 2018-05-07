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
        p1payout: [1,2,3,4],
        p2payout: [5,6,7,8],

      }
    },
    computed: {

    },
    methods: {
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
  }
}
</script>
