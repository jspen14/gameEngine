<template>
	<div class="gameboard">
		<br>
          <h2>Game Board</h2>
          <hr>
          <div class = "row">
            <div class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardHeader" >

            </div>

            <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader">
              <h2>A</h2>
            </div>

            <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardHeader">
              <h2>B</h2>
            </div>
          </div>


            <div>
              <div class = "row">
                <div class = "col-lg-2 col-md-2 col-sm-2 col-xs-2 boardRow" >
                  <button class="btn btn-success" v-on:click="submitChoice(1)"><h2>1</h2></button>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: {{$store.getters.matrix[0][0][0]}}</h5>
                  <h5> Opponent: {{$store.getters.matrix[0][0][1]}}</h5>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: {{$store.getters.matrix[0][1][0]}}</h5>
                  <h5> Opponent: {{$store.getters.matrix[0][1][1]}}</h5>
                </div>
              </div>

              <div class = "row">
                <div class = "col-lg-2 col-md-2 col-sm-5 col-xs-5 boardRow" >

                  <button class="btn btn-success" v-on:click="submitChoice(2)"><h2>2</h2></button>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: {{$store.getters.matrix[1][0][0]}}</h5>
                  <h5> Opponent: {{$store.getters.matrix[1][0][1]}}</h5>
                </div>

                <div class = "col-lg-5 col-md-5 col-sm-5 col-xs-5 boardRow">
                  <br>
                  <h5>You: {{$store.getters.matrix[1][1][0]}}</h5>
                  <h5> Opponent: {{$store.getters.matrix[1][1][1]}}</h5>
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
		data (){
			return {

			}
		},
		created (){
		
			
		},
		computed: {


		},
	   methods: {
			submitChoice: function(choice){
        console.log("selected ",choice);
        
        this.$store.dispatch("submitChoice", choice);
				
			},
      
		},

	}
</script>
<style scoped>

	.gameboard{
		margin:none;
		}
	  .boardHeader{
	
    min-height: 5vh;
    border: solid 1px black;
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