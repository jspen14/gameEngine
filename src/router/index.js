import Vue from 'vue'
import Router from 'vue-router'
import homepage from '@/components/homepage.vue'
import playerPlayground from '@/components/playerPlayground'
import coachPlayground from '@/components/coachPlayground'

import admin from '@/components/admin'
import waitingPage from '@/components/waitingPage'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'homepage',
      component: homepage,
    },
    {
      path: '/playerPlayground',
      name: 'playerPlayground',
      component: playerPlayground,
    },
    {
      path: '/coachPlayground',
      name: 'coachPlayground',
      component: coachPlayground,
    },
    {
      path: '/admin',
      name: 'admin',
      component: admin,
    },
    {
      path: '/waitingPage',
      name: 'waitingPage',
      component: waitingPage,
    }

  ]
})
