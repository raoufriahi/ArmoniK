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
                    style="height:100%;">
						<el-menu-item index="/transcoding/GPUTranscoding">
                            <i class="iconfont icon-zhengzaizhuanma" style="font-size:17px;"></i>
                            <span class="Setting_tree" slot="title">Transcoding</span>
                        </el-menu-item>
                        <el-menu-item index="/transcoding/Nvidia"  v-if="NVIDIA">
                            <i class="iconfont icon-NVIDIA1"  style="font-size:17px;"></i>
                            <span class="Setting_tree" slot="title">Nvidia</span>
                        </el-menu-item>
                        <el-menu-item index="/transcoding/LinuxIntel" v-if="LINUXINTEL">
                            <i class="iconfont icon-inter"  style="font-size:17px;"></i>
                            <span class="Setting_tree" slot="title">Intel</span>
                        </el-menu-item>
                        <el-menu-item index="/transcoding/TransGPU">
                            <i class="iconfont icon-bendizhuanmapeizhi" style="font-size:17px;"></i>
                            <span class="Setting_tree" slot="title">Local transcoding</span>
                        </el-menu-item>
                        <el-menu-item index="/transcoding/RTS">
							<i class="iconfont icon-shebeiguanli" style="font-size:17px;"></i>
                            <span class="Setting_tree" slot="title"> RTS Configuration</span>
                        </el-menu-item>
                        <el-menu-item index="/transcoding/ServerConf">
                            <i class="iconfont icon-peizhi-yunweipeizhi"></i>
                            <span class="Setting_tree" slot="title">RTS service</span>
                        </el-menu-item>
                        
                    <!-- </el-submenu> -->
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
	name: 'transcoding',
	data(){
		return{
			activemenu:"2-1",
			session:this.$store.state.token,
			color1:'',
			windows:false,
			NVIDIA:true,
			INTEL:false,
			LINUXINTEL:false,
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
	methods:{
		Nvidia(){
			var url = this.$store.state.IPPORT + "/api/v2/GetNvidiaGPUList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.status === 200) {
                    console.log(result)
					console.log(result.data.length);
					if (result.data.length==0) {
						this.NVIDIA = false;
					}
                }
            })
		},
		Intel(){
			var url = this.$store.state.IPPORT + "/api/v2/GetIntelGPUList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
				console.log(result);
                if (result.status === 200) {
					if (result.data.length==0) {
						this.INTEL = false;
					}else{
						this.INTEL = true;
					}
                }
            })
		},
		linuxIntel(){
			var url = this.$store.state.IPPORT + "/api/v2/GetLinuxIntelGPUList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
				console.log(result);
                if (result.status === 200) {
					if (result.data.length==0) {
						this.LINUXINTEL = false;
					}else{
						this.LINUXINTEL = true;
					}
                }
            })
		},
		menuList(){ 
            let path = this.$route.path
            this.activemenu = path
        }
		
	},
	mounted(){
		console.log(navigator.platform);
		if (this.$store.state.darkMode == 'blue') {
			this.color1 = '#1f2540'
		}else if(this.$store.state.darkMode == 'c-dark-theme'){
			this.color1 = '#0c0c0c'
		}else{
			this.color1 = '#f5f5f5'
		}
		this.menuList();
		this.Nvidia();
		this.Intel();
		this.linuxIntel();
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
		max-width: 278px;
		height: 91vh;
		overflow-y: auto;
	}
	i{
		margin-right: 10px;
	}
	.setting_you{
		width: 86%;
		height: 91vh;
		padding-top: 10px;
		overflow-y: auto;
	}
	.setting_you::-webkit-scrollbar,.setting_zuo::-webkit-scrollbar{
		display: none;
	}
}
</style>