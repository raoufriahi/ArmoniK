import Vue from 'vue'
import Vuex from 'vuex'
Vue.use(Vuex)

const state = {
  videoid:'',
  sidebarShow: 'responsive',
  sidebarMinimize: false,
  darkMode: 'c-dark-theme',
  asideShow: false,
  asideShows:false,
  liveviewrtc:'WS',
  liveviewrtc1:'RTC',
  deviceprintcolor:'',
  strNodeName:9090,
  switch:false,
  // tourrtc:"WS",
  watermarkstring:"linkingvision",
  watermarktoggle:"linkingvision",
  Certificatetime:null,//证书时间 是否显示水印
  oldstrPasswd:'',
  devicemarktoggle:'block',
  Ai_devicemarktoggle:'none',
  lang:'en',
  token:null,
  user:null,
  root:null,
  Adswitch:"false",
  IPPORT:'',
  WSROOT:'',
  IOTEVENTROOT:12222,
  FullScreenId:'',
  liveviewadd:[],
  Recviewadd:[],
  Mapurl:null,
  liveplay:{
    token:null,
    streamprofile:null,
    name:null,
    label:null,
    vid:null,
    viewparameter:null
  },
  layoutplay:{
    token:null,
    streamprofile:null,
    name:null,
    label:null,
    vid:null,
  },
  offVideo:'',
  stopTurePlay:true,
  trueplay:{
    token:null,
    streamprofile:null,
    vid:null,
  },
  drag:{
    drags:false,
    one:{
        strId:'',
        strIndex: "",
        strStream: "",
        strToken: "",
        name:''
    },
    two:{
        strId:'',
        strIndex: "",
        strStream: "",
        strToken: "",
        name:''
    }
  },
  dragid:'',
  dataview:{},
  jiaohuan:'',
  e1:undefined,
  gEvent:[],
  e2:undefined
}

const mutations = {
  toggleSidebarDesktop (state) {
    const sidebarOpened = [true, 'responsive'].includes(state.sidebarShow)
    state.sidebarShow = sidebarOpened ? false : 'responsive'
  },
  toggleSidebarMobile (state) {
    const sidebarClosed = [false, 'responsive'].includes(state.sidebarShow)
    state.sidebarShow = sidebarClosed ? true : 'responsive'
  },
  set (state, [variable, value]) {
    state[variable] = value
  },
  toggle (state, variable) {
    state[variable] = !state[variable]
  }
}

export default new Vuex.Store({
  state,
  mutations
})