import Vue from 'vue'
import Router from 'vue-router'
import homepage from '@/components/homepage.vue'
import playerPlayground from '@/components/playerPlayground'
import coachPlayground from '@/components/coachPlayground'
import playerLogin from '@/components/playerLogin'
import coachLogin from '@/components/coachLogin'
import admin from '@/components/admin'

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
      path: '/playerLogin',
      name: 'playerLogin',
      component: playerLogin,
    },
    {
      path: '/coachLogin',
      name: 'coachLogin',
      component: coachLogin,
    },
    {
      path: '/admin',
      name: 'admin',
      component: admin,
    }

  ]
})
