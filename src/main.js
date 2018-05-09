import Vue from 'vue'
import swal from 'sweetalert'
import App from './App'
import router from './router'
import store from './store'
import chatDisplay from './components/chatDisplay'
import gameBoard from './components/GameBoard'
import earnings from './components/Earnings'



Vue.config.productionTip = false

Vue.component('chatDisplay',chatDisplay);
Vue.component('gameBoard', gameBoard);
Vue.component('earnings',earnings);
/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  store,
  render: h => h(App),
  beforeCreate() {
  	this.$store.dispatch('initialize');
  }
})
