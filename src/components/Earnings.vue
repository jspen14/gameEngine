<template>
  <div class="earnings">
    <br>
    <br>
    <br>
    <hr>

      <h4>Your Average Earnings: $ {{averageEarnings}} </h4>
      <br>
      <h4>Your Total Earnings: ${{totalEarnings}}</h4>

      <hr>
  </div>
</template>
<script>
  export default {
    name: 'Earnings',

    computed: {
      gameState: function() {
        return this.$store.getters.gameState;
      },
      roundEarnings: function(){
        return this.$store.getters.roundEarnings;
      },
      totalEarnings: function(){
        return this.$store.getters.totalEarnings;
      },
      averageEarnings: function(){
          if(this.$store.getters.currentRound===1 || this.$store.getters.currentRound === 0){
            return this.$store.getters.totalEarnings;
          }
          else if(this.$store.getters.gameState === 'done' || this.$store.getters.gameState === 'isReady'){
            return Math.round((this.$store.getters.totalEarnings/(this.$store.getters.currentRound))*100)/100
          }
          else {
            return Math.round((this.$store.getters.totalEarnings/(this.$store.getters.currentRound-1))*100)/100;
          }
      },
      unsubmitted: function(){
        if(this.$store.getters.gameState != "unsubmitted"){
          return false;
        }
        return true;
      },
    }
  }
</script>
<style scoped>
  .earnings{
    text-align: left;
    color: #0064F9;
  }
</style>
