<template>
	<div>
		<h1>Thank You for Playing!</h1>
		 <button class="btn btn-success" v-on:click="logout()">Logout</button>
	</div>
</template>
<script>
import axios from 'axios'
export default {
	name: 'endGamePage',
	methods:{
	    logout: function(){
	    this.$store.dispatch('endGame');
    	}
    }
}
</script>