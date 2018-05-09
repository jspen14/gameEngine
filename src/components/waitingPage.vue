<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
<<<<<<< HEAD

      <div v-if="inGameStatus == true">
        <!-- If we need to pass things to new component, we can send parameters in router-link -->
        <div v-if="role == 'player'">
          <router-link to="/playerPlayground">
            <button class="btn btn-lg btn-success">
              Continue to Game
            </button>

          </router-link>
        </div>

        <div v-if="role == 'coach'">
          <router-link to="/coachPlayground">
            <button class="btn btn-lg btn-success">
              Continue to Game
            </button>

          </router-link>
        </div>

      </div>

=======
      {{userID}}

      <div v-if="inGameStatus == true">
        <!-- If we need to pass things to new component, we can send parameters in router-link -->
        <router-link to="/playerPlayground">
          <button class="btn btn-lg btn-success">
            Continue to Game!
          </button>

        </router-link>


      </div>

      {{inGameStatus}}

  <br>
  <img src="/static/images/ajax-loader.gif"/>
  <br>
  <button v-on:click="logout()" class="btn btn-primary">Log out</button>

>>>>>>> 700154ea0a2761953e2e0b149d943d4fbd3345e5
</div>
</template>

<script>
export default{
  data() {
    return {
<<<<<<< HEAD
      inGameStatus: '',

=======
      inGameStatus: 'false',
>>>>>>> 700154ea0a2761953e2e0b149d943d4fbd3345e5
    }
  },
  created: function(){
    this.updateData();
    this.$store.dispatch('updateData');
  },
  computed: {
<<<<<<< HEAD
    userID: function(){
      return this.$store.getters.userID;
    },
    name: function(){
      return this.$store.getters.name;
=======
    name: function(){
      return this.$store.getters.user.name;
    },
    userID: function(){
      return this.$store.getters.user.id;
>>>>>>> 700154ea0a2761953e2e0b149d943d4fbd3345e5
    },
    inGame: function(){
      if(this.$store.getters.inGameStatus == true){
        return true;
      }
      else {return false;}
    },
<<<<<<< HEAD
    role: function(){
      return this.$store.getters.role;
    },
=======
>>>>>>> 700154ea0a2761953e2e0b149d943d4fbd3345e5

  },
  methods: {
    updateData: function(){
      let timerID = setInterval(() => {
        this.inGameStatus = this.$store.getters.inGameStatus;

      }, 3000);
    },
<<<<<<< HEAD

  },
=======
    logout: function(){
      this.$store.dispatch('logout');
    }
  },
  
>>>>>>> 700154ea0a2761953e2e0b149d943d4fbd3345e5


}
</script>
