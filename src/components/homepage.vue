<template>
<div class = "container">
  <div v-if="loggedIn">
    <waiting-page/>
  </div>
  <div v-else>
    <login-register/>
  </div>
</div>
</template>
<script>
  import axios from 'axios'
  import loginRegister from './loginRegister'
  import waitingPage from './waitingPage'
export default{
  name: 'homepage',
  components: {loginRegister,waitingPage},
  computed: {
    loggedIn:function () {
      return this.$store.getters.loggedIn;
    }
  },

}
</script>
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
