<template>
	<div>
		<h1>Thank You for Playing!</h1>
		<h3 v-if='isPlayer'>You won ${{totalEarnings}}</h3>
		<h3 v-else>You helped your player win ${{totalEarnings}}</h3>
		 <button class="btn btn-success" v-on:click="logout()">Logout</button>
	

	</div>
</template>
<script>
import axios from 'axios'
export default {
	name: 'endGamePage',
	computed:{
		totalEarnings: function(){
			return this.$store.getters.totalEarnings;
		},
		isPlayer: function(){
			return this.$store.getters.user.role==='Player';
		}

	},
	methods:{
	    logout: function(){
	    this.$store.dispatch('endGame');
    	},


    }
}
</script>