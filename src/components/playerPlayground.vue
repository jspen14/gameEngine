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

<template>

  <div class="container">
    <h3> {{name}} Objective: Win as much money as possible. </h3>
    <hr>
    <div class="row">


      <div class="col-lg-5 col-md-5 col-sm-5 col-xs-5">
        <chatDisplay></chatDisplay>
      </div>

      <div class="col-lg-2 col-md-2 col-sm-2 col-xs-2"></div>

      <div class='col-lg-5 col-md-5 col-sm-5 col-xs-5'>

          <!-- <gameBoard></gameBoard> -->
          <earnings></earnings>

      </div>
    </div>
  </div>


</template>

<script>
import axios from 'axios'

  export default {
    name: 'playerPlayground',

    data () {
      return {
        roundNumber: 1,
        roundEarnings: 0,
        averageEarnings: 0,
        totalEarnings: 0,
        playerOption: '',
        msgText: '',
        messages: [],

      }
    },

    created: function() {
      this.getMatrix();
      this.updateData();
    },

    computed: {
      name: function(){
        return this.$store.getters.user.name;
      }
    },

    methods: {
      updateData: function(){
        let timerID = setInterval(() => {
          //Poll the store to see if there are any new messages

        }, 3000);
      },

      getChatMsgs: function(){
        axios.get('/api/p1coachChat').then(response => {
          this.messages = response.data;
          return true;
        }).catch (err => {
        });
      },

      addChatMsg: function(){
        axios.post('/api/p1coachChat/', {
          text: this.msgText,
          role: this.role,
        }).then(response => {
          this.msgText = '',
          this.getChatMsgs();
          return true;
        }).catch(err => {
        });
      },
      getMatrix: function(){
          this.$store.dispatch('getMatrix', this.$store.getters.roundID);
          console.log(this.$store.getters.matrix);
      },



  }
}
</script>
