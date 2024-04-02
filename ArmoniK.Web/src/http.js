/**
 * Created by superman on 17/2/16.
 * http configuration
 */

import axios from 'axios'
import store from './store'
import router from '@/router'
// http request Interceptor
axios.interceptors.request.use(
  config => {
   /* if (store.state.token) {
      config.headers.Authorization = `token ${store.state.token}`
    }*/
    return config
  },
  err => {
    return Promise.reject(err)
  }
)

// http response interceptor
axios.interceptors.response.use(
  response => {
    return response
  },
  error => {
    if (error.response) {
      switch (error.response.status) {
        case 401:
            // 401 Clear the token information and jump to the login page
            // Jump only when the current route is not the login page
            sessionStorage.removeItem('mcutoken')
            store.state.token = null
            sessionStorage.removeItem('mcuuser')
            store.state.user = null
            sessionStorage.removeItem('mcuroot')
            store.state.root = null
            router.currentRoute.path !== 'login' &&
            router.replace({
              path: '/login',
              query: { redirect: router.currentRoute.path }
            })
      }
    }
    //console.log(JSON.stringify(error));//console : Error: Request failed with status code 402
   
    return Promise.reject(error.response.data)
  }
)

export default axios
