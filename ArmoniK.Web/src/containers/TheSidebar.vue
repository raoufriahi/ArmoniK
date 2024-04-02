<template>
	<div class="Sidebar">
		<CSidebar 
			:show="$store.state.asideShow"
			@update:show="(val) => $store.commit('set', ['asideShow', val])"
			colorScheme="light"
			overlaid
		>
			<CSidebarBrand class="d-md-down-none">
				<img 
					:src="($store.state.darkMode)? require('../assets/imgs/logo_hei.svg'):require('../assets/imgs/logo_hei.png')"
					width="180" height="26" alt="H5S Logo"/>
			</CSidebarBrand>
			<!-- <CSidebarClose @click.native="$store.commit('toggle', 'asideShow')"/> -->
			<el-menu
				router
				:default-active="activeIndex" 
				class="el-menu-vertical-demo"
				:text-color="($store.state.darkMode) ? '#B7B7B7' : '#000'"
				active-text-color="#000000">
				<el-menu-item index="/Dashboard">
					<i class="iconfont icon-yibiaopan"></i>
					<span slot="title">{{$t("message.left.dashboard")}}</span>
				</el-menu-item>
				<el-menu-item index="/Liveview">
					<i class="iconfont icon-shishishipin"></i>
					<span slot="title">{{$t("message.live.liveview")}}</span>
				</el-menu-item>
				<el-menu-item index="/Replay/Advancepb">
					<i class="iconfont icon-huifang"></i>
					<span slot="title">{{$t("message.left.playback")}}</span>
				</el-menu-item>
				<!-- <el-menu-item v-if="this.$store.state.root=='Administrator'" index="/Avintercom/Avintercoms">
					<i class="iconfont icon-shipinhuiyi3"></i>
					<span slot="title">{{$t("message.Conference.Upload")}}</span>
				</el-menu-item> -->
				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/Tour">
					<i class="iconfont icon-xungeng"></i>
					<span slot="title">{{$t("message.left.tour")}}</span>
				</el-menu-item>
				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/GB/GB28181">
					<i class="iconfont icon-GB" style="font-size:18px"></i>
					<span slot="title">GB</span>
				</el-menu-item>
				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/PF/RTMP">
					<i class="iconfont icon-tuiliu" style="font-size:18px"></i>
					<span slot="title">{{$t("message.left.PF")}}</span>
				</el-menu-item>
				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/transcoding/GPUTranscoding">
					<i class="iconfont icon-zhuanma" style="font-size:18px"></i>
					<span slot="title">{{$t("message.dashboard.transcoding")}}</span>
				</el-menu-item>
				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/Clouds">
					<i class="iconfont icon-yunshipin" style="font-size:14px"></i>
					<span slot="title">{{$t("message.left.cloud")}}</span>
				</el-menu-item>

				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/Map">
					<i class="iconfont icon-ditu" style="font-size:14px"></i>
					<span slot="title">{{$t("message.left.Map")}}</span>
				</el-menu-item>

				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/Settings/Edit">
					<i class="iconfont icon-shezhi"></i>
					<span slot="title">{{$t("message.setting.setting")}}</span>
				</el-menu-item>
				<!-- <el-menu-item v-if="this.$store.state.root=='Administrator'" index="/Storage/StorageConfig" v-show="bStorageConfigUI">
					<i class="iconfont icon-wenjianjiqun"></i>
					<span slot="title">{{$t("message.left.storage")}}</span>
				</el-menu-item> -->
				<el-menu-item v-if="this.$store.state.root=='Administrator'" index="/Advanced">
					<i class="iconfont icon-gaojishezhi"></i>
					<span slot="title">{{$t("message.left.advanced")}}</span>
				</el-menu-item>
			</el-menu>
		</CSidebar>
	</div>
</template>

<script>
import nav from './_nav'
export default {
	name: 'TheSidebar',
	nav,
	computed: {
		show () {
			return this.$store.state.sidebarShow 
		},
		minimize () {
			return this.$store.state.sidebarMinimize 
		}
	},
	data(){
		return {
			activeIndex:"Dashboard",
			bStorageConfigUI:''
		}
	},
	mounted(){
		var configflag=this.$store.state.switch
		this.bStorageConfigUI=configflag
		console.log(configflag.switch)
		this.menuList();	
	},
	methods:{
		menuList(){ 
			let path = this.$route.matched[1].meta.title
			// console.log(this.$route)
            this.activeIndex = path
        }
	}
}
</script>
<style lang="scss" scoped>
a{
	text-decoration: none;
}
</style>
