<style>
  ul {
    list-style: none;
  }
  li {
    background: #fff;
    width: 500px;
    min-height: 30px;
    padding: 10px;
    margin-bottom: 10px;
    font-size: 2vh;
    display: flex;
    align-items: center;
  }
  .header{
    font-size: 5vh;
  }
  .subHeader{
    font-size: 4vh;
  }
</style>

<template>
  <div class="container">
    <div class='header'>
      <h3>Objective: As a player/coach team, win as much money as possible.</h3>
      <hr>
    </div>

    <div class = "subHeader">
      Enter Your Name:

    </div>

    <br>

    <div class="input-group input-group-lg">
      <div class="input-group-prepend">
        <router-link to='/waitingPage'><button class="btn btn-success" v-on:click="login">Submit</button></router-link>
      </div>
      <input v-model="name" type="text" class="form-control" aria-label="Large" aria-describedby="inputGroup-sizing-sm">
    </div>

  </div>
</template>


<script>
import axios from 'axios'
export default{
  data() {
    return {
      name: '',
      role: 'player',
    }
  },
  computed: {

  },
  methods: {
    login: function() {
      this.$store.dispatch('register', {name: this.name, role: this.role});
    },
  },
}

</script>
