<style scoped>
a:hover{
  text-decoration: none;
}


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
    Select Your User
    <router-link to="admin" class="subHeader" style="color:black">* </router-link>
    :
  </div>

  <br>

  <div class = "row">
    <div class="col-lg-5 col-md-5 roleOptions">
      <router-link to="playerLogin" class="subHeader">Player </router-link>

    </div>

    <div class="col-lg-2 col-md-2"></div>

    <div class="col-lg-5 col-md-5 roleOptions">
      <router-link to="coachLogin" class="subHeader">Coach</router-link>

    </div>
  </div>

  <br>

</div>
</template>
