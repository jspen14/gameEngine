<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>

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

</div>
</template>

<script>
export default{
  data() {
    return {
      inGameStatus: '',

    }
  },
  created: function(){
    this.updateData();
    this.$store.dispatch('updateData');
  },
  computed: {
    userID: function(){
      return this.$store.getters.userID;
    },
    name: function(){
      return this.$store.getters.name;
    },
    inGame: function(){
      if(this.$store.getters.inGameStatus == true){
        return true;
      }
      else {return false;}
    },
    role: function(){
      return this.$store.getters.role;
    },

  },
  methods: {
    updateData: function(){
      let timerID = setInterval(() => {
        this.inGameStatus = this.$store.getters.inGameStatus;

      }, 3000);
    },

  },


}
</script>
