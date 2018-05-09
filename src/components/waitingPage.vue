<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
      {{userID.userID}}

      <div v-if="inGameStatus == true">
        <!-- If we need to pass things to new component, we can send parameters in router-link -->
        <router-link to="/playerPlayground">
          <button class="btn btn-lg btn-success">
            Continue to Game!
          </button>

        </router-link>


      </div>

      {{inGameStatus}}

</div>
</template>

<script>
export default{
  data() {
    return {
      inGameStatus: 'false',
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
