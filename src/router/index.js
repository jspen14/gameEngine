import Vue from 'vue'
import Router from 'vue-router'
import homepage from '@/components/homepage.vue'
import player1Playground from '@/components/player1Playground.vue'
import coach1Playground from '@/components/coach1Playground.vue'
import player2Playground from '@/components/player2Playground.vue'
import coach2Playground from '@/components/coach2Playground.vue'



Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'homepage',
      component: homepage,
    },
    {
      path: '/player1Playground',
      name: 'player1Playground',
      component: player1Playground,
    },
    {
      path: '/coach1Playground',
      name: 'coach1Playground',
      component: coach1Playground,
    },
    {
      path: '/player2Playground',
      name: 'player2Playground',
      component: player2Playground,
    },
    {
      path: '/coach2Playground',
      name: 'coach2Playground',
      component: coach2Playground,
    },
  ]
})
