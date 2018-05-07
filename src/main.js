import Vue from 'vue'
import swal from 'sweetalert'
import App from './App'
import router from './router'
<<<<<<< HEAD
import store from './store';
=======
import store from './store'
import chatDisplay from './components/chatDisplay'
>>>>>>> jspencer1


<<<<<<< HEAD
Vue.config.productionTip = false
Vue.component('game-board')
=======
Vue.component('chatDisplay',chatDisplay);
>>>>>>> jspencer1
/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  store,
  render: h => h(App)
})
