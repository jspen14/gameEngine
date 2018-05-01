<style scoped>
.header{
  font-size: 5vh;
}
.subHeader{
  font-size: 4vh;
}
.roleOptions{
  font-size: 3vh;
}
</style>

<template>
<div class = "container">
  <div class='header'>
    Welcome to the Strata
    <hr>
  </div>
  <div class = "subHeader">
    Select Your User:
  </div>

  <div class = "row">
    <div class="col-lg-5 col-md-5 roleOptions">
      <router-link to="player1Playground">Player 1 </router-link>
      <hr>
    </div>

    <div class="col-lg-2 col-md-2"></div>

    <div class="col-lg-5 col-md-5 roleOptions">
      <router-link to="player2Playground">Player 2</router-link>
      <hr>
    </div>
  </div>

  <br>

  <div class = "row">
  <div class="col-lg-5 col-md-5 roleOptions">
    <router-link to="coach1Playground">Coach 1 </router-link>
  </div>

    <div class="col-lg-2 col-md-2"></div>

    <div class="col-lg-5 col-md-5 roleOptions">
      <router-link to="coach2Playground">Coach 2</router-link>
    </div>
  </div>
</div>
</template>
