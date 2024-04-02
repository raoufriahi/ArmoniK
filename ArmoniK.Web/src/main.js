import 'core-js/stable'
import Vue from 'vue'
import CoreuiVue from '@coreui/vue/src'
import App from './App'
import router from './router/index'
import { iconsSet as icons } from './assets/icons/icons.js'
import {message} from './assets/js/resetMessage';
import store from './store'
import './assets/icon/iconfont.css'//font
import $ from 'jquery'

import './assets/fonts/iconfont.css'//font
import './assets/fonts/resetFontFamily.css'//font

import './assets/newicon/iconfont.css'//font
import axios from '@/http';
Vue.prototype.$http = axios;

//Register to the vue prototype
import clipboard from 'clipboard';
Vue.prototype.clipboard = clipboard;

import ElementUI from 'element-ui'
ElementUI.Dialog.props.closeOnClickModal.default=false;
import 'element-ui/lib/theme-chalk/index.css'
import locale from 'element-ui/lib/locale/lang/en'
Vue.use(ElementUI, { locale })


import VueI18n from 'vue-i18n'
import LangEn from '../static/lang/en'
import LangZhCHS from '../static/lang/zhchs'
import LangZhCHT from '../static/lang/zhcht'
Vue.use(VueI18n)
const bus = new Vue()
// console.log(store.state.lang,"mian")
import event from './views/public/event'
Vue.prototype.EVENT = event

import realEvent from './views/public/realTimeEvent'
// Vue.prototype.realEvent = realEvent

Vue.prototype.$message = message;

const i18n = new VueI18n({
  locale:'en',
  messages: {
    'en': LangEn,
    'zhchs': LangZhCHS,
    'zhcht': LangZhCHT
  }
})
i18n.locale = store.state.lang

Vue.use(CoreuiVue)


router.beforeEach((to, from, next) => {
    const type = to.meta.type
    console.log(store.state.root,type)
    
    // Determine whether the route requires login permission
    if (store.state.root!=null) {
		if (store.state.token) {
			if(store.state.root==='Administrator'){
				next()
			}else{
				if(store.state.root === 'Operator' && type==='Operator'){
					next()  // Make sure that next() must be called
				}else{
					sessionStorage.removeItem('mcutoken')
					store.state.token = null
					sessionStorage.removeItem('mcuuser')
					store.state.user = null
					sessionStorage.removeItem('mcuroot')
					store.state.root = null
					next('/login')
				}
			}
			
		} else {
			sessionStorage.removeItem('mcutoken')
			store.state.token = null 
			sessionStorage.removeItem('mcuuser')
			store.state.user = null
			sessionStorage.removeItem('mcuroot')
			store.state.root = null
			next('/login')
		}
    } else {
		console.log('No authentication required')
		next()
    }
})

new Vue({
  el: '#app',
  i18n,
  router,
  store,
  //CIcon component documentation: https://coreui.io/vue/docs/components/icon
  icons,
  template: '<App/>',
  data: {
    bus
  },
  components: {
    App
  }
})
