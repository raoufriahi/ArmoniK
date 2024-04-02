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
                    <!-- <el-submenu index="/PF/RTMP">
                        <template slot="title">
                            <i class="iconfont icon-tuiliu"></i>
                            <span>{{$t("message.left.PF")}}</span>
                        </template> -->
                        <el-menu-item index="/PF/RTMP">
							<i class="iconfont icon-tuiliu"></i>
                            <span class="Setting_tree" slot="title"> {{$t("message.PF.RTMP")}}</span>
                        </el-menu-item>
                        <!-- <el-menu-item index="/PF/WHIP">
                            <span class="Setting_tree" slot="title"> {{$t("message.PF.WHIP")}}</span>
                        </el-menu-item> -->
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
            this.activemenu = path
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
		max-width: 278px;
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
		// padding-top: 10px;
		overflow-y: auto;
		// background-color: #171717;
	}
	.setting_you::-webkit-scrollbar,.setting_zuo::-webkit-scrollbar{
		display: none;
	}
}
</style>