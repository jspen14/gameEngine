<template>
<div class="container">
    <h1>Games Currently in Progress</h1>
    <hr>

    <div v-for="activeGame in activeGames" class="gamesDisplay">
      <h4><strong>Game ID:</strong> {{activeGame._gameID}} &nbsp &nbsp <strong>Round:</strong> {{activeGame._currentRound}}/{{activeGame._numberOfRounds}}</h4>
      <hr>
      <h5><strong>Player 1 ID:</strong> {{activeGame._player1}} &nbsp&nbsp <strong>Coach 1 ID:</strong> {{activeGame._coach1}}</h5>
      <h6>Round Earnings: &nbsp&nbsp&nbsp&nbsp Average: &nbsp&nbsp&nbsp&nbsp Total:</h6>
      <hr>
      <h5><strong>Player 2 ID:</strong> {{activeGame._player2}} &nbsp&nbsp <strong>Coach 2 ID:</strong> {{activeGame._coach2}}</h5>
      <h6>Round Earnings: &nbsp&nbsp&nbsp&nbsp Average: &nbsp&nbsp&nbsp&nbsp Total:</h6>
    </div>
</div>

</template>

<script>
import axios from 'axios'
export default {
  data() {
    return {
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
        console.log(this.activeGamesData);
      }).catch(err => {
        console.log("Error in getActiveGames on gamesDisplay Component: ", err);
      })
    },
  },
}
</script>

<style>
.gamesDisplay{
  border: .5px solid #BABABA;
  border-radius: 5px;
  max-width: 40vw;

}
</style>
