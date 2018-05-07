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
</template>
<script>
  export default {
    name: 'Earnings',
    data() {
      return {
      roundEarnings: 0,
      averageEarnings: 0,
      totalEarnings: 0,
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