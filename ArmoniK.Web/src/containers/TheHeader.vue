<template>
<div>
	<el-dialog
		class="plugin_adout"
		:visible.sync="centerDialogVisible"
		width="25%"
		append-to-body
		center>
		<div class="about_flex">
			<div v-if="$store.state.darkMode">
				<img class="adout_img" src="../assets/imgs/logo_hei.svg"/>
			</div>
			<div v-else>
				<img class="adout_img" src="../assets/imgs/logo.svg"/>
			</div>
			<div style="margin: 20px 0 10px 20px; text-align: center;">
				<div>{{$t("message.dashboard.version")}}: {{information.strVersion}}</div>
			</div>
		</div>
	</el-dialog>
	<el-dialog
		class="Rebootdialog"
		title="Reboot"
		append-to-body
		:visible.sync="Rebootdialog"
		width="25%">
		<div class="Rebootdialog1">
			<div style="">Please restart after modification</div>
			<div slot="footer" class="dialog-footer" style="display: flex;justify-content: flex-end;">
				<el-button type="primary" @click="Reboot">{{label.Reboot}}</el-button>
			</div>
		</div>
	</el-dialog>
	<CHeader with-subheader>
		<!-- <CToggler
		in-header
		class="ml-3 d-lg-none"
		@click="$store.commit('toggleSidebarMobile')"
		/>
		<CToggler
			in-header
			class="ml-3 d-md-down-none"
			@click="$store.commit('toggleSidebarDesktop')"
		/> -->
		
		<CHeaderNav>
			<CHeaderNavItem class="px-3">
				<button
				in-header
				class="c-header-nav-btn"
				@click="$store.commit('toggle', 'asideShow')"
				>
					<i class="iconfont icon-hanbaobao" style="font-size: 24px;"></i>
				</button>
			</CHeaderNavItem>
		</CHeaderNav>
		<CHeaderNav class="d-md-down-none mr-auto" >
			<img 
			v-if="$store.state.darkMode"
			src="../assets/imgs/logo_hei.svg"
			width="180"
			height="25"
			alt="H5S Logo"/>
			<img 
			v-else
			src="../assets/imgs/logo_hei.png"
			width="180"
			height="25"
			alt="H5S Logo"/>
		</CHeaderNav>
		
		<!-- 快照 -->
		<CHeaderNav>
			<!-- <div class="system" id="system_togg">
				<el-tooltip content="快照" placement="bottom" effect="dark">
					<el-button style="border: none;background: none; color:#fff;line-height: 0.9;padding-right: 20px;" >
						<i style=" font-size: 18px;color:#ea5252;font-weight: 500;" class="iconfont icon-jinggao"></i>
					</el-button>
				</el-tooltip>
			</div> -->
			<div class="system" id="system_togg" style="padding-right: 20px;">
				<el-popover
					placement="bottom"
					trigger="click">
					<template slot="reference">
						<i id="breath_light" style=" font-size: 18px;color:#ea5252;font-weight: 500;" class="iconfont icon-jinggao"></i>
					</template>
					<div id="system_togg2" style="border-radius:7px;padding-bottom: 6px;">{{$t("message.header.Systemdoes")}}
						<router-link :to="{name:'System'}">
							<div style="color:rgba(58, 161, 255, 1); text-align: right;margin-top: 10px;">Go to snapshot<i class="iconfont icon-qianjin"></i> </div>
						</router-link>
					</div>
					<div id="CyberSecurity_togg"  style="padding-top: 6px;">{{$t("message.header.CyberSecurityOption")}}
						<router-link :to="{name:'CyberSecurity'}">
							<div style="color:rgba(58, 161, 255, 1); text-align: right;margin-top: 10px;">Go to close<i class="iconfont icon-qianjin"></i> </div>
						</router-link>
					</div>
					<div id="LogRotating_togg"  style="padding-top: 6px;">{{$t("message.header.LogRotatingRemind")}}
						<router-link :to="{name:'LogRotating'}">
							<div style="color:rgba(58, 161, 255, 1); text-align: right;margin-top: 10px;">Go to open<i class="iconfont icon-qianjin"></i> </div>
						</router-link>
					</div>
				</el-popover>
			</div>
		</CHeaderNav>

		<CHeaderNav>
			<div class="c_Docker" id="Docker"></div>
		</CHeaderNav>
		<CHeaderNav style="margin-right: 15px;">
			<router-link :to="{name:'Event'}">
				<el-badge :value="gEvvalue" :max="999" class="item" data-toggle="dropdown">
					<i style="color: #fff;" class="iconfont icon-lingdang" type="primary"></i>
				</el-badge>
			</router-link>
		</CHeaderNav>
		<CHeaderNav>
		<div class="use_user" id="rtc_togg">
			<el-tooltip content="Reboot" placement="bottom" effect="dark">
				<el-button @click="Rebootdialog=true" style="border: none;background: none; color:#fff;line-height: 0.9;padding-right: 10px;" >
					<i style=" font-size: 18px;color:#ea5252;font-weight: 500;" class="iconfont icon-zhongqi"></i>
				</el-button>
			</el-tooltip>
		</div>
		</CHeaderNav>
		<!-- <CHeaderNav>
			<CDropdown
				:caret="false">
				<template #toggler-content>
					<div style=""><i class="iconfont icon-huanfu" style="font-size:22px;"></i></div>
				</template>
				<CDropdownItem @click="skin(false)">
					<div class="about_ab" style="padding-left:10px;">默认白</div>
				</CDropdownItem>
				<CDropdownItem @click="skin('c-dark-theme')">
					<div class="about_ab" style="padding-left:10px;">炫酷黑</div>
				</CDropdownItem>
				<CDropdownItem @click="skin('blue')">
					<div class="about_ab" style="padding-left:10px;">科技蓝</div>
				</CDropdownItem>
			</CDropdown>
		</CHeaderNav> -->
		<!-- <CHeaderNav>
			<CHeaderNavItem class="px-3">
				<button 
				@click="() => $store.commit('toggle', 'darkMode')" 
				class="c-header-nav-btn"
				>
				<CIcon v-if="$store.state.darkMode" name="cil-sun"/>
				<CIcon v-else name="cil-moon"/>
				</button>
			</CHeaderNavItem>
		</CHeaderNav> -->
		<CHeaderNav>
			<!-- <CDropdown
				:caret="false">
				<template #toggler-content> -->
					<i class="iconfont icon-yonghuming"></i> {{user}}
				<!-- </template> -->
				<!-- <CDropdownItem :to="{name:'Logout'}">
					<div class="about_ab" v-if="user==''||user==null"><i class="iconfont icon-shijian-"></i>{{$t("message.header.login")}}</div>
					<div class="about_ab" v-else><i class="iconfont icon-shijian-"></i>{{$t("message.header.logouts")}}</div>
				</CDropdownItem> -->
			<!-- </CDropdown> -->
		</CHeaderNav>
		<!-- <CHeaderNav>
			<CDropdown
				:caret="false">
				<template #toggler-content>
					<i class="iconfont icon-gengduo"></i>
				</template>
				<CDropdownItem >
					<div @click="centerDialogVisible=true" class="about_ab"><i class="iconfont icon-prompt"></i>关于</div>
				</CDropdownItem> -->
				<!-- <CDropdownItem href="doc/api.html">
					<div class="about_ab"><i class="iconfont icon-category"></i>API</div>
				</CDropdownItem> -->
				<!-- <CDropdownItem href="doc/H5S视频平台用户手册-zh.chm">
					<div class="about_ab"><i class="iconfont icon-category"></i>手册</div>
				</CDropdownItem> -->
				<!-- <CDropdownItem @click="skin">
					<div class="about_ab"><i class="iconfont icon-huanfu"></i>主题</div>
				</CDropdownItem> -->
				<!-- <CDropdownItem @click="Rebootdialog=true">
					<div class="about_ab"><i class="iconfont icon-zhongqi"></i>{{$t("message.header.Reboot")}}</div>
				</CDropdownItem>
			</CDropdown>
		</CHeaderNav> -->
		<!-- 黑色主题 -->
		<el-dropdown trigger="click" class="headerMoreFunctions" v-if="this.$store.state.darkMode == 'c-dark-theme'">
			<div class="dropdown"><i class="iconfont icon-androidgengduo"></i></div>
			<el-dropdown-menu slot="dropdown" class="dropdownMenu" style="background-color:#2C2C2C;border: none;color:#fff;">
				<div>
					<el-popover placement="left-start" trigger="hover" ref="huanfu">
						<div @click="skin(false)" style="padding-left:10px;padding-right:10px;padding-bottom:10px">
							<el-button type="text" style="padding:0px;color:#fff;">Default white</el-button>
						</div>
						<div @click="skin('c-dark-theme')" style="padding-left:10px;padding-right:10px;padding-bottom:10px">
							<el-button type="text" style="padding:0px;color:#fff;">Cool black</el-button>
						</div>
						<div @click="skin('blue')" style="padding-left:10px;padding-right:10px;">
							<el-button type="text" style="padding:0px;color:#fff;">Tech blue</el-button>
						</div>
						<el-button slot="reference" class="about_ab huanfu" style="width:100%;background-color:#2C2C2C;border:none;color:#fff;text-align:left;"><i class="iconfont icon-huanfu"></i>Theme</el-button>
					</el-popover>
				</div>
				<el-dropdown-item>
					<div @click="centerDialogVisible=true" class="about_ab"><i class="iconfont icon-prompt"></i>About</div>
				</el-dropdown-item>
				<el-dropdown-item >
					<a href="doc/H5S视频平台用户手册-zh.chm"><div class="about_ab"><i class="iconfont icon-category"></i>User Manual</div></a>
				</el-dropdown-item>
				<el-dropdown-item >
					<div class="about_ab" @click="client"><i class="iconfont icon-kehuduanpeizhi"></i>Client configuration</div>
				</el-dropdown-item>
				<el-dropdown-item divided></el-dropdown-item>
				<el-dropdown-item>
					<div class="about_ab" @click="Rebootdialog=true"><i class="iconfont icon-zhongqi"></i>{{$t("message.header.Reboot")}}</div>
				</el-dropdown-item>
				<el-dropdown-item>
					<router-link :to="{name:'Logout'}">
					<div class="about_ab" v-if="user==''||user==null"><i class="iconfont icon-shijian-"></i>{{$t("message.header.login")}}</div>
					<div class="about_ab" v-else><i class="iconfont icon-shijian-"></i>{{$t("message.header.logouts")}}</div>
					</router-link>
				</el-dropdown-item>
			</el-dropdown-menu>
		</el-dropdown>
		<!-- 蓝色主题 -->
		<el-dropdown trigger="click" class="headerMoreFunctions" v-else-if="this.$store.state.darkMode == 'blue'">
			<div class="dropdown"><i class="iconfont icon-androidgengduo"></i></div>
			<el-dropdown-menu slot="dropdown" class="dropdownMenu dropdownMenuBlue" style="background-color:#090822;border: none;color:#fff;">
				<div>
					<el-popover placement="left-start" trigger="hover" ref="huanfu">
						<div @click="skin(false)" style="padding-left:10px;padding-right:10px;padding-bottom:10px">
							<el-button type="text" style="padding:0px;color:#fff;">Default white</el-button>
						</div>
						<div @click="skin('c-dark-theme')" style="padding-left:10px;padding-right:10px;padding-bottom:10px">
							<el-button type="text" style="padding:0px;color:#fff;">Cool black</el-button>
						</div>
						<div @click="skin('blue')" style="padding-left:10px;padding-right:10px;">
							<el-button type="text" style="padding:0px;color:#fff;">Tech blue</el-button>
						</div>
						<el-button slot="reference" class="about_ab huanfu" style="width:100%;background-color:#090822;border:none;color:#fff;text-align:left;"><i class="iconfont icon-huanfu"></i>Theme</el-button>
					</el-popover>
				</div>
				<el-dropdown-item>
					<div @click="centerDialogVisible=true" class="about_ab"><i class="iconfont icon-prompt"></i>About</div>
				</el-dropdown-item>
				<el-dropdown-item >
					<a href="doc/H5S视频平台用户手册-zh.chm"><div class="about_ab"><i class="iconfont icon-category"></i>User Manual</div></a>
				</el-dropdown-item>
				<el-dropdown-item >
					<div class="about_ab" @click="client"><i class="iconfont icon-kehuduanpeizhi"></i>Client Configuration</div>
				</el-dropdown-item>
				<el-dropdown-item divided></el-dropdown-item>
				<el-dropdown-item>
					<div class="about_ab" @click="Rebootdialog=true"><i class="iconfont icon-zhongqi"></i>{{$t("message.header.Reboot")}}</div>
				</el-dropdown-item>
				<el-dropdown-item>
					<router-link :to="{name:'Logout'}">
					<div class="about_ab" v-if="user==''||user==null"><i class="iconfont icon-shijian-"></i>{{$t("message.header.login")}}</div>
					<div class="about_ab" v-else><i class="iconfont icon-shijian-"></i>{{$t("message.header.logouts")}}</div>
					</router-link>
				</el-dropdown-item>
			</el-dropdown-menu>
		</el-dropdown>
		<!-- 白色主题 -->
		<el-dropdown trigger="click" class="headerMoreFunctions" v-else>
			<div class="dropdown"><i class="iconfont icon-androidgengduo"></i></div>
			<el-dropdown-menu slot="dropdown" class="dropdownMenuWhite" style="background-color:#F2F1F1;border: none;color:#000000;">
				<div>
					<el-popover placement="left-start" trigger="hover" ref="huanfu">
						<div @click="skin(false)" style="padding-left:10px;padding-right:10px;padding-bottom:10px">
							<el-button type="text" style="padding:0px;color:#000000;">Default white</el-button>
						</div>
						<div @click="skin('c-dark-theme')" style="padding-left:10px;padding-right:10px;padding-bottom:10px">
							<el-button type="text" style="padding:0px;color:#000000;">Cool black</el-button>
						</div>
						<div @click="skin('blue')" style="padding-left:10px;padding-right:10px;">
							<el-button type="text" style="padding:0px;color:#000000;">Tech blue</el-button>
						</div>
						<el-button slot="reference" class="about_ab huanfu" style="width:100%;background-color:#F2F1F1;border:none;color:#000000;text-align:left;"><i class="iconfont icon-huanfu"></i>Theme</el-button>
					</el-popover>
				</div>
				<el-dropdown-item>
					<div @click="centerDialogVisible=true" class="about_ab"><i class="iconfont icon-prompt"></i>About</div>
				</el-dropdown-item>
				<el-dropdown-item >
					<a href="doc/H5S视频平台用户手册-zh.chm"><div class="about_ab"><i class="iconfont icon-category"></i>User Manual</div></a>
				</el-dropdown-item>
				<el-dropdown-item >
					<div class="about_ab" @click="client"><i class="iconfont icon-kehuduanpeizhi"></i>Client configuration</div>
				</el-dropdown-item>
				<el-dropdown-item divided></el-dropdown-item>
				<el-dropdown-item>
					<div class="about_ab" @click="Rebootdialog=true"><i class="iconfont icon-zhongqi"></i>{{$t("message.header.Reboot")}}</div>
				</el-dropdown-item>
				<el-dropdown-item>
					<router-link :to="{name:'Logout'}" >
					<div class="about_ab" v-if="user==''||user==null"><i class="iconfont icon-shijian-"></i>{{$t("message.header.login")}}</div>
					<div class="about_ab" v-else><i class="iconfont icon-shijian-"></i>{{$t("message.header.logouts")}}</div>
					</router-link>
				</el-dropdown-item>
			</el-dropdown-menu>
		</el-dropdown>
		<CSubheader class="px-4">
			<!-- 修改面包屑导航 -->
			<CHeaderNav>
				<div class="header_size">
					<i :class="this.$route.matched[1].meta.icon"> </i>
					{{this.$route.matched[1].meta.name}}
				</div>
			</CHeaderNav>
			<!-- <CHeaderNav v-if="this.$route.matched[1].meta.icon=='iconfont icon-shipin1'">
				<CHeaderNavItem>
					<button
					in-header
					class="c-header-nav-btn"
					@click="$store.commit('toggle', 'asideShows')"
					>
						<img 
						src="../assets/imgs/liveview_setting.png"
						width="16"
						height="16"
						alt="CoreUI Logo"/>
					</button>
				</CHeaderNavItem>
			</CHeaderNav> -->
		<!-- <CBreadcrumbRouter class="border-0 mb-0"/> -->
		</CSubheader>
		
	</CHeader>
	
</div>
</template>

<script>
import Vue from 'vue'
export default {
	name: 'TheHeader',
	data(){
		return {
			timerRunInfo:undefined,
			gEvvalue: 0,
			user:this.$store.state.user,
			Rebootdialog:false,
			centerDialogVisible:false,
			subtitle:{
				icon:this.$route.matched[1].meta.icon,
				name:this.$route.matched[1].meta.name
			},
			information:{
				strVersion: ""
			},
			toggle:"",//主题
			label:{
				Edit:this.$t("message.table.Edit"),
				user:this.$t("message.setting.username"),
				role:this.$t("message.setting.role"),
				type:this.$t("message.setting.Authority"),
				olPassword:this.$t("message.setting.currentpass"),
				nePassword:this.$t("message.setting.newpass"),
				nePassword1:this.$t("message.setting.confirmpass"),
				Change:this.$t("message.setting.Change"),
				Download:this.$t("message.archive.Download"),
				Control:this.$t("message.left.Control"),
				goto:this.$t("message.header.goto"),
				control1:this.$t("message.header.control"),
				Reboot:this.$t("message.header.Reboot")
			},
			timersetInterval:"",//定时器
		}
	},
	watch:{
		toggle(a){
			this.$root.bus.$emit('skintoggle', a);
		}
	},
	beforeDestroy() {
        clearInterval(this.timerRunInfo);
				clearInterval(this.timersetInterval);
    },
	mounted(){
		if (this.$route.query.session) {
			sessionStorage.setItem('mcutoken',this.$route.query.session)
			this.$store.state.token=this.$route.query.session;
		};
		this.Docker();
		this.GetSystemInfo();
		var _this=this
		$("#rtc_togg").hide();
		_this.$root.bus.$on('webrtc', function(token){
			$("#rtc_togg").show();
		});
		this.gEventval();
		this.System();
		this.anonymou();
		this.systemdata();
		setTimeout(()=>{
			this.dangerSetinterval();
		},1000)
		_this.$root.bus.$on('System', function(token){
			_this.System();
			setTimeout(()=>{
				_this.dangerSetinterval();
			},800)
		});
		_this.$root.bus.$on('Anonymous', function(token){
			_this.anonymou();
			setTimeout(()=>{
				_this.dangerSetinterval();
			},800)
		});
		_this.$root.bus.$on('LogData', function(token){
			_this.systemdata();
			setTimeout(()=>{
				_this.dangerSetinterval();
			},800)
		});
	},
	methods:{
		client(){
			this.$router.push({path: '/Settings/Client'})
		},
		// 头部警告闪烁提示
		dangerSetinterval(){
			if ($("#system_togg2").css('display')=="block"||$("#CyberSecurity_togg").css('display')=="block"||$("#LogRotating_togg").css('display')=="block") {
			this.timersetInterval=setInterval(()=>{
					if ($("#breath_light").css('color')=="rgb(234, 82, 82)") {
							$('#breath_light').css('color','rgb(255, 255, 0)');
					}else if ($("#breath_light").css('color')=="rgb(255, 255, 0)"){
						$('#breath_light').css('color','rgb(234, 82, 82)');
					}
				},1000)
			}else{
				clearInterval(this.timersetInterval);
			}
		},
		// 日志循环覆盖
		systemdata(){
			var url = this.$store.state.IPPORT + "/api/v1/GetRotatingLog?session="+ this.$store.state.token;
			this.$http.get(url).then(result=>{
				// console.log(result);
				if(result.status == 200){
					if (result.data.enable==true) {
						$('#LogRotating_togg').hide();
						if ($("#system_togg2").css('display')=="block"||$("#CyberSecurity_togg").css('display')=="block") {
							$("#system_togg").show();
						}else{
							$("#system_togg").hide();
						}
					}else{
						$('#LogRotating_togg').show();
						$("#system_togg").show();
					}
				}
			})
		},
		// 是否匿名浏览
		anonymou(){
			var url1 = this.$store.state.IPPORT + "/api/v1/GetAnonymousView?session="+ this.$store.state.token;
			this.$http.get(url1).then(result=>{
				if(result.status==200){
					// this.anonymous=result.data.bAnonymousView
					if (result.data.bAnonymousView==true) {
						setTimeout(()=>{
							$("#system_togg").show();
							$('#CyberSecurity_togg').show();
						},200)
					}else{
						setTimeout(function(){
							$('#CyberSecurity_togg').hide();
							if ($("#system_togg2").css('display')=="block"||$("#LogRotating_togg").css('display')=="block") {
								$("#system_togg").show();
							}else{
								$("#system_togg").hide();
							}
						},200)
					}
				}
			})
		},
		System(){
			var url = this.$store.state.IPPORT + "/api/v1/GetSnapshotList?session="+ this.$store.state.token;
			// console.log(url)
			this.$http.get(url).then(result=>{
				console.log(result,result.data.strName.length)
				if(result.status == 200){
					if(result.data.strName.length<=0){
						$("#system_togg2").show();
						$("#system_togg").show();
					}else{
						$("#system_togg2").hide();
						if ($("#CyberSecurity_togg").css('display')=="block"||$("#LogRotating_togg").css('display')=="block") {
							$("#system_togg").show();
						}else{
							$("#system_togg").hide();
						}
					}
				}
			})
		},
		gEventval() {
			this.timerRunInfo= setInterval(()=> {
				this.gEvvalue = this.EVENT.gEvent.length;
				},1000);
		},
		rtctogg(){
			$("#rtc_togg").hide();
		},
		GetSystemInfo(){
			var root = this.$store.state.IPPORT;
			var url = root + "/api/v1/GetSystemInfo?session="+ this.$store.state.token;

			this.$http.get(url).then(result => {
				//console.log(result);
				if (result.status == 200) 
				{
					this.information = result.data;
					// console.log(_this.information);
				}
			}).catch(error => {
				console.log('GetSystemInfo', error);
			});

		},
		//重启
		Reboot(){
			var url = this.$store.state.IPPORT + "/api/v1/Restart?session="+ this.$store.state.token;
			this.$http.get(url).then(result=>{
				if(result.status==200){
					if(result.data.bStatus){
						console.log("重启",result.data.bStatus)
					}
				}
			})
			
			var loading = Vue.prototype.$loading({
				lock: true,
				text: '重启中',
				background:"RGBA(0, 0, 0, 0.5)",
				target: '.Rebootdialog1'  // 需要loading的元素的类名
			})
			setTimeout(()=>{
				this.$nextTick(()=>{
					loading.close();
					this.$router.push({ path:'../../login'})
				})
			},1000*30)
		},
		Docker(){
			var url = this.$store.state.IPPORT + "/api/v1/GetEnableLinkagent?session="+ this.$store.state.token;
			this.$http.get(url).then(result=>{
				if(result.status==200){
					// console.log(result)
					if(result.data.enable){
						document.getElementById('Docker').style.display='block'
					}
				}
			})
		},
		skin(data){
			let refName ='huanfu';
			if (this.$refs[refName].showPopper) {
				this.$refs[refName].doClose();
			}
			this.toggle=this.$store.state.darkMode;
			// this.$store.commit('toggle', ['darkMode',data]);
			this.$store.state.darkMode=data;
			localStorage.themetoggle=this.$store.state.darkMode;
			console.log(localStorage.getItem('themetoggle'),this.$store.state.darkMode,typeof(this.$store.state.darkMode));
		},
	}
}
</script>
<style lang="scss" scoped>
.header_size{
  font-size: 14px;
  i{
    font-size: 14px;
    margin-right: 10px;
  }
}
.c-subheader{
	display: flex;
	justify-content: space-between;
}
.about_ab i{
	margin-right: 10px;
}
.c_Docker{
    width: 40px;
	height: 100%;
    background: url("~@/assets/imgs/Docker.png") center center no-repeat;
	display: none;
	background-size: 65%;
}
</style>