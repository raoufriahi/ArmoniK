import Vue from 'vue'
import Router from 'vue-router'
import VueI18n from 'vue-i18n'
import store from '../store'
import LangEn from '../../static/lang/en'
import LangZhCHS from '../../static/lang/zhchs'
import LangZhCHT from '../../static/lang/zhcht'

Vue.use(VueI18n)
const i18n = new VueI18n({
    locale: 'en',
      fallbackLocale: 'en',
      localeDir: 'locales',
      enableInSFC: true,
    messages: {
      'en': LangEn,
    }
})
if(sessionStorage.getItem('mculang')){
	store.state.lang=sessionStorage.getItem('mculang');
}
i18n.locale = store.state.lang
// Containers
const TheContainer = () => import('@/containers/TheContainer')
const Login = () => import('@/containers/Login')
const Logout = () => import('@/containers/Logout')
const Downloadapp = () => import('@/containers/Downloadapp')

// Views
const Dashboard = () => import('@/views/Dashboard')
const Liveview = () => import('@/views/Liveview')
const Replay = () => import('@/views/Replay')
const Avintercom = () => import('@/views/Avintercom')
const Tour = () => import('@/views/Tour')
const Settings = () => import('@/views/Settings')
const GB = () => import('@/views/GB')
const Clouds = () => import('@/views/Clouds')
const Event = () => import('@/views/Event')
const Storage = () => import('@/views/storaged/Storage')
const StorageConfig = () => import('@/views/storaged/StorageConfig')
const StorageTerminal = () => import('@/views/storaged/StorageTerminal')

const Device = () => import('@/views/dashboard/Device')
const GPU = () => import('@/views/dashboard/GPU')

const GB28181 = () => import('@/views/GB/GB28181')
const GBplatform = () => import('@/views/GB/GBplatform')
const GBService = () => import('@/views/GB/GBService')

const Advancepb = () => import('@/views/replay/Advancepb')
const Archive = () => import('@/views/replay/Archive')
const Playback = () => import('@/views/replay/Playback')
const Snapshot = () => import('@/views/replay/Snapshot')

const Configure = () => import('@/views/settings/https/Configure')
const Domainlist = () => import('@/views/settings/https/Domainlist')

const Client = () => import('@/views/settings/Client/Client')
const License = () => import('@/views/settings/license/License')
const Cluster = () => import('@/views/settings/cluster/Cluster')
const Map = () => import('@/views/settings/Map/Map')
const Mapmanagement = () => import('@/views/settings/Map/Mapmanagement')
const DeviceImport = () => import('@/views/settings/Batch/BatchImport')
const Camera = () => import('@/views/settings/Camera/Camera')
const DeviceStream = () => import('@/views/settings/device/DeviceStream')
const DeviceOnvif = () => import('@/views/settings/device/DeviceOnvif')
const DeviceFile = () => import('@/views/settings/device/DeviceFile')
const DeviceAll = () => import('@/views/settings/device/DeviceAll')
const Docker = () => import('@/views/settings/docker/Docker')
const Storagepage = () => import('@/views/settings/storagedconfig/storageconfig')
const RTSP = () => import('@/views/settings/Protocol/RTSP')
const DeviceSdk = () => import('@/views/settings/SDK/DeviceSdk')
const User = () => import('@/views/settings/user/User')
const Role = () => import('@/views/settings/user/Role')
const CyberSecurity = () => import('@/views/settings/user/CyberSecurity')
const Default = () => import('@/views/settings/Transcoding/Default')
const Profile = () => import('@/views/settings/Transcoding/Profile')
const CloudMode = () => import('@/views/settings/WEBRTC/CloudMode')
const RelayMode = () => import('@/views/settings/WEBRTC/RelayMode')
const TURNServer = () => import('@/views/settings/WEBRTC/TURNServer')
const Record = () => import('@/views/settings/record/Record')
const LogLevel = () => import('@/views/settings/log/LogLevel')
const LogRotating = () => import('@/views/settings/log/LogRotating')
const System = () => import('@/views/settings/system/System')
const RtmpPush = () => import('@/views/settings/RTMP/RtmpPush')
const CloudConnect = () => import('@/views/settings/CloudConnect/CloudConnect')
const Edit = () => import('@/views/settings/region/Edit')
const Region = () => import('@/views/settings/region/Region')

const Avintercoms = () => import('@/views/avintercom/Avintercoms')
const Upload = () => import('@/views/avintercom/Upload')

const RealTimeEvent = () => import('@/views/event/RealTimeEvent')
const EventSearch = () => import('@/views/event/EventSearch')
const EventDebug = () => import('@/views/event/EventDebug')
const EventVQD = () => import('@/views/event/EventVQD')
const EventAI = () => import('@/views/event/EventAI')
const Database = () => import('@/views/settings/database/database')

//高级系统设置
const Advanced = () => import('@/views/Advanced')
const Storages = () => import('@/views/advanced/storage')
const Terminals = () => import('@/views/advanced/terminals')
const Systems = () => import('@/views/advanced/systems')
const metrics = () => import('@/views/advanced/metrics')
const Network = () => import('@/views/advanced/network')
const Information = () => import('@/views/advanced/information')
const Class = () => import('@/views/advanced/class')
const advaiclass = () => import('@/views/advanced/ai')

const Layout = () => import('@/views/layout')
const SinglePlayBack = () => import('@/views/SinglePlayBack')

//vqd 
const Vqd = ()=> import('@/views/settings/AI/vqd')
const Ai = ()=> import('@/views/settings/AI/ai')
const AiChannel = ()=> import('@/views/settings/AI/AiChannel')
const Advai = ()=> import('@/views/settings/AI/advai')
//推流
const PF = () => import('@/views/PushFlow')
const RTMP = () => import('@/views/PF/RTMP')
const WHIP = () => import('@/views/PF/WHIP')

//转码
const transcoding = () => import('@/views/transcoding')
const RTS = () => import('@/views/transcoding/rts')
const ServerConf = () => import('@/views/transcoding/ServerConf')
const TransGPU = () => import('@/views/transcoding/TransGPU')
const GPUTranscoding = () => import('@/views/transcoding/GPUTranscoding')
const Nvidia = () => import('@/views/transcoding/Nvidia')
const WinIntel = () => import('@/views/transcoding/WinIntel')
const LinuxIntel = () => import('@/views/transcoding/LinuxIntel')

// PlayCode播放码
const PlayCode = () => import('@/views/settings/user/PlayCode')
Vue.use(Router)

export default new Router({
	mode: 'hash', // https://router.vuejs.org/api/#mode
	linkActiveClass: 'open active',
	scrollBehavior: () => ({ y: 0 }),
	routes: [
		{path:"/",redirect:'/Login'},
		{
			path: '/',
			name: 'Home',
			component: TheContainer,
			meta: {
                type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
            },
			children: [
				{
					path: 'Dashboard',
					name: 'Dashboard',
					component: Dashboard,
					meta: {
						title: '/Dashboard',
						name:i18n.tc("message.left.dashboard"),
						icon:'iconfont icon-yibiaopan',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'Layout',
					name: 'Layout',
					component: Layout,
					meta: {
						title: '/Layout',
						icon:'iconfont icon-11111-copy',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'transcoding',
					name: 'transcoding',
					component: transcoding,
					meta: {
						title: '/transcoding/RTS',
						name:i18n.tc("message.left.transcoding"),
						icon:'iconfont icon-zhuanma',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children: [
						{
							path: 'ServerConf',
							name: 'ServerConf',
							component: ServerConf,
							meta: {
								title: 'ServerConf',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'RTS',
							name: 'RTS',
							component: RTS,
							meta: {
								title: 'RTS',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'TransGPU',
							name: 'TransGPU',
							component: TransGPU,
							meta: {
								title: 'TransGPU',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'GPUTranscoding',
							name: 'GPUTranscoding',
							component: GPUTranscoding,
							meta: {
								title: 'GPUTranscoding',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'WinIntel',
							name: 'WinIntel',
							component: WinIntel,
							meta: {
								title: 'WinIntel',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'LinuxIntel',
							name: 'LinuxIntel',
							component: LinuxIntel,
							meta: {
								title: 'LinuxIntel',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Nvidia',
							name: 'Nvidia',
							component: Nvidia,
							meta: {
								title: 'Nvidia',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
					]
				},{
					path: 'SinglePlayBack',
					name: 'SinglePlayBack',
					component: SinglePlayBack,
					meta: {
						title: '/SinglePlayBack',
						icon:'iconfont icon-11111-copy',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'Event',
					name: 'Event',
					component: Event,
					redirect:'/Event/RealTimeEvent',
					meta: {
						title: 'Event/RealTimeEvent',
						name:i18n.tc("message.left.event"),
						icon:'iconfont icon-lingdang',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children: [
						{
							path: 'RealTimeEvent',
							name: 'RealTimeEvent',
							component: RealTimeEvent,
							meta: {
								title: 'RealTimeEvent',
								name:i18n.tc("message.event.RealTimeEvent"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'EventSearch',
							name: 'EventSearch',
							component: EventSearch,
							meta: {
								title: 'EventSearch',
								name:i18n.tc("message.event.EventSearch"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'EventDebug',
							name: 'EventDebug',
							component: EventDebug,
							meta: {
								title: 'EventDebug',
								name:i18n.tc("message.event.EventDebug"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
						,{
							path: 'EventVQD',
							name: 'EventVQD',
							component: EventVQD,
							meta: {
								title: 'EventVQD',
								name:i18n.tc("message.event.EventVQD"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'EventAI',
							name: 'EventAI',
							component: EventAI,
							meta: {
								title: 'EventAI',
								name:i18n.tc("message.event.EventAI"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
					]
				},{
					path: 'Device',
					name: 'Device',
					component: Device,
					meta: {
						title: 'Device',
						name:i18n.tc("message.dashboard.device"),
						icon:'iconfont icon-11111-copy',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'GPU',
					name: 'GPU',
					component: GPU,
					meta: {
						title: 'GPU',
						name:'GPU',
						icon:'iconfont icon-11111-copy',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'Liveview',
					name: 'Liveview',
					component: Liveview,
					meta: {
						title: '/Liveview',
						name:i18n.tc("message.live.liveview"),
						icon:'iconfont icon-shishishipin',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'Clouds',
					name: 'Clouds',
					component: Clouds,
					meta: {
						title: '/Clouds',
						name:i18n.tc("message.left.cloud"),
						icon:'iconfont icon-yunshipin',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'Map',
					name: 'Map',
					component: Map,
					meta: {
						title: '/Map',
						name:i18n.tc("message.left.Map"),
						icon:'iconfont icon-ditu',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'Tour',
					name: 'Tour',
					component: Tour,
					meta: {
						title: '/Tour',
						name:i18n.tc("message.left.tour"),
						icon:'iconfont icon-xungeng',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
				},{
					path: 'Replay',
					name: 'Replay',
					component: Replay,
					meta: {
						title: '/Replay/Advancepb',
						name:i18n.tc("message.left.playback"),
						icon:'iconfont icon-huifang',
						type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children: [
						{
							path: 'Advancepb',
							name: 'Advancepb',
							component: Advancepb,
							meta: {
								title: 'Advancepb',
								name:i18n.tc("message.left.AdvancePB"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Archive',
							name: 'Archive',
							component: Archive,
							meta: {
								title: 'Archive',
								name:i18n.tc("message.left.AdvancePB"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Playback',
							name: 'Playback',
							component: Playback,
							meta: {
								title: 'Playback',
								name:i18n.tc("message.left.AdvancePB"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Snapshot',
							name: 'Snapshot',
							component: Snapshot,
							meta: {
								title: 'Snapshot',
								name:i18n.tc("message.left.AdvancePB"),
								icon:'iconfont icon-bofang1',
								type: 'Operator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},
						
					]
				},{
					path: 'Avintercom',
					name: 'Avintercom',
					component: Avintercom,
					meta: {
						title: '/Avintercom/Avintercoms',
						name:i18n.tc("message.Conference.Upload"),
						icon:'iconfont icon-shipinhuiyi3',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children: [
						{
							path: 'Avintercoms',
							name: 'Avintercoms',
							component: Avintercoms,
							meta: {
								title: 'Avintercoms',
								name:i18n.tc("message.Conference.Videointercom"),
								icon:'iconfont icon-bofang1',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Upload',
							name: 'Upload',
							component: Upload,
							meta: {
								title: 'Upload',
								name:i18n.tc("message.Conference.Upload"),
								icon:'iconfont icon-bofang1',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
					]
				},{
					path: 'GB',
					name: 'GB',
					component: GB,
					meta: {
						title: '/GB/GB28181',
						name:'GB',
						icon:'iconfont icon-GB',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children: [
						{
							path: 'GB28181',
							name: 'GB28181',
							component: GB28181,
							meta: {
								title: 'GB28181',
								name:i18n.tc("message.GB.GB28181"),
								icon:'iconfont icon-bofang1',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'GBplatform',
							name: 'GBplatform',
							component: GBplatform,
							meta: {
								title: 'GBPlatform',
								name:i18n.tc("message.GB.GBPlatform"),
								icon:'iconfont icon-bofang1',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'GBService',
							name: 'GBService',
							component: GBService,
							meta: {
								title: 'GBService',
								name:i18n.tc("message.GB.GBService"),
								icon:'iconfont icon-bofang1',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
					]
				},{
					path: 'PF',
					name: i18n.tc("message.left.PF"),
					component: PF,
					meta: {
						title: '/PF/RTMP',
						name:i18n.tc("message.left.PF"),
						icon:'iconfont icon-tuiliu',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children: [
						{
							path: 'RTMP',
							name: 'RTMP',
							component: RTMP,
							meta: {
								title: 'RTMP',
								name:i18n.tc("message.PF.RTMP"),
								icon:'iconfont icon-bofang1',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'WHIP',
							name: 'WHIP',
							component: WHIP,
							meta: {
								title: 'WHIP',
								name:i18n.tc("message.PF.WHIP"),
								icon:'iconfont icon-bofang1',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
					]
				},{
					path: 'Settings',
					name: 'Settings',
					component: Settings,
					meta: {
						title: '/Settings/DeviceStream',
						name:i18n.tc("message.setting.setting"),
						icon:'iconfont icon-shezhi',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children: [{
						path: 'DeviceImport',
						name: 'DeviceImport',
						component: DeviceImport,
						meta: {
							title: '/DeviceImport',
							name:i18n.tc("message.left.camera"),
							icon:'iconfont icon-jiankong',
							type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
						    }
						},{
							path: 'DeviceImport',
						name: 'DeviceImport',
						component: DeviceImport,
						meta: {
							title: '/DeviceImport',
							name:i18n.tc("message.left.database"),
							icon:'iconfont icon-jiankong',
							type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
						    }
						},{
							path: 'Database',
							name: 'Database',
							component: Database,
							meta: {
							title: '/Database',
							name:i18n.tc("message.left.Database"),
							icon:'iconfont icon-jiankong',
							type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
						    }
						},{
							path: 'Vqd',
							name: 'Vqd',
							component: Vqd,
							meta: {
								title: '/Vqd',
								name:'VQD',
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
								}
						},{
							path: 'Ai',
							name: 'Ai',
							component: Ai,
							meta: {
								title: '/Ai',
								name:'AI',
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
								}
						},{
							path: 'Advai',
							name: 'Advai',
							component: Advai,
							meta: {
								title: '/Advai',
								name:'Advai',
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
								}
						},{
							path: 'AiChannel',
							name: 'AiChannel',
							component: AiChannel,
							meta: {
								title: '/AiChannel',
								name:'AiChannel',
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
								}
						},{
							path: 'Cluster',
							name: 'Cluster',
							component: Cluster,
							meta: {
								title: '/Cluster',
								name:i18n.tc("message.dashboard.cluster"),
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Client',
							name: 'Client',
							component: Client,
							meta: {
								title: '/Client',
								name:i18n.tc("message.dashboard.cluster"),
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Configure',
							name: 'Configure',
							component: Configure,
							meta: {
								title: '/Configure',
								name:i18n.tc("message.dashboard.cluster"),
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Domainlist',
							name: 'Domainlist',
							component: Domainlist,
							meta: {
								title: '/Domainlist',
								name:i18n.tc("message.dashboard.cluster"),
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'License',
							name: 'License',
							component: License,
							meta: {
								title: '/License',
								name:"Authorization management",
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Mapmanagement',
							name: 'Mapmanagement',
							component: Mapmanagement,
							meta: {
								title: '/Mapmanagement',
								name:i18n.tc("message.left.camera"),
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Camera',
							name: 'Camera',
							component: Camera,
							meta: {
								title: '/Camera',
								name:i18n.tc("message.left.camera"),
								icon:'iconfont icon-jiankong',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'DeviceStream',
							name: 'DeviceStream',
							component: DeviceStream,
							meta: {
								title: '/DeviceStream',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'DeviceOnvif',
							name: 'DeviceOnvif',
							component: DeviceOnvif,
							meta: {
								title: '/DeviceOnvif',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'DeviceFile',
							name: 'DeviceFile',
							component: DeviceFile,
							meta: {
								title: '/DeviceFile',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'DeviceAll',
							name: 'DeviceAll',
							component: DeviceAll,
							meta: {
								title: '/DeviceAll',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Docker',
							name: 'Docker',
							component: Docker,
							meta: {
								title: '/Docker',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Storagepage',
							name: 'Storagepage',
							component: Storagepage,
							meta: {
								title: '/Storagepage',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'RTSP',
							name: 'RTSP',
							component: RTSP,
							meta: {
								title: '/RTSP',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'DeviceSdk',
							name: 'DeviceSdk',
							component: DeviceSdk,
							meta: {
								title: '/DeviceSdk',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'User',
							name: 'User',
							component: User,
							meta: {
								title: '/User',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Role',
							name: 'Role',
							component: Role,
							meta: {
								title: '/Role',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'CyberSecurity',
							name: 'CyberSecurity',
							component: CyberSecurity,
							meta: {
								title: '/CyberSecurity',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'PlayCode',
							name: 'PlayCode',
							component: PlayCode,
							meta: {
								title: '/PlayCode',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							}						
							
						},{
							path: 'Default',
							name: 'Default',
							component: Default,
							meta: {
								title: '/Default',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							}
						},{
							path: 'Profile',
							name: 'Profile',
							component: Profile,
							meta: {
								title: '/Profile',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'CloudMode',
							name: 'CloudMode',
							component: CloudMode,
							meta: {
								title: '/CloudMode',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'RelayMode',
							name: 'RelayMode',
							component: RelayMode,
							meta: {
								title: '/RelayMode',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'TURNServer',
							name: 'TURNServer',
							component: TURNServer,
							meta: {
								title: '/TURNServer',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Record',
							name: 'Record',
							component: Record,
							meta: {
								title: '/Record',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'LogLevel',
							name: 'LogLevel',
							component: LogLevel,
							meta: {
								title: '/LogLevel',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'LogRotating',
							name: 'LogRotating',
							component: LogRotating,
							meta: {
								title: '/LogRotating',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'System',
							name: 'System',
							component: System,
							meta: {
								title: '/System',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'RtmpPush',
							name: 'RtmpPush',
							component: RtmpPush,
							meta: {
								title: '/RtmpPush',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'CloudConnect',
							name: 'CloudConnect',
							component: CloudConnect,
							meta: {
								title: '/CloudConnect',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Edit',
							name: 'Edit',
							component: Edit,
							meta: {
								title: '/Edit',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'Region',
							name: 'Region',
							component: Region,
							meta: {
								title: '/Region',
								name:i18n.tc("message.left.dashboard"),
								icon:'iconfont icon-11111-copy',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
					]
				},{
					path: 'Storage',
					name: 'Storage',
					component: Storage,
					meta: {
						title: '/storage/StorageConfig',
						name:i18n.tc("message.left.storage"),
						icon:'iconfont icon-shezhi',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children:[
						{   path: 'StorageConfig',
							name: 'StorageConfig',
							component: StorageConfig,
							meta: {
								title: '/storage/StorageConfig',
								name:i18n.tc("message.left.storage"),
								icon:'iconfont icon-shezhi',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{
							path: 'StorageTerminal',
							name: 'StorageTerminal',
							component: StorageTerminal,
							meta: {
								title: '/storage/StorageTerminal',
								name:i18n.tc("message.left.storage"),
								icon:'iconfont icon-shezhi',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						}
					]
				},{
					path: 'Advanced',
					name: 'Advanced',
					component: Advanced,
					redirect:'/Advanced/Information',
					meta: {
						title: '/Advanced/Information',
						name:i18n.tc("message.left.advanced"),
						icon:'iconfont icon-gaojishezhi',
						type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
					},
					children:[
						{   
							path: 'Storages',
							name: 'Storages',
							component: Storages,
							meta: {
								title: '/Storages',
								name:i18n.tc("message.left.storage"),
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{ 
							path: 'Class',
							name: 'Class',
							component: Class,
							meta: {
								title: '/Class',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{ 
							path: 'advaiclass',
							name: 'advaiclass',
							component: advaiclass,
							meta: {
								title: '/advaiclass',
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{   
							path: 'Terminals',
							name: 'Terminals',
							component: Terminals,
							meta: {
								title: '/Terminals',
								name:i18n.tc("message.left.Terminals"),
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{   
							path: 'Systems',
							name: 'Systems',
							component: Systems,
							meta: {
								title: '/Systems',
								name:i18n.tc("message.left.Systems"),
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{   
							path: 'Metrics',
							name: 'Metrics',
							component: metrics,
							meta: {
								title: '/Metrics',
								name:i18n.tc("message.left.Metrics"),
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{   
							path: 'Network',
							name: 'Network',
							component: Network,
							meta: {
								title: '/Network',
								name:i18n.tc("message.left.Network"),
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},{   
							path: 'Information',
							name: 'Information',
							component: Information,
							meta: {
								title: '/Information',
								name:i18n.tc("message.left.Information"),
								type: 'Administrator'  // Whether you need to judge whether to log in or not, here is the need to judge
							},
						},
					]
				},
				
			]
		},{
			path: '/Login',
			name: 'Login',
			component: Login,
			meta: {
				title: 'login',
				type: '' // No authentication required
			}
        },{
			path: '/Downloadapp',
			name: 'Downloadapp',
			component: Downloadapp,
			meta: {
				title: 'Downloadapp',
				type: '' // No authentication required
			}
        },{
			path: '/Logout',
			name: 'Logout',
			component: Logout,
			meta: {
				title: 'Logout',
				type: '' // No authentication required
			}
        }
	]
})


if(sessionStorage.getItem('Adswitch')){
	store.state.Adswitch=sessionStorage.getItem('Adswitch');
}
if(sessionStorage.getItem('mcuroot')){
	store.state.root=sessionStorage.getItem('mcuroot');
}
if(sessionStorage.getItem('mcutoken')){
	store.state.token=sessionStorage.getItem('mcutoken');
}
if(sessionStorage.getItem('mcuuser')){
	store.state.user=sessionStorage.getItem('mcuuser');
}
if(sessionStorage.getItem('Certificatetime')){
	store.state.Certificatetime=sessionStorage.getItem('Certificatetime');
}

if(localStorage.getItem('devicemarktoggle')){
	store.state.devicemarktoggle=localStorage.getItem('devicemarktoggle');
}
if(localStorage.getItem('Ai_devicemarktoggle')){
	store.state.Ai_devicemarktoggle=localStorage.getItem('Ai_devicemarktoggle');
}
if(localStorage.getItem('Mapurl')){
	store.state.Mapurl=localStorage.getItem('Mapurl');
}
if(localStorage.getItem('liveviewrtc')){
	store.state.liveviewrtc=localStorage.getItem('liveviewrtc');
}
if(localStorage.getItem('liveviewrtc1')){
	store.state.liveviewrtc1=localStorage.getItem('liveviewrtc1');
}
if(localStorage.getItem('watermarkstring')){
	store.state.watermarkstring=localStorage.getItem('watermarkstring');
}
if(localStorage.getItem('watermarktoggle')){
	store.state.watermarktoggle=localStorage.getItem('watermarktoggle');
}
if(localStorage.getItem('strNodeName')){
	store.state.strNodeName=JSON.parse(localStorage.getItem('strNodeName'));
}
if(localStorage.getItem('switchconfig')){
	store.state.switch=JSON.parse(localStorage.getItem('switchconfig'));
	console.log(store.state.switch);
}
// 画质选择
if(localStorage.getItem('elqualitytoggle')){
	store.state.elqualitytoggle=localStorage.getItem('elqualitytoggle');
}
// if(localStorage.getItem('tourrtc')){
// 	store.state.tourrtc=localStorage.getItem('tourrtc');
// }
if(localStorage.getItem('themetoggle')){
	// console.log('主题',JSON.parse(localStorage.getItem('themetoggle')))
	store.state.darkMode=localStorage.getItem('themetoggle')=='false'?false:localStorage.getItem('themetoggle');
}

let root=process.env.VUE_APP_URL;
let rtmplink=window.location.host.split(':')
let rtmproot="rtmp://" + rtmplink[0]+':8554' + window.location.pathname+'live'
if (root == undefined){
	root = window.location.protocol + '//' + window.location.host;
}
// console.log(root)
store.state.IPPORT=root
store.state.RTMPROOT=rtmproot

console.log("host",window.location.hostname,window.location.host,window.location.href)

var wsroot = process.env.VUE_APP_PORT;
if (wsroot == undefined)
{
	wsroot = window.location.host;
}
// console.log(wsroot)
store.state.WSROOT=wsroot