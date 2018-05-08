<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
      {{userID.userID}}
      {{inGame}}

</div>
</template>

<script>
export default{
  data() {
    return {
      inGame: 'false',
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

  },
  methods: {
    updateData: function(){
      let timerID = setInterval(() => {
        this.inGame = this.$store.getters.inGame;
          if(this.inGame == "true"){
            console.log("success");
          }
      }, 1500);
    },

  },


}
</script>
