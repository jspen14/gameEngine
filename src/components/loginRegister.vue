<template>
<div class = "container">
  <div class='header'>
    <h3>Objective: As a player/coach team, win as much money as possible.</h3>
    <hr>
  </div>
  <div class = "subHeader">
    Select Your User:
  </div>
  <br>
    <div class = "row">
    <div class="col-lg-3 col-md-3">
    </div>

  <div class="col-lg-6 col-md-6">
    <ul class="nav nav-tabs">
      <li v-bind:class= "{active: isregister}" v-on:click="isregister=true" class="nav-link "><a>Register</a></li>
      <li v-bind:class= "{active: !isregister}" v-on:click="isregister=false" class="nav-link ">Login</a></li>
    </ul>
    <br>

    <form>
      <div v-show="isregister" class="form-group row">
        <div class="col-2 col-form-label">Role</div>
        <div class="col-10">
          <select  class="form-control"  v-model="role" required>
            <option>Player</option>
            <option>Coach</option>
          </select>
        </div>
      </div>
    <div class="form-group row">
      <div class="col-2 col-form-label">Name</div>
      <div class="col-10">
        <input type="text" class="form-control"  placeholder= "first and last name" v-model="name" required>
      </div>
    </div>
    <div class="form-group row">
      <div class="col-2 col-form-label">Password</div>
      <div class="col-10">
        <input type="password" class="form-control"  placeholder= "password" v-model="password" required>
      </div>
    </div>
    <button v-if="isregister" v-on:click="registerUser()" type="submit" class="btn btn-primary">Register</button>
    <button v-else  v-on:click="login()" type="submit" class="btn btn-primary">Log In</button>

  </form>
  </div>
  <div class="col-lg-3 col-md-3">
  </div>
</div>



</div>
</template>
<script>
  import axios from 'axios'
export default{
  name: 'LoginRegister',
  data() {
    return {
      name: '',
      role: 'Player',
      password: '',
      isregister: true,

    }
  },
  computed: {
    registerError: function(){
      return this.$store.getters.registerError;
    },
    loginError: function(){
      return this.$store.getters.loginError;
    }
  },

  methods: {
    registerUser: function() {
      this.$store.dispatch('register', {name: this.name, role: this.role, password: this.password});
      if(this.$store.getters.registerError!='')
        swal("Error: ",this.$store.getters.registerError).then(()=>{this.$store.dispatch('closeErrors','');});
    },

    login: function(){
      console.log({name: this.name, password:this.password});
      this.$store.dispatch('login', {name: this.name, password:this.password});

      if(this.$store.getters.loginError!=''){
        swal("Error: ",this.$store.getters.loginError).then(()=>{this.$store.dispatch('closeErrors','');});
        console.log("BANANA");
      }
      console.log("PEAR");
    },

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
