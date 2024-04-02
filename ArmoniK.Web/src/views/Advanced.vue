<template>
    <div class="advanced"> 
        <div class="setting_zuo">
			<el-menu
                router
                :default-active="activemenu" 
				class="el-menu-vertical-demo"
				:background-color="color1"
				:text-color="($store.state.darkMode) ? '#fff' : '#7A7A7A'"
				style="height:100%;">
				<el-menu-item index="/Advanced/Information">
					<i class="icon iconfont icon-xitongxinxi" style="font-size:16px;"> </i>
					<span slot="title"> System message</span>
				</el-menu-item>
				<el-menu-item index="/Advanced/Systems" v-if="webconsole&&protocol">
					<i class="iconfont icon-caozuoxinxi" style="font-size:16px;"> </i>
					<span slot="title"> {{$t("message.left.Systems")}}</span>
				</el-menu-item>
				<el-menu-item index="/Advanced/Storages" v-if="webconsole&&protocol">
					<i class="icon iconfont icon-cunchu" style="font-size:16px;"> </i>
					<span slot="title"> {{$t("message.left.storage")}}</span>
				</el-menu-item>
				<el-menu-item index="/Advanced/Terminals" v-if="webconsole&&protocol">
					<i class="icon iconfont icon-zhongduan" style="font-size:16px;"> </i>
					<span slot="title"> {{$t("message.left.Terminals")}}</span>
				</el-menu-item>
				<el-menu-item index="/Advanced/Network" v-if="webconsole&&protocol">
					<i class="icon iconfont icon-wangluopeizhi" style="font-size:16px;"> </i>
					<span slot="title"> {{$t("message.left.Network")}}</span>
				</el-menu-item>
				<el-menu-item index="/Advanced/Class"  v-if="H5SExtention">
					<i class="icon iconfont icon-wangluopeizhi" style="font-size:16px;"> </i>
					<span slot="title"> {{$t("message.left.class")}}</span>
				</el-menu-item>
				<el-menu-item index="/Advanced/advaiclass"  v-if="H5SExtention">
					<i class="icon iconfont icon-wangluopeizhi" style="font-size:16px;"> </i>
					<span slot="title"> AI标注</span>
				</el-menu-item>
			</el-menu>
		</div>
		<div class="setting_you">
			<router-view></router-view>
		</div>
    </div>
</template>
<script>

export default {
    name:'Advanced',
    data(){
        return{
            activemenu:'/Advanced/Information',
			color1:'',
			webconsole:false,
			protocol: window.location.protocol == "https:"?true:false,
			H5SExtention:false
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
		menuList(){ 
            let path = this.$route.path
			// console.log(path,this.$route)
            this.activemenu = path
        },
		GetAdvSystemInfo(){
            let url = this.$store.state.IPPORT + "/api/v1/GetAdvSystemInfo?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
				console.log(result);
                if (result.status == 200){
                    this.webconsole = result.data.bWebConsoleSupport;
					this.H5SExtention= result.data.bExtSupport;
                } 
            }).catch((err) =>{
                console.log('GetSrc failed', err);
            })
        },
		
	},
	mounted(){
		this.GetAdvSystemInfo();
		this.menuList();
	},
}
</script>
<style lang="scss" scoped>
.advanced{
    display: flex;
    justify-content: space-between;
    flex-wrap: wrap;
    .setting_zuo{
		.icon{
			font-size: 18px;
		}
        .el-menu{
            border: none;
            .el-submenu{
                .el-menu-item{
                    &.is-active {
                        border: none;
                        .Setting_tree{
                            padding-right: 50px;
                            border-right: 2px solid #2E9EFB;
                        }
                        color: #FFFFFF;
                    }
                    
                }
            }
        }
		width: 14%;
		height: 91vh;
		overflow-y: auto;
	}
    .setting_you{
		width: 86%;
		height: 91vh;
		overflow-y: auto;
	}
}
</style>
