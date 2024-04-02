<template>
    <div class="top_el_menu">
        <el-menu
            router
            :default-active="activeIndex" 
            class="el-menu-demo" 
            mode="horizontal"
            :background-color="color1"
            :text-color="($store.state.darkMode) ? '#B7B7B7' : '#000'"
            :active-text-color="($store.state.darkMode) ?'#FFFFFF':'#000000'">
            <el-menu-item index="GB28181">{{$t("message.GB.GB28181")}}</el-menu-item>
            <el-menu-item index="GBPlatform">{{$t("message.GB.GBPlatform")}}</el-menu-item>
            <el-menu-item index="GBService">{{$t("message.GB.GBService")}}</el-menu-item>
        </el-menu>
        <div>
            <router-view></router-view>
        </div>
    </div>
</template>

<script>
export default {
    name:'Avintercom',
    data(){
        return {
            color1:'',
            activeIndex:'Advancepb',
        }
    },
    watch:{
		contractFile(data){
		if (data == 'blue') {
			this.color1 = '#0C1637'
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
    mounted(){
        if (this.$store.state.darkMode == 'blue') {
			this.color1 = '#0C1637';
			this.color2 = '#0C1637';
		}else if(this.$store.state.darkMode == 'c-dark-theme'){
			this.color1 = '#0c0c0c'
		}else{
			this.color1 = '#f5f5f5'
		}
        this.menuList();
    },
    methods:{
        menuList(){ 
			let path = this.$route.meta.title
			// console.log(this.$route)
            this.activeIndex = path
        }
    }
}
</script>

<style lang="scss" scoped>

</style>