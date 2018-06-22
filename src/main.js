import Vue from 'vue'
import swal from 'sweetalert'
import App from './App'
import router from './router'
import store from './store'
import ptcChatDisplay from './components/ptcChatDisplay'
import ptpChatDisplay from './components/ptpChatDisplay'
import gameBoard1 from './components/GameBoard1'
import gameBoard2 from './components/GameBoard2'
import earnings from './components/Earnings'



Vue.config.productionTip = false

Vue.component('ptcChatDisplay',ptcChatDisplay);
Vue.component('gameBoard1', gameBoard1);
Vue.component('gameBoard2', gameBoard2); // This is where Elizabeth should look to change the code for the display
Vue.component('earnings',earnings);
Vue.component('ptpChatDisplay',ptpChatDisplay);
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
