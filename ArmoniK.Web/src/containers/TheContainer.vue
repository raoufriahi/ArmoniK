<template>
  <div class="c-app" :class=" $store.state.darkMode ">
    <TheSidebar/>
    <TheAside/>
    <CWrapper>
      <TheHeader v-if="embed==false"/>
      <div class="c-body">
        <main class="c-main">
          <CContainer fluid>
            <transition name="fade">
              <router-view></router-view>
            </transition>
          </CContainer>
        </main>
        <!-- <TheFooter/> -->
      </div>
    </CWrapper>
		<el-dialog
			:modal="false"
			:show-close="false"
			:visible.sync="dialogVisible"
			width="25%"
			top="2.5vh"
			class="anonymouView">
			<div style="display: flex;justify-content:center;color:#F5EC2B;font-size:16px"><i class="iconfont icon-tixingshixin"></i></div>
			<span style="display: inline-block; width: 100%; text-align: center;">Reminder: Anonymous is turned on, please turn it off as soon as possible, pay attention to network security</span>
			<span slot="footer" class="dialog-footer">
				<router-link :to="{name:'CyberSecurity'}">
					<div style="color:rgba(58, 161, 255, 1); text-align: right;margin-top: 5px;font-size:12px">前往关闭<i class="iconfont icon-qianjin"></i> </div>
				</router-link>
			</span>
		</el-dialog>
  </div>
</template>

<script>
import TheSidebar from './TheSidebar'
import TheHeader from './TheHeader'
import TheFooter from './TheFooter'
import TheAside from './TheAside'

import '../assets/js/adapter'
export default {
	name: 'TheContainer',
	components: {
		TheSidebar,
		TheHeader,
		TheFooter,
		TheAside
	},
	data(){
		return {
			embed:false,
			dialogVisible: false,
		}
	},
	mounted(){
		let _this = this;
		if (this.$route.query.view) {
			this.srcview();
		}
		// this.open();
		const cur_query = this.$route.query || {};
		// console.log(cur_query);
		if(cur_query['embed'] === 'true'){
			this.embed = true;
		}
		this.anonymou();
		_this.$root.bus.$on('Anonymous', function(token){
			_this.anonymou();
		});
	},
	methods:{
		// 是否匿名浏览
		anonymou(){
			var url1 = this.$store.state.IPPORT + "/api/v1/GetAnonymousView?session="+ this.$store.state.token;
			this.$http.get(url1).then(result=>{
				if(result.status==200){
					// this.anonymous=result.data.bAnonymousView
					if (result.data.bAnonymousView==true) {
						this.dialogVisible=true;
					}else{
						this.dialogVisible=false;
					}
				}
			})
		},
		/**open(){
			var conf1 = {
				protocol: window.location.protocol, //http: or https:
				host:this.$store.state.WSROOT, //localhost:8080
				rootpath:'/', // '/'
				callback: this.EventCB, 
				userdata: null, // user data
				session: this.$store.state.token //session got from login
			};
			
			// console.log("*******2",conf1)
			this.e1 = new H5sEvent(conf1);
			this.e1.connect();
		},
		EventCB(event, userdata){
			var msgevent = JSON.parse(event);
			// console.log("****消息",msgevent)
		}*/
		srcview(){
			console.log(this.$store.state.token);
			var url = this.$store.state.IPPORT + "/api/v1/GetView?session="+ this.$store.state.token;
			this.$http.get(url).then(result=>{
				console.log(result);
				if (result.status === 200) {
					// console.log(result)
					var oldarr=result.data.conf;
					for(var i in oldarr){
						if (this.$route.query.view==oldarr[i].strName) {
							this.getview(oldarr[i]);
						}
					}
				}
			})
		},
		getview(arr){
			if(arr.strLayoutType=='1|1'){
			  this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='1|3'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='2|2'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='1|4'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='1|6'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='1|7'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='3|3'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='1|13'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='4|4'){
				this.rows = arr.strLayoutType.split('|')[0];
			}else if(arr.strLayoutType=='5|5'){
				this.rows = arr.strLayoutType.split('|')[0];
			}
			console.log(this.rows);
			// return this.rows;
				setTimeout(() => {
					if (this.$route.query.embed) {
						if (this.$route.query.embed=='true') {
							// this.updateUI(0.98,this.rows);
							if (this.$route.query.fullscreen=="false") {
								this.updateUI(0.89,this.rows);
							}else if (this.$route.query.fullscreen=="true") {
								this.updateUI(0.98,this.rows);
							}
						}else if(this.$route.query.embed=='false'){
							this.updateUI(0.89,this.rows);
						}
						// if (this.$route.query.embed=='true') {
						// 	this.updateUI(0.98,this.rows);
						// }else{
						// 	this.updateUI(0.89,this.rows);
						// }
						// if (this.$route.query.fullscreen=="false") {
						// 	this.updateUI(0.89,this.rows);
						// }else{
						// 	// this.updateUI(0.89,this.rows);
						// }
					}
				},500)
		},
		
		// ui宽度
		updateUI(x,rows) {
			console.log(x,'xxxxxxxxxxxxxxxxxxxxxxxx');
			// console.log(this.rows);
			if($(document.body).width() < 768){
					this.contentHeight = $(document.body).height()*0.4;
			}else{
				this.contentHeight = $(document.body).height()*x;
			}
			$('div[name="flex"]').height(this.contentHeight / rows);
		},
	},
	computed: {
		watchvideo(){
			console.log(this.$route.query);
			return this.$route.query;
		},
	},
	watch: {
		watchvideo(data){
      console.log(data);
			if (this.$route.query) {
				this.srcview();
			}
			if (this.$route.query.embed) {
				if (this.$route.query.embed=='true') {
					this.embed=true;
				}else{
					this.embed=false;
				}
			}else{
				this.embed=false;
			}
		},
	},
}
</script>

<style lang="scss" scoped>
.c-main{
  padding-top: 0px;
  .container-fluid{
    padding: 0;
  }
}
.c-app {
  .anonymouView{
    pointer-events:none;
    /deep/ .el-dialog{
      pointer-events:auto;
			border-radius: 5px;
			.el-dialog__header{
				padding: 0 0 0;
			}
			.el-dialog__body{
				padding: 5px 20px 0;
				.icon-tixingshixin:before {
					font-size: 18px;
				}
			}
			.el-dialog__footer{
				padding: 0 96px 3px;
				a:hover {
					text-decoration: none;
				}
				.icon-qianjin:before {
					font-size: 13px;
				}
			}
    }
  }
}
</style>
<style scoped>
.fade-enter-active,
.fade-leave-active {
  transition: opacity 0.3s;
}
.fade-enter,
.fade-leave-to {
  opacity: 0;
}
</style>
