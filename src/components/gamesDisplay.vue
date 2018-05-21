<template>
<div class="container">
    <h2> {{title}} </h2>
    <router-link to="gamesDisplay"><button class="btn btn-warning btn-sm" name="button">View All Games Currently in Progress</button></router-link>
    <hr>

    <div v-for="activeGame in activeGames" class="gamesDisplay">
      <h4><strong>Game ID:</strong> {{activeGame._gameID}} &nbsp &nbsp <strong>Round:</strong> {{activeGame._currentRound}}/{{activeGame._numberOfRounds}}</h4>
      <button v-on:click="removeActiveGame(activeGame)" class="btn btn-sm btn-danger">Remove from List</button>
      <hr>
      <h5><strong>Player 1:</strong> {{activeGame._player1Name}} &nbsp&nbsp <strong>Coach 1:</strong> {{activeGame._coach1Name}}</h5>
      <h6>Round Earnings: &nbsp&nbsp&nbsp&nbsp Average: &nbsp&nbsp&nbsp&nbsp Total:</h6>
      <hr>
      <h5><strong>Player 2:</strong> {{activeGame._player2Name}} &nbsp&nbsp <strong>Coach 2:</strong> {{activeGame._coach2Name}}</h5>
      <h6>Round Earnings: &nbsp&nbsp&nbsp&nbsp Average: &nbsp&nbsp&nbsp&nbsp Total:</h6>
    </div>
</div>

</template>

<script>
import axios from 'axios'
export default {
  data() {
    return {
      title: 'Current Games Page',
      activeGamesData: [],
    }
  },
  created: function(){
    this.getActiveGames();
    this.updateData();

  },
  computed: {
    activeGames: function(){
      return this.activeGamesData;

    }
  },
  methods: {
    updateData: function(){
      let timerID = setInterval(() => {
        this.getActiveGames();

      }, 3000);
    },

    getActiveGames: function(){
      axios.get('/api/gameModels').then(response => {
        this.activeGamesData = response.data.activeGames.slice();
      }).catch(err => {
        console.log("Error in getActiveGames on gamesDisplay Component: ", err);
      })
    },

    removeActiveGame: function(activeGame){
      console.log(activeGame);
      if(activeGame._currentRound > 3){
        swal("Success! Game is stored in Database.", {
        });
        axios.delete('/api/game/:', activeGame.gameID);
      }
      else {
        swal({
          title: "Are you sure?",
          text: "Game is not yet completed.",
          buttons: true,
          dangerMode: true,
        })
        .then((willDelete) => {
          if (willDelete) {
            swal("Success! Game was removed.", {
              icon: "success",
            });
            axios.delete('/api/game/:', activeGame.gameID);
          } else {
            swal("The show goes on!");
          }
        });
      }
    },

  },
}

</script>

<style>
.gamesDisplay{
  border: 1px solid #BABABA;
  border-radius: 5px;
  max-width: 40vw;

}
</style>
