<template>
	<div class="gameboard">
		<br>
          <h2>Game Board</h2>
          <hr>
          <div class = "row">
            <div class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardHeader" >

            </div>

            <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader">
              <h2 v-if="$store.getters.whichPlayer===0" class="p1">A</h2>
              <div v-else>
                <button v-if="isUnsubmitted" class="btn btn-primary" v-on:click="submitChoice(0)"><h2>A</h2></button>
                <h2 v-else class="p1">A</h2>
             </div>
            </div>

            <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader">
              <h2 v-if="$store.getters.whichPlayer===0" class="p1">B</h2>
              <div v-else>
                <button v-if="isUnsubmitted" class="btn btn-primary" v-on:click="submitChoice(1)"><h2>B</h2></button>
                <h2 v-else class="p1">B</h2>
             </div>
            </div>
          </div>


            <div>
              <div class = "row">
                <div class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow" >
                  <h2 class="p0" v-if="$store.getters.whichPlayer===1">1</h2>
                  <div v-else>
                    <button v-if="isUnsubmitted" class="btn btn-success" v-on:click="submitChoice(0)"><h2>1</h2></button>
                    <h2 v-else class="p0">1</h2>
                  </div>
                </div>

                <div v-bind:class="{selected: zero_zero}" class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 v-bind:class="{p0:isPlayer0, p1: isPlayer1}">{{$store.getters.matrix[0][0][me]}}</h5>
                  <h5 v-bind:class="{p1:isPlayer0, p0: isPlayer1}">{{$store.getters.matrix[0][0][other]}}</h5>
                </div>

                <div v-bind:class="{selected: zero_one }" class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 v-bind:class="{p0:isPlayer0, p1: isPlayer1}">{{$store.getters.matrix[0][1][me]}}</h5>
                  <h5 v-bind:class="{p1:isPlayer0, p0: isPlayer1}">{{$store.getters.matrix[0][1][other]}}</h5>
                </div>
              </div>

              <div class = "row">
                <div class = "col-lg-2 col-md-2 col-sm-5 col-xs-5 boardRow" >

                  <h2 class="p0" v-if="$store.getters.whichPlayer===1">2</h2>
                  <div v-else>
                    <button v-if="isUnsubmitted" class="btn btn-success" v-on:click="submitChoice(1)"><h2>2</h2></button>
                    <h2 v-else class="p0">2</h2>
                  </div>
                </div>

                <div v-bind:class="{selected: one_zero}" class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 v-bind:class="{p0:isPlayer0, p1: isPlayer1}">{{$store.getters.matrix[1][0][me]}}</h5>
                  <h5 v-bind:class="{p1:isPlayer0, p0: isPlayer1}">{{$store.getters.matrix[1][0][other]}}</h5>
                </div>

                <div v-bind:class="{selected: one_one}" class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 v-bind:class="{p0:isPlayer0, p1: isPlayer1}">{{$store.getters.matrix[1][1][me]}}</h5>
                  <h5 v-bind:class="{p1:isPlayer0, p0: isPlayer1}">{{$store.getters.matrix[1][1][other]}}</h5>
                </div>
              </div>
            </div>

      </div>
    </div>

</template>
<script>
  import axios from 'axios'
	export default {
		name: 'GameBoard',
    created: function(){
      this.getMatrix();
    },
    computed: {
      isUnsubmitted: function(){
        if(this.$store.getters.gameState==="unsubmitted")
          return true;
        return false;
      },
      me: function(){
        return this.$store.getters.whichPlayer;
      },
      other: function(){
        let player=this.$store.getters.whichPlayer;
        if(player===1)
          return 0;
        return 1;
      },
      isPlayer0: function(){
        if(this.$store.getters.whichPlayer===0)
          return true;
        return false;
      },
      isPlayer1: function(){
        if(this.$store.getters.whichPlayer===1)
          return true;
        return false;
      },
      zero_zero: function(){
        let p1Choice= this.$store.getters.p1Choice;
        let p2Choice= this.$store.getters.p2Choice;
        if(p1Choice===0 && p2Choice===0)
          return true;
        return false;
      },
        zero_one: function(){
        let which=this.$store.getters.whichPlayer;
        let p1Choice= this.$store.getters.p1Choice;
        let p2Choice= this.$store.getters.p2Choice;
        if(p1Choice===0 && p2Choice===1) 
        {
          return true;
        }
        return false;
      },
      one_zero: function(){
        let which=this.$store.getters.whichPlayer;
        let p1Choice= this.$store.getters.p1Choice;
        let p2Choice= this.$store.getters.p2Choice;
        if(p1Choice===1 && p2Choice===0)
        {
          return true;
        }
        return false;
      },
      one_one: function(){
        let p1Choice= this.$store.getters.p1Choice;
        let p2Choice= this.$store.getters.p2Choice;
        if(p1Choice===1 && p2Choice===1)
          return true;
        return false;
      }
    },
	   methods: {
			submitChoice: function(choice){
        console.log("selected ",choice);
        
        this.$store.dispatch("submitChoice", choice);
				

			},
      getMatrix: function(){
          this.$store.dispatch('getMatrix', this.$store.getters.currentRound);
      },
		},

	}
</script>
<style scoped>
  .selected{
    background: #7FDBFF;
  }
  div[data-col='1']{
    background: red;
  }
	.gameboard{
		margin:none;
		}
	  .boardHeader{
	
    min-height: 5vh;
    border: solid 1px black;
  }
  .p1{
    color: #007bff;
}
.p0{
  color:#28a745;
}
  .boardRow{
    min-height: 10vh;
    border: solid 1px black;
  }
  .row{
  	border:solid 1px black;
  }
  .box{
  	border:solid 1px black;
  }
</style>