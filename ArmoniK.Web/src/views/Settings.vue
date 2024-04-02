<template>
<div class="Setting">
	<div class="setting_all">
		<div class="setting_zuo">
			<el-menu
                router
                :default-active="activemenu" 
				class="el-menu-vertical-demo"
				:background-color="color1"
				:text-color="($store.state.darkMode) ? '#FFF' : '#303133'"
				style="height:100%">
				<el-submenu index="/Settings/Edit">
					<template slot="title">
						<i class="iconfont icon-quyuguanli"></i>
						<span>{{$t("message.setting.Regional")}} </span>
					</template>
                    <el-menu-item index="/Settings/Edit">
                        <span class="Setting_tree">{{$t("message.setting.Area")}}</span> 
                    </el-menu-item>
					<el-menu-item index="/Settings/Region">
                        <span class="Setting_tree">{{$t("message.setting.Regional")}}</span> 
                    </el-menu-item>
				</el-submenu>
				
				<!-- <el-submenu index="/Settings/Client">
					<template slot="title">
						<i class="iconfont icon-kehuduanpeizhi"></i>
						<span>{{$t("message.setting.ClientConfiguration")}}</span>
					</template>
                    <el-menu-item index="/Settings/Client">
                        <span class="Setting_tree">{{$t("message.setting.ClientConfiguration")}}</span> 
                    </el-menu-item>
				</el-submenu> -->
				<el-submenu index="/Settings/DeviceStream">
					<template slot="title">
						<i class="iconfont icon-shebei"></i>
						<span>{{$t("message.setting.device")}} </span>
					</template>
                    <el-menu-item index="/Settings/DeviceStream">
                        <span class="Setting_tree">{{$t("message.setting.RTSPRTMP")}}</span> 
                    </el-menu-item>
                    <el-menu-item index="/Settings/DeviceOnvif">
                         <span class="Setting_tree">{{$t("message.setting.ONVIF")}}</span> 
                    </el-menu-item>
					<el-menu-item index="/Settings/DeviceSdk">
                        <span class="Setting_tree">{{$t("message.setting.devicesdk")}}</span> 
                    </el-menu-item>
					<el-menu-item index="/Settings/RtmpPush">
                        <span class="Setting_tree">{{$t("message.setting.devicertmppush")}}</span> 
                    </el-menu-item>
					<el-menu-item index="/Settings/DeviceFile">
                         <span class="Setting_tree">{{$t("message.setting.File")}}</span> 
                    </el-menu-item>
					<el-menu-item index="/Settings/DeviceAll">
                         <span class="Setting_tree">{{$t("message.setting.all")}}</span> 
                    </el-menu-item>
					<el-menu-item index="/Settings/DeviceImport">
                        <span class="Setting_tree">{{$t("message.setting.DeviceImport")}}</span> 
                    </el-menu-item>
				</el-submenu>
				
				<!-- <el-submenu index="/Settings/DeviceSdk">
					<template slot="title">
						<span>{{$t("message.setting.devicesdk")}}</span>
					</template>
                    
				</el-submenu>
				<el-submenu index="/Settings/DeviceImport">
					<template slot="title">
						<span>{{$t("message.setting.DeviceImport")}}</span>
					</template>
                    
				</el-submenu>
				<el-submenu index="/Settings/RtmpPush">
					<template slot="title">
						<span>{{$t("message.setting.devicertmppush")}} </span>
					</template>
                    
				</el-submenu> -->
				<el-submenu index="/Settings/Record">
					<template slot="title">
						<i class="iconfont icon-luxiangguanli"></i>
						<span>{{$t("message.setting.record")}} </span>
					</template>
                    <el-menu-item index="/Settings/Record">
                        <span class="Setting_tree">{{$t("message.setting.record")}}</span> 
                    </el-menu-item>
				</el-submenu>

				<el-submenu index="/Settings/Default">
					<template slot="title">
						<i class="iconfont icon-zhuanmaguanli"></i>
						<span>{{$t("message.setting.Transcoding")}}</span>
					</template>
                    <el-menu-item index="/Settings/Default">
                        <span class="Setting_tree">{{this.$t("message.setting.Default")}}</span> 
                    </el-menu-item>
					<el-menu-item index="/Settings/Profile">
                        <span class="Setting_tree">{{this.$t("message.setting.Profile")}}</span> 
                    </el-menu-item>
				</el-submenu>

				<el-submenu index="/Settings/CloudConnect">
					<template slot="title">
						<i class="iconfont icon-jilianguanli"></i>
						<span>{{$t("message.setting.CloudConnect")}}</span>
					</template>
                    <el-menu-item index="/Settings/CloudConnect">
                        <span class="Setting_tree">{{this.$t("message.setting.CloudConnect")}}</span> 
                    </el-menu-item>
				</el-submenu>

				<!-- 系统 -->
				<el-submenu index="System">
					<template slot="title">
						<i class="iconfont icon-xitong"></i>
						<span>{{$t("message.dashboard.system_info")}} </span>
					</template>
					<!-- 系统 -->
					<el-submenu index="/Settings/System">
						<template slot="title">
							<span>{{$t("message.setting.System")}} </span>
						</template>
						<el-menu-item index="/Settings/System">
							<span class="Setting_tree">{{this.$t("message.setting.System")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/License">
							<span class="Setting_tree">Authorization</span> 
						</el-menu-item>
					</el-submenu>
					<!-- User -->
					<el-submenu index="/Settings/User">
						<template slot="title">
							<span>{{$t("message.setting.user")}}</span>
						</template>
						<el-menu-item index="/Settings/User">
							<span class="Setting_tree">{{this.$t("message.setting.user")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/Role">
							<span class="Setting_tree">{{this.$t("message.setting.Rolemanagement")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/CyberSecurity">
							<span class="Setting_tree">{{this.$t("message.setting.CyberSecurity")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/PlayCode">
							<span class="Setting_tree">{{this.$t("message.setting.PlayCode")}}</span> 
						</el-menu-item>
					</el-submenu>
					<!-- Docker -->
					<el-submenu index="/Settings/Docker">
						<template slot="title">
							<span>Docker</span>
						</template>
						<el-menu-item index="/Settings/Docker">
							<span class="Setting_tree">Docker</span> 
						</el-menu-item>
					</el-submenu>
					<!-- log -->
					<el-submenu index="/Settings/LogLevel">
						<template slot="title">
							<span>{{$t("message.setting.Log")}} </span>
						</template>
						<el-menu-item index="/Settings/LogLevel">
							<span class="Setting_tree">{{this.$t("message.setting.LogLevel")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/LogRotating">
							<span class="Setting_tree">{{this.$t("message.setting.LogRotating")}}</span> 
						</el-menu-item>
					</el-submenu>
					<el-menu-item index="/Settings/Database">
                        <span class="Setting_tree">{{$t("message.setting.database")}}</span> 
                    </el-menu-item>
					<a :href="'/mediastore?session='+session">
						<el-menu-item >
							<!-- <i class="iconfont icon-wenjianjia" style="font-size:20px"></i> -->
							<span slot="title">{{$t("message.left.screenshots")}}</span>
						</el-menu-item>
					</a>
				</el-submenu>

				<el-submenu index="协议">
					<template slot="title">
						<i class="iconfont icon-xieyi"></i>
						<span>{{$t("message.setting.Protocol")}} </span>
					</template>
					<el-menu-item index="/Settings/RTSP">
						<span class="Setting_tree">RTSP</span> 
					</el-menu-item>
					<!-- WEBRTC -->
					<el-submenu index="/Settings/CloudMode">
						<template slot="title">
							<span>WEBRTC</span>
						</template>
						<el-menu-item index="/Settings/CloudMode">
							<span class="Setting_tree">{{this.$t("message.setting.CloudMode")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/RelayMode">
							<span class="Setting_tree">{{this.$t("message.setting.RelayMode")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/TURNServer">
							<span class="Setting_tree">{{this.$t("message.setting.TURNServer")}}</span> 
						</el-menu-item>
					</el-submenu>
					<!-- HTTP -->
					<el-submenu index="HTTP">
						<template slot="title">
							<span>HTTP</span>
						</template>
						<el-menu-item index="/Settings/Configure">
							<span class="Setting_tree">{{$t("message.setting.Configuration")}}</span> 
						</el-menu-item>
						<el-menu-item index="/Settings/Domainlist">
							<span class="Setting_tree">{{$t("message.setting.Domainlist")}}</span> 
						</el-menu-item>
					</el-submenu>
				</el-submenu>

				<el-submenu index="/Settings/Map">
					<template slot="title">
						<i class="iconfont icon-ditu1"></i>
						<span>{{$t("message.left.Map")}}</span>
					</template>
					<el-menu-item index="/Settings/Mapmanagement">
                        <span class="Setting_tree">{{$t("message.left.MapManagement")}}</span> 
                    </el-menu-item>

				</el-submenu>

				<el-submenu index="/Settings/Cluster">
					<template slot="title">
						<i class="iconfont icon-jiqun"></i>
						<span>{{$t("message.dashboard.cluster")}}</span>
					</template>
					<el-menu-item index="/Settings/Cluster">
                        <span class="Setting_tree">{{$t("message.dashboard.cluster")}}</span> 
                    </el-menu-item>
				</el-submenu>
				
				<el-menu-item index="/Settings/Camera">
					<i class="iconfont icon-jiankongdian"></i>
					<span slot="title">{{$t("message.left.camera")}}</span>
				</el-menu-item>
				<el-submenu index="/Settings/Vqd">
					<template slot="title">
						<i class="iconfont icon-Ai"></i>
						<span>AI</span>
					</template>
					<el-menu-item index="/Settings/Vqd">
						<span class="Setting_tree">{{$t("message.left.VQD")}}</span>
					</el-menu-item>
					<el-menu-item index="/Settings/Ai">
						<span class="Setting_tree">{{$t("message.left.AI")}}</span>
					</el-menu-item>
					<el-menu-item index="/Settings/Advai">
						<span class="Setting_tree">{{$t("message.left.AdvAi")}}</span>
					</el-menu-item>
					<el-menu-item index="/Settings/AiChannel">
						<span class="Setting_tree">{{$t("message.left.AIXK")}}</span>
					</el-menu-item>
				</el-submenu>

			</el-menu>
		</div>
		<div class="setting_you">
			<router-view></router-view>
		</div>
	</div>
</div>
</template>

<script>
export default {
	name: 'Dashboard',
	data(){
		return{
			activemenu:"2-1",
			session:this.$store.state.token,
			color1:''
		}
	},
	watch:{
		contractFile(data){
			if (data == 'blue') {
				this.color1 = '#1f2540'
			}else if(data == 'c-dark-theme'){
				this.color1 = '#0c0c0c'
			}else{
				this.color1 = '#f5f5f5'
			}
		}
	},
	computed: {
		contractFile() {
			return this.$store.state.darkMode
		}
	},
	beforeUpdate(){
		this.menuList();
	},
	methods:{
		menuList(){ 
			let path = this.$route.path
			// console.log(path,this.$route)
			if(path!='/Settings/Client'){
				this.activemenu = path;
			} 
		}
	},
	mounted(){
		if (this.$store.state.darkMode == 'blue') {
			this.color1 = '#1f2540'
		}else if(this.$store.state.darkMode == 'c-dark-theme'){
			this.color1 = '#0c0c0c'
		}else{
			this.color1 = '#f5f5f5'
		}
		this.menuList();
	},
	created(){
	}
}
</script>
<style lang="scss" scoped>
.setting_all{
	display: flex;
	justify-content: space-between;
	flex-wrap: wrap;
	.setting_zuo{
		width: 14%;
		height: 91vh;
		overflow-y: auto;
		// background: #2d2d2d;
	}
	i{
		margin-right: 10px;
	}
	.setting_you{
		width: 86%;
		height: 91vh;
		padding-top: 10px;
		overflow-y: auto;
		// background-color: #171717;
	}
	.setting_you::-webkit-scrollbar,.setting_zuo::-webkit-scrollbar{
		display: none;
	}
}
</style>