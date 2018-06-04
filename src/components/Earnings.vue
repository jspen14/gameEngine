<template>
  <div class="earnings">
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
        <h4>Avg. </h4>
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
</template>
<script>
  export default {
    name: 'Earnings',

    computed: {
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
          else {
            return Math.round((this.$store.getters.totalEarnings/(this.$store.getters.currentRound-1))*100)/100;
          }
      }
    }
  }
</script>
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
