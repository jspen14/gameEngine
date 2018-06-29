<template>
	<div>
		<div class="gameboard">
			<br>
      <h2>Game Board</h2>
      <hr>
      <div class = "row">
      	<div class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardHeader" ></div>
				<div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader"> <h2> A </h2> </div>
				<div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader"> <h2> B </h2> </div>
      </div>


            <div v-if="$store.getters.whichPlayer===0"> <!-- IF PLAYER 1 -->
              <div class = "row">
								<div v-if="isUnsubmitted && isPlayer" v-on:click="submitChoice(0)" @mouseenter="Row1=true" @mouseleave="Row1=false" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow unsubp1">
                  <h2>1</h2>
                </div>
                <div v-else v-bind:class="{rowX: rowActive(Row1)}" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow p1">
                  <h2>1</h2>
                </div>


  						<div v-bind:class="{selected: zero_zero, colX: colActive(Col1), rowX: rowActive(Row1)}" class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 class="p2">{{$store.getters.matrix[0][0][other]}}</h5>
                  <h5 class="p1">{{$store.getters.matrix[0][0][me]}}</h5>
                </div>

                <div v-bind:class="{selected: zero_one, colX: colActive(Col2), rowX: rowActive(Row1)}"  class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 class="p2">{{$store.getters.matrix[0][1][other]}}</h5>
                  <h5 class="p1">{{$store.getters.matrix[0][1][me]}}</h5>
                </div>


              </div>

              <div class = "row">
								<div v-if="isUnsubmitted && isPlayer" v-on:click="submitChoice(1)" @mouseenter="Row2=true" @mouseleave="Row2=false" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow unsubp1">
                	<h2>2</h2>
                </div>
                <div v-else v-bind:class="{rowX: rowActive(Row2)}" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow p1">
                  <h2>2</h2>
                </div>

                <div v-bind:class="{selected: one_zero, colX: colActive(Col1), rowX: rowActive(Row2)}"  class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 class="p2">{{$store.getters.matrix[1][0][other]}}</h5>
                  <h5 class="p1">{{$store.getters.matrix[1][0][me]}}</h5>
                </div>

                <div v-bind:class="{selected: one_one, colX: colActive(Col2), rowX: rowActive(Row2)}"  class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 class="p2">{{$store.getters.matrix[1][1][other]}}</h5>
                  <h5 class="p1">{{$store.getters.matrix[1][1][me]}}</h5>
                </div>
              </div>
            </div>

						<div v-if="$store.getters.whichPlayer===1"> <!-- IF PLAYER 1 -->
              <div class = "row">
								<div v-if="isUnsubmitted && isPlayer" v-on:click="submitChoice(0)" @mouseenter="Row1=true" @mouseleave="Row1=false" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow unsubp1">
                  <h2>1</h2>
                </div>
                <div v-else v-bind:class="{rowX: rowActive(Row1)}" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow p1">
                  <h2>1</h2>
                </div>


  						<div v-bind:class="{selected: zero_zero, colX: colActive(Col1), rowX: rowActive(Row1)}" class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 class="p2">{{$store.getters.matrix[0][0][1-me]}}</h5>
                  <h5 class="p1">{{$store.getters.matrix[0][0][1-other]}}</h5>
                </div>

                <div v-bind:class="{selected: zero_one, colX: colActive(Col2), rowX: rowActive(Row1)}"  class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
									<h5 class="p2">{{$store.getters.matrix[1][0][1-me]}}</h5>
                  <h5 class="p1">{{$store.getters.matrix[1][0][1-other]}}</h5>
                </div>


              </div>

              <div class = "row">
								<div v-if="isUnsubmitted && isPlayer" v-on:click="submitChoice(1)" @mouseenter="Row2=true" @mouseleave="Row2=false" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow unsubp1">
                	<h2>2</h2>
                </div>
                <div v-else v-bind:class="{rowX: rowActive(Row2)}" class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow p1">
                  <h2>2</h2>
                </div>

                <div v-bind:class="{selected: one_zero, colX: colActive(Col1), rowX: rowActive(Row2)}"  class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
									<h5 class="p2">{{$store.getters.matrix[0][1][1-me]}}</h5>
									<h5 class="p1">{{$store.getters.matrix[0][1][1-other]}}</h5>
                </div>

                <div v-bind:class="{selected: one_one, colX: colActive(Col2), rowX: rowActive(Row2)}"  class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5 class="p2">{{$store.getters.matrix[1][1][1-me]}}</h5>
                  <h5 class="p1">{{$store.getters.matrix[1][1][1-other]}}</h5>
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
    data(){
      return{
        Col1: false,
        Col2: false,
        Row1: false,
        Row2: false,
      }
    },
    created: function(){
      this.getMatrix();
    },
    computed: {

      isUnsubmitted: function(){
        if(this.$store.getters.gameState==="unsubmitted"){
					return true;
				}
				else if(this.$store.getters.gameState==="isReady"){
						this.deactivate();

				}

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
      },
      isPlayer: function(){
        if(this.$store.getters.user.role=="Player")
          return true;
        return false;
      }
    },
	   methods: {
      colActive: function(col){
        let which=this.$store.getters.whichPlayer;
        if(which===1 && col)
          return true;
        return false;
      },
      rowActive: function(row){
        let which= this.$store.getters.whichPlayer;
        if(which===0 && row)
          return true;
        return false;
      },
      activate: function(row, col){
        if(row===0)
          this.Row1=true;
        else
          this.Row2=true;
        if(col===0)
          this.Col1=true;
        else
          this.Col2=true;
      },
      deactivate:function(row,col){
          this.Row1=false;
          this.Row2=false;
          this.Col1=false;
          this.Col2=false;
      },
			submitChoice: function(choice){
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
    background: gold !important;
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
  h2{
    line-height: 90px;
  }
.p2{
    color: #007bff;
}
.p1{
  color:#28a745;
}

.unsubp1{
  transition: background-color 0.2s ease;
  color: white;
  background: #28a745;
}

.unsubp1:hover{
  background: #186329;
}
.unsubp2{
  transition: background-color 0.2s ease;
  color: white;

}
.unsubp2:hover{
  background: #0057B5;
}
  .boardRow{
    min-height: 10vh;
    border: solid 1px black;
  }
.colX{
  background-color: #ABEFFF;
}

.rowX{
  background-color: #B9FFCB;
}

</style>
