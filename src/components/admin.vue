<style>
  ul {
    list-style: none;
  }
  li {
    background: #fff;
    display: flex;
    padding-bottom: .75vh;
    align-items: left;
  }
  .btn2{
    border:none;
    background-color: inherit;
  }
  .btn2:hover{
    background: #eee;
  }
  .userDisplay{
    max-height: 55vh;
    min-height: 55vh;
  }
  .vertSpacer1{
    max-height: 5vh;
    min-height: 5vh;
  }
  .selectedUsers{
    float: left;
    clear: left;
  }
</style>

<template>
  <div class="container">

  <br>
  <h2> {{title}} </h2>
  <hr>

  <div class="row">
          <div class='col-lg-1 col-md-1 col-sm-1 col-xs-1'></div>

    <div class="col-lg-4 col-md-4 col-sm-4 col-xs-4 userDisplay ">
      <h3>Available Players</h3>
      <hr>
      <ul>
        <li v-for="player in players">
          <button class="btn btn-primary" v-on:click="setPlayer(player.name)">
            <h5>
              {{player.name}}
            </h5>
          </button>
        </li>
      </ul>
    </div>

    <div class='col-lg-2 col-md-2 col-sm-2 col-xs-2 userDisplay'>
      <button class="btn btn-warning" v-on:click="updateAvailableUsers()">Update Users</button>
    </div>

    <div class="col-lg-4 col-md-4 col-sm-4 col-xs-4 userDisplay ">
      <h3>Available Coaches</h3>
      <hr>
      <ul>
        <li v-for="coach in coaches">
          <button class="btn btn-primary" v-on:click="setCoach(coach.name)">
            <h5>{{coach.name}}</h5>
          </button>
        </li>
      </ul>
    </div>

          <div class='col-lg-1 col-md-1 col-sm-1 col-xs-1'></div>
  </div>
    <hr>
  <div class="vertSpacer1"></div>

  <div class="row">
          <div class='col-lg-1 col-md-1 col-sm-1 col-xs-1'></div>

      <div class='col-lg-4 col-md-4 col-sm-4 col-xs-4 '>
        <h4>Selected Players</h4>


          <h5 class="selectedUsers">Player 1: {{selectedPlayer1}}</h5>
          <h5 class="selectedUsers">Player 2: {{selectedPlayer2}}</h5>

      </div>

          <div class='col-lg-2 col-md-2 col-sm-2 col-xs-2'>
            <br>
            <button type="button" class="btn btn-danger" v-on:click="resetSelection()">Reset Selection</button>
            <br><br>
            <button type="button" class="btn btn-success" v-on:click="createGame()">Create Game</button>


          </div>

      <div class='col-lg-4 col-md-4 col-sm-4 col-xs-4 '>
        <h4>Selected Coaches</h4>


          <h5 class="selectedUsers">Coach 1: {{selectedCoach1}}</h5>
          <h5 class="selectedUsers"> Coach 2: {{selectedCoach2}}</h5>

      </div>

          <div class='col-lg-1 col-md-1 col-sm-1 col-xs-1'></div>

  </div>

  </div>
</template>

<script>
import axios from 'axios'
export default{
  data() {
    return {
      title: 'Admin Page',
      users: [],
      selectedPlayer1: '', // This should eventually be a playerID that will go to the server and return a name
      selectedCoach1: '', // This should eventually be a playerID that will go to the server and return a name
      selectedPlayer2: '', // This should eventually be a playerID that will go to the server and return a name
      selectedCoach2: '', // This should eventually be a playerID that will go to the server and return a name
    }
  },
  computed: {
    coaches: function(){
      let coachArray = [];
      for(let i = 0; i < this.users.length; i++){
        if(this.users[i].role == "coach"){
          coachArray.push(this.users[i]);
        }
      }
      return coachArray;
    },
    players: function(){
      let playerArray = [];
      for(let i = 0; i < this.users.length; i++){
        if(this.users[i].role == "player"){
          playerArray.push(this.users[i]);
        }
      }
      return playerArray;
    },
  },
  methods: {
    setPlayer: function(playerName){
      if(playerName == this.selectedPlayer1 || playerName == this.selectedPlayer2){
        return;
      }

      if (this.selectedPlayer1 === ''){
        this.selectedPlayer1 = playerName;
      }
      else{
        this.selectedPlayer2 = playerName;
      }
    },
    setCoach: function(coachName){
      if(coachName != 'No Coach'){
        if(coachName == this.selectedCoach1 || coachName == this.selectedCoach2){
          return;
        }
      }
      if (this.selectedCoach1 === ''){
        this.selectedCoach1 = coachName;
      }
      else{
        this.selectedCoach2 = coachName;
      }
    },
    resetSelection: function(){
      this.selectedPlayer1 = '';
      this.selectedPlayer2 = '';
      this.selectedCoach1 = '';
      this.selectedCoach2 = '';
    },
    createGame: function(){
      //STEPS:
      if (this.selectedPlayer1 == '' || this.selectedPlayer2 == '' ||
          this.selectedCoach1 == '' || this.selectedCoach2 == ''){
        swal("Enter all fields.","Please enter users for both players and coaches.","warning")
      }
      else{
        //Axios call
        // For this to work properly, we need to get the playerID's from the Database
        axios.post('/api/createGame',{
          player1ID: 1,
          coach1ID: 2,
          player2ID: 3,
          coach2ID: 4,
        }).then(response => {

          this.selectedPlayer1 = '';
          this.selectedCoach1 = '';
          this.selectedPlayer2 = '';
          this.selectedCoach2 = '';
        }).catch(error => {
          console.log('createGame failed: ', error);
        });
      }
    },
    updateAvailableUsers: function(){
      axios.get('/api/availableUsers').then(response => {
        console.log("from admin: " + response.data[0].role);
        this.users =  response.data;
        console.log (this.users);
      }).catch(err => {
        console.log(err);
      })
    },

  },
}


</script>
