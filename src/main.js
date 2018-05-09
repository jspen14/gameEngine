import Vue from 'vue'
import swal from 'sweetalert'
import App from './App'
import router from './router'
import store from './store'
import chatDisplay from './components/chatDisplay'




Vue.config.productionTip = false
Vue.component('game-board')

Vue.component('chatDisplay',chatDisplay);

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
