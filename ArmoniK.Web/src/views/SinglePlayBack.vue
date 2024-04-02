<template>
	<div class="playback">
		<div class="playback_snap_you">
			<div id="Fullscreen">
				<div  id="videoPanel" class="videogh1">
					<div name='flex' style="position: relative;" class="videoColor"  v-for="r in rows" :key="r">
						<div 
							v-for="c in cols" 
							:key="c" 
							v-bind:id="'h'+r+c"
							class="palace" 
							name="flex" 
							@contextmenu.prevent="stopVideo($event)" 
							@click="videoClick(r,c,$event)"
							@mouseover="hideover(r,c)"
							@mouseout="showout(r,c)" >
							<avintercoms-play :h5id="'h'+r+c" :h5videoid="'gaovideohb'+r+c"></avintercoms-play>
							<div class="h5controls" style="display:none padding:0px;">
								<div style="display: flex;justify-content: flex-end;">
									<button :id="rtcid" class="iconfont icon-bofang open_button rotate" @click="open_liveplay_butt" style="font-size:12px;line-height: 14px;"></button>
									<button class="open_liveplay_butt" type="text" v-if="$store.state.liveviewrtc1 == 'RTC'">RTC</button>
									<button class="open_liveplay_butt" type="text" v-else>WS</button>
								<!-- <button type="button" class="vidbuttion pull-right iconfont icon-roundclosefill" @click="CloseVideo($event)"></button> -->
									<button type="button" class="vidbuttion pull-right iconfont icon-full" @click="panelFullScreen($event)"></button>
								</div>
							</div>
						</div>
					</div>
				</div>
				<div style="width:100%;height:14px;position:absolute;display:flex;justify-content: space-between;padding:0 5px;">
					<div style="line-height:12px; cursor:pointer;"> 
						<el-button class="iconfont icon-jian fuhao" @click="jian()"></el-button> 
						<el-button class="iconfont icon-jia fuhao" @click="jia()"></el-button> 
					</div>
					<div style="line-height:12px; cursor:pointer;"> 
						<el-button class="iconfont icon-jia fuhao" @click="jia()"></el-button> 
						<el-button class="iconfont icon-jian fuhao" @click="jian()"></el-button> 
					</div>
				</div>
				<div v-for="r in rows" :key="r">
					<div v-for="c in cols" :key="c">
						<div class="tab" :id="'tab'+r+c" >
							<div style="display: flex;justify-content: center;">
								<canvas class="timeline" :id="'timeline'+r+c" style="width:100%" width="1900" height="55px"
								 	@mousedown ="timelinndown($event)"
									@mouseout="mouseoutcanvas"
									@mouseup="timetz"
									ondragstart="return false;">
								</canvas>
							</div>
							<div style="width:100%;display: flex;justify-content: space-between;">
								<div class="mode_Choice">
									<div class="back_Choice"  @mouseenter="enter1()" @mouseleave="leave1()">
										<div id="back_Choice" class="back_Choice1">
											<div :class="{co_Baise:Adswitch=='true'}" @click="Adswitchs" class="co_black" >{{$t("message.playback.H5SPlayback")}}</div>
											<div :class="{co_Baise:Adswitch=='false'}" @click="Adswitchs1" class="co_black">{{$t("message.playback.NVRPlayback")}}</div>
										</div>
										<span class="back_zi iconfont icon-gengduo"></span>
										<span v-if="Adswitch=='true'" class="">&nbsp;&nbsp;{{$t("message.playback.H5SPlayback")}}</span> 
										<span v-else class="">&nbsp;&nbsp;{{$t("message.playback.NVRPlayback")}}</span>
									</div>
								</div>
								<div class="function_butt" style="display: flex;align-items:center;justify-content:center;">
									<!-- <button type="button" class="layout layout1x1" data-row="1|1" @click="changePanel($event)"></button>
									<button type="button" class="layout layout2x2" data-row="2|2" @click="changePanel($event)"></button>
									<button type="button" class="layout layout3x3" data-row="3|3" @click="changePanel($event)"></button> -->
									<i style="font-size: 24px;color:#B2B1B1;margin-right:20px" class="layoutfull" @click="panelFullScreen($event)"></i>
									<!-- <el-date-picker
										class="fixed_input"
										v-model="xzvalue"
										size="mini"
										@change="input_ch"
										type="date"
										placeholder="选择日期时间"
										default-time="00:00:00">
									</el-date-picker> -->
									<el-select filterable remote v-if="r=='1'&&c=='1'" v-model="region" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='1'&&c=='2'" v-model="region1" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='2'&&c=='1'" v-model="region2" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='2'&&c=='2'" v-model="region3" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='1'&&c=='3'" v-model="region4" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='2'&&c=='3'" v-model="region5" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='3'&&c=='1'" v-model="region6" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='3'&&c=='2'" v-model="region7" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<el-select filterable remote v-if="r=='3'&&c=='3'" v-model="region8" size="mini" class="ele" :popper-append-to-body="false" popper-class='selectdrop' style="width:70px" @change="timeSpeed()">
										<el-option
											v-for="item in regiondata"
											:key="item.value"
											:label="item.label"
											:value="item.value">
										</el-option>
									</el-select>
									<button v-if="r=='1'&&c=='1'" type="button" :class="icon" @click="resume()" class="button_resume"></button>
									<button v-if="r=='1'&&c=='2'" type="button" :class="icon1" @click="resume()" class="button_resume"></button>
									<button v-if="r=='2'&&c=='1'" type="button" :class="icon2" @click="resume()" class="button_resume"></button>
									<button v-if="r=='2'&&c=='2'" type="button" :class="icon3" @click="resume()" class="button_resume"></button>
									<button v-if="r=='1'&&c=='3'" type="button" :class="icon" @click="resume()" class="button_resume"></button>
									<button v-if="r=='2'&&c=='3'" type="button" :class="icon1" @click="resume()" class="button_resume"></button>
									<button v-if="r=='3'&&c=='1'" type="button" :class="icon2" @click="resume()" class="button_resume"></button>
									<button v-if="r=='3'&&c=='2'" type="button" :class="icon3" @click="resume()" class="button_resume"></button>
									<button v-if="r=='3'&&c=='3'" type="button" :class="icon" @click="resume()" class="button_resume"></button>
								</div>
								<div class="caveat_butt" style="">
										<button class="mr-0" type="button"></button> {{$t("message.archive.Schedulerecord")}}
										<button class="mr-1" type="button"></button>{{$t("message.archive.ManualRecord")}}
										<button class="mr-2" type="button"></button>{{$t("message.archive.AlarmRecord")}}
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</template>
<script>
import Vue from 'vue'
import {listdatagload,listdatag} from './public/regional'
import '../assets/js/adapter'
import {H5sPlayerWS,H5sPlayerHls,H5sPlayerRTC,H5sPlayerAudBack} from '../assets/js/h5splayer.js'
import {H5siOS,H5sPlayerCreate} from '../assets/js/h5splayerhelper.js'
import './singleplayback/timeline-canvas1'
// import './public/timeline-canvas1'
import Avintercomsplay from './singleplayback/Avintercomsplay.vue'
export default {
    name:"Advancepb",
    components: {
			'avintercoms-play': Avintercomsplay
    },
    data() {
			return {
				starttimes:'',
				endtimes:'',
				defaultProps: {
					children: this.$store.state.root=="Operator"?'cam':'children',
					label: this.$store.state.root=="Operator"?'strName':'label',
					token:this.$store.state.root=="Operator"?'strToken':"token",
					iconclass:"iconclass",
					online:'online',
				},
				filterText: '',
				data: listdatag,
				demoEvents: [],
				//过滤文字
				rows: 1,
				cols: 1,
				selectCol: 1,
				selectRow: 1,
				selectCol1: 1,
				selectRow1: 1,
				contentHeight: '',
				contentWidth: '',
				region:'X1',//倍速
				region1:'X1',//倍速
				region2:'X1',//倍速
				region3:'X1',//倍速
				region4:'X1',//倍速
				region5:'X1',//倍速
				region6:'X1',//倍速
				region7:'X1',//倍速
				region8:'X1',//倍速
				regiondata:[
					{
						value: "0.5",
						label: "X0.5"
					},{
						value: "1.0",
						label: "X1"
					},{
						value: "2.0",
						label: "X2"
					},{
						value: "4.0",
						label: "X4"
					},{
						value: "8.0",
						label: "X8"
					},{
						value: "16.0",
						label: "X16"
				}],
				icon:"iconfont icon-bofang",//暂停图片
				icon1:"iconfont icon-bofang",//暂停图片
				icon2:"iconfont icon-bofang",//暂停图片
				icon3:"iconfont icon-bofang",//暂停图片
				Adswitch:this.$store.state.Adswitch,//开关
				proto: 'WS',
				//进度条
				value:new Date(),
				xzvalue: new Date(),//双向日历
				timedata:[],//数据数组
				timecell:[],//滚动条数组
				v1:undefined,
				v2:undefined,
				v3:undefined,
				v4:undefined,
				v5:undefined,
				v6:undefined,
				v7:undefined,
				v8:undefined,
				v9:undefined,
				Gtoken:"",//全局点击节点后token
				SGtoken:["","","","","","",'','',''],//四宫点击节点后token
				id:"",
				PlaybackCBinterval:'',
				PlaybackCBinterval1:'',
				PlaybackCBinterval2:'',
				PlaybackCBinterval3:'',
				PlaybackCBinterval4:'',
				PlaybackCBinterval5:'',
				PlaybackCBinterval6:'',
				PlaybackCBinterval7:'',
				PlaybackCBinterval8:'',
				refNamePopover: 'popover-', // popover ref名称前缀
				rtcid:'',
			}
    },
    beforeDestroy() {
			this.handleClose();
			// this.$root.bus.$off('liveplay');
    },
    mounted(){
			$('.c-header').hide();
			if(this.$store.state.root=="Operator"){
				this.Regional()
			}
			setTimeout(()=>{
				this.updateUI();
			},1000)
			// if (this.$route.query.session) {
				if (this.$route.query.token&&this.$route.query.starttime) {
					this.Play(this.$route.query.token,this.$route.query.starttime,this.$route.query.endtime)
				}
				
			// }
			// this.goolsh();
			this.functlist();
			this.funtimeine();
			var _this=this
			this.$root.bus.$on('SinglePlayBack',function(data){
				// this.xzvalue = data.xzvalue;
					_this.Play(data.token,data.xzvalue,data.endxzvalue)
				})
			window.onresize = () => {
				console.log("aaa1")
				// 全屏下监控是否按键了ESC
				if (!_this.checkFull()) {
					// 全屏下按键esc后要执行的动作
					$(".layout").css("display",  "block");
					// this.isFullscreen = false
				}
			}
			this.contentHeight = $('.videogh1').height();
			$('div[name="flex"]').height(this.contentHeight / this.rows);
    },
    methods:{
			// 展开或收缩liveplay上面的button按钮
			open_liveplay_butt(){
				if ($("#"+this.rtcid).attr("class")=="iconfont icon-bofang open_button rotate") {
					$("#"+this.rtcid).siblings('.open_liveplay_butt').css("display", "block");
					$("#"+this.rtcid).removeClass('rotate');
					this.$nextTick(()=>{
						$("#"+this.rtcid).parent().parent('.h5controls').css('background-size', '125px 35px');
					})
				}else{
					$("#"+this.rtcid).siblings('.open_liveplay_butt').css("display", "none");
					$("#"+this.rtcid).addClass('rotate');
					$("#"+this.rtcid).parent().parent('.h5controls').css('background-size', '90px 35px');
				}
			},
			// 在线离线状态提示
			StatusPrompt(id,e){
				let refName = this.refNamePopover + id;
				if (this.$refs[refName].showPopper) {
					this.$refs[refName].doClose();
					for (let i = 0; i < e.currentTarget.classList.length; i++) {
						e.currentTarget.classList.remove('StatusPromptColor')
						e.stopPropagation();
						return;
					}
				}else{
					this.$refs[refName].doShow();
					for (let i = 0; i < e.currentTarget.classList.length; i++) {
						e.currentTarget.classList.add('StatusPromptColor')
						e.stopPropagation();
						return;
					}
				}
			},
			Regional(){
				var root = this.$store.state.IPPORT;
				var url = root + "/api/v1/GetRegion?session="+ this.$store.state.token;
				this.$http.get(url).then(result=>{
					console.log(result);
					var oldarr=result.data.root;
					var oldarr1=result.data.src;
					// console.log(oldarr,oldarr1)
					var dataroot=this.getchild(oldarr,oldarr1);
					// this.data1.push(dataroot);
					console.log(dataroot);
					this.datapush(dataroot);
					console.log(this.data1);
				})
			},
			//重新组装 tree 里面的数据
			datapush(data){
				this.a(data)
			},
			a(data){//第一级
				console.log(data);
				if(data.node.length!=0){
					for (let i = 0; i < data.node.length; i++) {
						data.cam.push(data.node[i])
					}
				}
				delete data.node
				for (let i = 0; i < data.cam.length; i++) {
					console.log(data.cam[i].strName);
					if (data.cam[i].strName!='设备暂时不可用') {
						this.b(data.cam[i])
						this.data=[];
						this.data.push(data);
					}
				}
			},
			b(data){//第二级
				if (data.name) {
					return
				}
				if(data.node.length!=0){
					for (let i = 0; i < data.node.length; i++) {
						data.cam.push(data.node[i])
					}
				}
				delete data.node
				for (let i = 0; i < data.cam.length; i++) {
					if (data.cam[i].strName!='设备暂时不可用') {
						console.log(data.cam[i]);
						if (data.cam[i].cam) {
							this.c(data.cam[i])
						}
					}
				}
			},
			c(data){//第三级
				if (data.name) {
					return
				}
				if(data.node.length!=0){
					for (let i = 0; i < data.node.length; i++) {
						data.cam.push(data.node[i])
					}
				}
				delete data.node;
				for (let i = 0; i < data.cam.length; i++) {
					if (data.cam[i].strName!='设备暂时不可用') {
						console.log(data.cam[i]);
						if (data.cam[i].cam) {
							this.d(data.cam[i])
						}
					}
				}
			},
			d(data){//第四级
				if (data.name) {
					return
				}
				if(data.node.length!=0){
					for (let i = 0; i < data.node.length; i++) {
						data.cam.push(data.node[i])
					}
				}
				delete data.node;
				for (let i = 0; i < data.cam.length; i++) {
					if (data.cam[i].strName!='设备暂时不可用') {
					console.log(data.cam[i]);
					if (data.cam[i].cam) {
						this.e(data.cam[i])
					}}
				}
			},
			e(data){//第五级
				if (data.name) {
					return
				}
				if(data.node.length!=0){
					for (let i = 0; i < data.node.length; i++) {
						if (data.cam[i].strName!='设备暂时不可用') {
							data.cam.push(data.node[i])
						}
					}
				}
				delete data.node;
			},
			getchild(arr,arr1) {
				for(var i in arr.cam){
					if(!arr.cam[i].strName){
						for(var j in arr1){
							if(arr.cam[i].strToken == arr1[j].strToken){
								var node1=[{
									strToken : arr1[j].strToken,
									streamprofile : "main",
									strName :this.$t('message.live.mainstream'),
									name:arr1[j].strName+"--"+this.$t('message.live.mainstream'),
									iconclass : 'mdi mdi-playlist-play fa-fw',
									disabled_me:false
								},{
									strToken : arr1[j].strToken,
									streamprofile : "sub",
									strName :this.$t('message.live.substream'),
									name:arr1[j].strName+"--"+this.$t('message.live.substream'),
									iconclass : 'mdi mdi-playlist-play fa-fw',
									disabled_me:false
								}]
								arr.cam[i].node=node1;
								arr.cam[i].strName = arr1[j].strName;
								arr.cam[i].name=arr1[j].strName+"--"+this.$t('message.live.mainstream')
								arr.cam[i].bOnline = arr1[j].bOnline;
								arr.cam[i].iconclass="iconfont  icon-shexiangji"
								arr.cam[i].disabled_me=false
								if(!arr1[j].bOnline){
									// arr.cam[i].iconclass = 'iconfont icon-kaiqishexiangtou';
									arr.cam[i].iconclass = 'iconfont icon-shexiangji el-tree-camera';
									arr.cam[i].iconclass1 = 'el-tree-camera';
								}

								if(arr1[j].nConnectType == 'H5_CLOUD')
								arr.cam[i].iconclass = 'iconfont  icon-shexiangji';

								if(arr1[j].bRec == true)
								// arr.cam[i].iconclass2  = 'iconfont icon-radioboxfill none';
								arr.cam[i].iconclass2= 'none';

								if(arr1[j].bDisable== true){
									arr.cam[i].node[0].disabled_me=true;
									arr.cam[i].node[1].disabled_me=true;
									arr.cam[i].disabled_me=true;
									// arr.cam[i].iconclass1= 'camera';
									arr.cam[i].iconclass= 'iconfont icon-jinyong1 el-tree-camera';
									arr.cam[i].iconclass1 = 'el-tree-camera';
								}
								if (arr1[j].bIdle == true) {
									arr.cam[i].iconclass = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
									arr.cam[i].iconclass1 = 'el-tree-camera';
								}
							}
						}
					}
				}
			var camarr=[]
			var camarr1=[]
			for(var i in arr.cam){
				camarr.push(arr.cam[i].strToken)
			}
			for(var i in arr1){
				camarr1.push(arr1[i].strToken)
			}
			let diff = camarr.filter(item => !new Set(camarr1).has(item))
			// console.log(diff)
			for(var i in arr.cam){
				for(var j in diff){
					if(arr.cam[i].strToken == diff[j]){
						arr.cam[i].strName = '设备暂时不可用';
						// arr.cam[i].iconclass="iconfont  icon-kaiqishexiangtou"
						arr.cam[i].iconclass = 'iconfont icon-shexiangji el-tree-camera';
						arr.cam[i].iconclass1 = 'el-tree-camera';
					}
				}
			}
			if(arr.node && arr.node.length>0){
				for (var i = 0; i < arr.node.length; i++) {
					arr.node[i] = this.getchild(arr.node[i],arr1);
				}
			}
			return arr;
		},
		Adswitchs(){
			var open="true";
			this.Adswitch=open;
			console.log(open,"111")
			this.$store.state.Adswitch=open
			sessionStorage.Adswitch=this.$store.state.Adswitch
		},
		Adswitchs1(){
			var open="false";
			this.Adswitch=open;
			console.log(open,"222")
			this.$store.state.Adswitch=open
			sessionStorage.Adswitch=this.$store.state.Adswitch
		},
		//显示第一个数轴
		functlist(){
			$('.tab').each(function (e) {
			if (e == 0)
				$(this).show();//首次加载的时候默认显示第一个
			else
				$(this).hide();
			});
		},
		
		//点击确定键
		input_ch(){
			// var data=this.Gtoken;
			var data = this.$route.query.token;
			// console.log("日历",data,this.Gtoken,this.Adswitch);
			// return false;
			this.Play(data);
		},
		//树形节点点击
		//开始播放时间  0：00 每天结束时间可能回到第二天清晨
		handleNodeClick(data, checked, indeterminate){
			if(data.disabled_me){
				return false
			}
			var data=data.token||data.strToken;
			if(data==undefined){
				return false;
			}
			if(this.selectRow=="1"&&this.selectCol=="1"){
				this.SGtoken.splice(0, 1,data);
				this.Gtoken=this.SGtoken[0];
			}else if(this.selectRow=="1"&&this.selectCol=="2"){
				this.SGtoken.splice(1, 1,data);
				this.Gtoken=this.SGtoken[1];
			}else if(this.selectRow=="2"&&this.selectCol=="1"){
				this.SGtoken.splice(2, 1,data);
				this.Gtoken=this.SGtoken[2];
			}else if(this.selectRow=="2"&&this.selectCol=="2"){
				this.SGtoken.splice(3, 1,data);
				this.Gtoken=this.SGtoken[3];
			}else if(this.selectRow=="1"&&this.selectCol=="3"){
				this.SGtoken.splice(4, 1,data);
				this.Gtoken=this.SGtoken[4];
			}else if(this.selectRow=="2"&&this.selectCol=="3"){
				this.SGtoken.splice(5, 1,data);
				this.Gtoken=this.SGtoken[5];
			}else if(this.selectRow=="3"&&this.selectCol=="1"){
				this.SGtoken.splice(6, 1,data);
				this.Gtoken=this.SGtoken[6];
			}else if(this.selectRow=="3"&&this.selectCol=="2"){
				this.SGtoken.splice(7, 1,data);
				this.Gtoken=this.SGtoken[7];
			}else if(this.selectRow=="3"&&this.selectCol=="3"){
				this.SGtoken.splice(8, 1,data);
				this.Gtoken=this.SGtoken[8];
			}
			// return false;
			this.Play(data);
		},
		mouseoutcanvas(){
			console.log("11111");
			return
		},
		//拉播
		timelinndown(err){
			if(this.selectRow=="1"&&this.selectCol=="1"){
				if(this.v1 != undefined){
					this.v1.pause();
					clearInterval(this.PlaybackCBinterval);
				}
			}else if(this.selectRow=="1"&&this.selectCol=="2"){
				if(this.v2 != undefined){
					this.v2.pause();
					clearInterval(this.PlaybackCBinterval1);
				}
			}else if(this.selectRow=="2"&&this.selectCol=="1"){
				if(this.v3 != undefined){
					this.v3.pause();
					clearInterval(this.PlaybackCBinterval2);
				}
			}else if(this.selectRow=="2"&&this.selectCol=="2"){
				if(this.v4 != undefined){
					this.v4.pause();
					clearInterval(this.PlaybackCBinterval3);
				}
			}else if(this.selectRow=="1"&&this.selectCol=="3"){
				if(this.v5 != undefined){
					this.v5.pause();
					clearInterval(this.PlaybackCBinterval4);
				}
			}else if(this.selectRow=="2"&&this.selectCol=="3"){
				if(this.v6 != undefined){
					this.v6.pause();
					clearInterval(this.PlaybackCBinterval5);
				}
			}else if(this.selectRow=="3"&&this.selectCol=="1"){
				if(this.v7 != undefined){
					this.v7.pause();
					clearInterval(this.PlaybackCBinterval6);
				}
			}else if(this.selectRow=="3"&&this.selectCol=="2"){
				if(this.v8 != undefined){
					this.v8.pause();
					clearInterval(this.PlaybackCBinterval7);
				}
			}else if(this.selectRow=="3"&&this.selectCol=="3"){
				if(this.v9 != undefined){
					this.v9.pause();
					clearInterval(this.PlaybackCBinterval8);
				}
			}
		},
		timetz(){
			console.log('111')
			var timevalue=this.xzvalue;
			var _this=this;
			_this.timedata=[];
			// return false
			var root = this.$store.state.IPPORT;
			var wsroot = this.$store.state.WSROOT;
			this.Gtoken=this.$route.query.token;
			if(_this.Gtoken==undefined||_this.Gtoken==""){
				return false;
			}
			setTimeout(function(){
				var a = $("#timeline"+_this.selectRow+_this.selectCol).TimeSlider('returnMouseupTime',null,null,function(time){
					console.log("time",time,_this.Gtoken)
					// return false;
					// 放入视频
					// return false;
					var timevalue=new Date(time);
					console.log(timevalue)
					var year = timevalue.getFullYear();
					var month = timevalue.getMonth() + 1;
					var strDate = timevalue.getDate();
					var strDate1 = timevalue.getDate()+1;
					var strDate2 = timevalue.getDate()-1;
					var getHours = timevalue.getHours();
					var getMinutes = timevalue.getMinutes();
					var getSeconds = timevalue.getSeconds();
					var localOffset = Math.abs(timevalue.getTimezoneOffset() /60);
					if (month<10) {
						month = '0'+month;
					}
					if (strDate<10) {
						strDate = '0'+strDate;
					}
					if (strDate1<10) {
						strDate1 = '0'+strDate1;
					}
					if (strDate2<10) {
						strDate2 = '0'+strDate2;
					}
					if (getHours<10) {
						getHours = '0'+getHours;
					}
					if (getMinutes<10) {
						getMinutes = '0'+getMinutes;
					}
					if (getSeconds<10) {
						getSeconds = '0'+getSeconds;
					}
					var timevalues=year+"-"+month+"-"+strDate+"T"+""+getHours+":"+getMinutes+":"+getSeconds+""+"+0"+localOffset+":00";
					
					var timevaluee=year+"-"+month+"-"+strDate+"T"+"23:59:59"+"+0"+localOffset+":00";

					var timevalues1=year+"-"+month+"-"+strDate2+"T"+""+getHours+":"+getMinutes+":"+getSeconds+""+"+0"+localOffset+":00";
					
					var timevaluee1=year+"-"+month+"-"+strDate1+"T"+"23:59:59"+"+0"+localOffset+":00";
					
					// console.log("======",strDate1);
					// console.log("timevaluee222222",timevalues,timevaluee,"------",localOffset,"**",timevalue);
					var url=""
					if(_this.Adswitch=="false"){
						url = url = root + "/api/v1/SearchDeviceRecordByTime?token="+_this.Gtoken+"&start="+encodeURIComponent(timevalues1)+"&end="+encodeURIComponent(timevaluee1)+"&session="+ _this.$store.state.token;
					}else{
						url = root + "/api/v1/Search?type=record&token="+_this.Gtoken
						+"&start="+encodeURIComponent(timevalues1)+"&end="+encodeURIComponent(timevaluee)+"&session="+ _this.$store.state.token;
					}
					// console.log(url);
					//  return false;
					_this.$http.get(url).then(result=>{
						let tim0 = new Date(timevalues).getTime();
						let tim1;
						if(result.status == 200){
							var data=result.data;
							console.log(data);
							var timedata1=[];
							//console.log("length",data.record.length);
							for(var i=0;i<data.record.length;i++){
								var item1
								if (i < data.record.length-1) {
									item1=data.record[i+1];
								}
								var item=data.record[i];
								//时间转换
								var starf=new Date(item['strStartTime']).getTime();
								var starf1=new Date(item1['strStartTime']).getTime();
								var end=new Date(item['strEndTime']).getTime();
								if (end<tim0&&starf1>tim0) {
									timevalues = item1['strStartTime'];
									console.log('没有视频');
								}
								var starf=new Date(starf);
								var end=new Date(end);
								var timeitem={
										beginTime :starf,
										endTime :end,
										style:{background:"rgba(60,196,60, 0.498039)"}
									};
								//console.log("录像段时间段颜色",timeitem["style"].background); //录像段时间段颜色
								if(item["nType"]==="H5_REC_MANUAL"){
									timeitem["style"].background="rgba(60,196,60, 0.498039)"
									//console.log("录像段时间段颜色1",timeitem["style"].background);
								}else if(item["nType"]==="H5_REC_SCHEDULE"){
									timeitem["style"].background="#31b1ff"
								}else{
									timeitem["style"].background="rgba(238,17,17, 0.498039)"
									//console.log("录像段时间段颜色2",timeitem["style"].background);
								}
								// _this.timedata.push(timeitem);
								timedata1.push(timeitem);
								// console.log(timedata1)
								
								$("#timeline"+_this.selectRow+_this.selectCol).TimeSlider('init',timevalue,timedata1);
								
							}
							if (tim0>new Date(data.record[data.record.length-1].strEndTime).getTime()) {
								timevalues = data.record[data.record.length-1].strStartTime;
								$("#timeline"+_this.selectRow+_this.selectCol).TimeSlider('init',timevalues,timedata1);
							}
							tim1=(data.record[data.record.length-1].strStartTime).split('T')
						}
						if ( new Date(timevalues).getTime()>new Date(timevaluee).getTime()) {
							timevaluee=tim1+"T"+"23:59:59"+"+0"+localOffset+":00";
						}
						if(_this.selectRow=="1"&&_this.selectCol=="1"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v1 != undefined)
							{
								_this.v1.disconnect();
								delete _this.v1;
								_this.v1 = undefined;
								console.log("上this.v1",_this.v1);
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							console.log(pbconf1,'911111111122222222222222211111111');
							let conf = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v2 is for fmp4
								session: _this.$store.state.token
							};
							_this.$nextTick (()=>{
								_this.v1 = new H5sPlayerRTC(conf);
								//return false;
								_this.v1.connect();
								_this.icon="iconfont icon-zantingtingzhi";
								setTimeout(()=>{
									_this.timeSpeed();
								},1000)
							})
						}else if(_this.selectRow=="1"&&_this.selectCol=="2"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v2 != undefined)
							{
								_this.v2.disconnect();
								delete _this.v2;
								_this.v2 = undefined;
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB1,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf2 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v2 is for fmp4
								session: _this.$store.state.token
							};
							
							_this.$nextTick (()=>{
								_this.v2 = new H5sPlayerRTC(conf2);
								//return false;
								_this.v2.connect();
								_this.icon="iconfont icon-zantingtingzhi";
								setTimeout(()=>{
									_this.timeSpeed();
								},1000)
							})
						}else if(_this.selectRow=="2"&&_this.selectCol=="1"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v3 != undefined)
							{
								_this.v3.disconnect();
								delete _this.v3;
								_this.v3 = undefined;
								console.log("上this.v1",_this.v1);
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB2,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf3 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v2 is for fmp4
								session: _this.$store.state.token
							};
							
							_this.$nextTick (()=>{
								_this.v3 = new H5sPlayerRTC(conf3);
								//return false;
								_this.v3.connect();
								_this.icon="iconfont icon-zantingtingzhi";
								setTimeout(()=>{
									_this.timeSpeed();
								},1000)
							})
						}else if(_this.selectRow=="2"&&_this.selectCol=="2"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v4 != undefined)
							{
								_this.v4.disconnect();
								delete _this.v4;
								_this.v4 = undefined;
								console.log("上this.v1",_this.v1);
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB3,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf4 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v2 is for fmp4
								session: _this.$store.state.token
							};
							_this.$nextTick (()=>{
								_this.v4 = new H5sPlayerRTC(conf4);
								//return false;
								_this.v4.connect();
								_this.icon="iconfont icon-zantingtingzhi";
								setTimeout(()=>{
									_this.timeSpeed();
								},1000)
							})
						}else if(_this.selectRow=="1"&&_this.selectCol=="3"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v5 != undefined)
							{
								_this.v5.disconnect();
								delete _this.v5;
								_this.v5 = undefined;
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB4,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf2 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v5 is for fmp4
								session: _this.$store.state.token
							};
							
							_this.$nextTick (()=>{
								_this.v5 = new H5sPlayerRTC(conf2);
								//return false;
								_this.v5.connect();
								_this.icon="iconfont icon-zantingtingzhi";
							})
						}else if(_this.selectRow=="2"&&_this.selectCol=="3"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v6 != undefined)
							{
								_this.v6.disconnect();
								delete _this.v6;
								_this.v6 = undefined;
								console.log("上this.v1",_this.v1);
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB5,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf3 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v2 is for fmp4
								session: _this.$store.state.token
							};
							
							_this.$nextTick (()=>{
								_this.v6 = new H5sPlayerRTC(conf3);
								//return false;
								_this.v6.connect();
								_this.icon="iconfont icon-zantingtingzhi";
							})
						}else if(_this.selectRow=="3"&&_this.selectCol=="1"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v7 != undefined)
							{
								_this.v7.disconnect();
								delete _this.v7;
								_this.v7 = undefined;
								console.log("上this.v1",_this.v1);
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB6,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf4 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v2 is for fmp4
								session: _this.$store.state.token
							};
							_this.$nextTick (()=>{
								_this.v7 = new H5sPlayerRTC(conf4);
								//return false;
								_this.v7.connect();
								_this.icon="iconfont icon-zantingtingzhi";
							})
						}else if(_this.selectRow=="3"&&_this.selectCol=="2"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v8 != undefined)
							{
								_this.v8.disconnect();
								delete _this.v8;
								_this.v8 = undefined;
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB7,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf2 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v8 is for fmp4
								session: _this.$store.state.token
							};
							
							_this.$nextTick (()=>{
								_this.v8 = new H5sPlayerRTC(conf2);
								//return false;
								_this.v8.connect();
								_this.icon="iconfont icon-zantingtingzhi";
							})
						}else if(_this.selectRow=="3"&&_this.selectCol=="3"){
							_this.selectCol1 = _this.selectCol;
							_this.selectRow1 = _this.selectRow;
							if (_this.v9 != undefined)
							{
								_this.v9.disconnect();
								delete _this.v9;
								_this.v9 = undefined;
								console.log("上this.v1",_this.v1);
							}
							var pbconf1 = {
								begintime: timevalues,
								endtime: timevaluee,
								autoplay: 'true',
								showposter:"false", //'true' or 'false' show poster
								callback: _this.PlaybackCB8,
								serverpb: _this.Adswitch, 
								userdata:  _this // user data
							};
							let conf3 = {
								videoid: "gaovideohb"+_this.selectRow+_this.selectCol,
								protocol: window.location.protocol, //http: or https:
								host: wsroot, //localhost:8080
								rootpath:'/', // '/'
								token:_this.Gtoken,
								pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
								hlsver:'v1', //v1 is for ts, v2 is for fmp4
								session: _this.$store.state.token
							};
							
							_this.$nextTick (()=>{
								_this.v9 = new H5sPlayerRTC(conf3);
								//return false;
								_this.v9.connect();
								_this.icon="iconfont icon-zantingtingzhi";
							})
						}
					})
					
				})
			},100);
		},
		//开始
		resume(){
			var strart=this.icon;
			var strart1=this.icon1;
			var strart2=this.icon2;
			var strart3=this.icon3;
			console.log(strart);
			if(this.selectRow=="1"&&this.selectCol=="1"){
				if(this.v1 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart=="iconfont icon-zantingtingzhi"){
						this.icon="iconfont icon-bofang";
						console.log(this.icon);
						this.v1.pause();
					}
					if(strart=="iconfont icon-bofang"){
						this.icon="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v1.resume();
					}
				}
			}else if(this.selectRow=="1"&&this.selectCol=="2"){
				if(this.v2 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart1=="iconfont icon-zantingtingzhi"){
						this.icon1="iconfont icon-bofang";
						console.log(this.icon);
						this.v2.pause();
					}
					if(strart1=="iconfont icon-bofang"){
						this.icon1="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v2.resume();
					}
				}
			}else if(this.selectRow=="2"&&this.selectCol=="1"){
				if(this.v3 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart2=="iconfont icon-zantingtingzhi"){
						this.icon2="iconfont icon-bofang";
						console.log(this.icon);
						this.v3.pause();
					}
					if(strart2=="iconfont icon-bofang"){
						this.icon2="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v3.resume();
					}
				}
			}else if(this.selectRow=="2"&&this.selectCol=="2"){
				if(this.v4 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart3=="iconfont icon-zantingtingzhi"){
						this.icon3="iconfont icon-bofang";
						console.log(this.icon);
						this.v4.pause();
					}
					if(strart3=="iconfont icon-bofang"){
						this.icon3="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v4.resume();
					}
				}
			}else if(this.selectRow=="1"&&this.selectCol=="3"){
				if(this.v5 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart1=="iconfont icon-zantingtingzhi"){
						this.icon1="iconfont icon-bofang";
						console.log(this.icon);
						this.v5.pause();
					}
					if(strart1=="iconfont icon-bofang"){
						this.icon1="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v5.resume();
					}
				}
			}else if(this.selectRow=="2"&&this.selectCol=="3"){
				if(this.v6 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart2=="iconfont icon-zantingtingzhi"){
						this.icon2="iconfont icon-bofang";
						console.log(this.icon);
						this.v6.pause();
					}
					if(strart2=="iconfont icon-bofang"){
						this.icon2="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v6.resume();
					}
				}
			}else if(this.selectRow=="3"&&this.selectCol=="1"){
				if(this.v7 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart3=="iconfont icon-zantingtingzhi"){
						this.icon3="iconfont icon-bofang";
						console.log(this.icon);
						this.v7.pause();
					}
					if(strart3=="iconfont icon-bofang"){
						this.icon3="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v7.resume();
					}
				}
			}else if(this.selectRow=="3"&&this.selectCol=="2"){
				if(this.v8 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart1=="iconfont icon-zantingtingzhi"){
						this.icon1="iconfont icon-bofang";
						console.log(this.icon);
						this.v8.pause();
					}
					if(strart1=="iconfont icon-bofang"){
						this.icon1="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v8.resume();
					}
				}
			}else if(this.selectRow=="3"&&this.selectCol=="3"){
				if(this.v9 != undefined){
					//iconfont icon-zantingtingzhi  iconfont icon-bofang
					if(strart2=="iconfont icon-zantingtingzhi"){
						this.icon2="iconfont icon-bofang";
						console.log(this.icon);
						this.v9.pause();
					}
					if(strart2=="iconfont icon-bofang"){
						this.icon2="iconfont icon-zantingtingzhi";
						console.log(this.icon);
						this.v9.resume();
					}
				}
			}
		},
		// 拖动/倍速
		timeSpeed(){
			let _this= this;
			if(_this.selectRow=="1"&&_this.selectCol=="1"){
				if(_this.v1 != undefined){
					_this.v1.speed(_this.region);
				}
			}else if(_this.selectRow=="1"&&_this.selectCol=="2"){
				if(_this.v2 != undefined){
					_this.v2.speed(_this.region1);
				}
			}else if(_this.selectRow=="2"&&_this.selectCol=="1"){
				if(_this.v3 != undefined){
					_this.v3.speed(_this.region2);
				}
			}else if(_this.selectRow=="2"&&_this.selectCol=="2"){
				if(_this.v4 != undefined){
					_this.v4.speed(_this.region3);
				}
			}else if(_this.selectRow=="1"&&_this.selectCol=="3"){
				if(_this.v5 != undefined){
					_this.v5.speed(_this.region4);
				}
			}else if(_this.selectRow=="2"&&_this.selectCol=="3"){
				if(_this.v6 != undefined){
					_this.v6.speed(_this.region5);
				}
			}else if(_this.selectRow=="3"&&_this.selectCol=="1"){
				if(_this.v7 != undefined){
					_this.v7.speed(_this.region6);
				}
			}else if(_this.selectRow=="3"&&_this.selectCol=="2"){
				if(_this.v8 != undefined){
					_this.v8.speed(_this.region7);
				}
			}else if(_this.selectRow=="3"&&_this.selectCol=="3"){
				if(_this.v9 != undefined){
					_this.v9.speed(_this.region8);
				}
			}
		},
		//关闭
		CloseVideo(event){
			var timevalue=new Date();
			var timedata1=[];
			this.timersetInterval=setTimeout(function(){
				if(this.selectRow=="1"&&this.selectCol=="1"){
					if (this.v1 != undefined){
						this.region = "X1"
						this.v1.disconnect();
						delete this.v1;
						this.v1 = undefined;
						
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#rtcgaovideohb11").removeClass("rtc_new");
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="1"&&this.selectCol=="2"){
					if (this.v2 != undefined){
						this.region1 = "X1"
						this.v2.disconnect();
						delete this.v2;
						this.v2 = undefined;
						$("#rtcgaovideohb12").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="2"&&this.selectCol=="1"){
					if (this.v3 != undefined){
						this.region2 = "X1"
						this.v3.disconnect();
						delete this.v3;
						this.v3 = undefined;
						$("#rtcgaovideohb21").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="2"&&this.selectCol=="2"){
					if (this.v4 != undefined){
						this.region3 = "X1"
						this.v4.disconnect();
						delete this.v4;
						this.v4 = undefined;
						$("#rtcgaovideohb22").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="1"&&this.selectCol=="3"){
					if (this.v5 != undefined){
						this.region4 = "X1"
						this.v5.disconnect();
						delete this.v5;
						this.v5 = undefined;
						$("#rtcgaovideohb13").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="2"&&this.selectCol=="3"){
					if (this.v6 != undefined){
						this.region5 = "X1"
						this.v6.disconnect();
						delete this.v6;
						this.v6 = undefined;
						$("#rtcgaovideohb23").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="3"&&this.selectCol=="1"){
					if (this.v7 != undefined){
						this.region6 = "X1"
						this.v7.disconnect();
						delete this.v7;
						this.v7 = undefined;
						$("#rtcgaovideohb31").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="3"&&this.selectCol=="2"){
					if (this.v8 != undefined){
						this.region7 = "X1"
						this.v8.disconnect();
						delete this.v8;
						this.v8 = undefined;
						$("#rtcgaovideohb32").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}else if(this.selectRow=="3"&&this.selectCol=="3"){
					if (this.v9 != undefined){
						this.region8 = "X1"
						this.v9.disconnect();
						delete this.v9;
						this.v9 = undefined;
						$("#rtcgaovideohb33").removeClass("rtc_new");
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
						$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
					}
				}
			}.bind(this),100)
		},
		//timeline
		funtimeine(){
			// console.log(this.cols*this.rows);
			for(var i=1;i<=this.rows;i++){
				for(var l=1;l<=this.cols;l++){
					// console.log("#timeline"+i+l);
					$("#timeline"+i+l).TimeSlider({
						init_cells:this.timedata
					});
				}
			}
		},
		//un ui
		updateUI(){
			$(".content").innerHeight($('.content-wrapper').innerHeight() - $('.content-header').outerHeight() - $('.main-header').innerHeight());
			//$('div[name="flex"]').height(($(".content").height() - 50) / this.rows);
			if($(document.body).width() < 768)
			{
				this.contentHeight = $(document.body).height()*0.4;
			}else
			{
				this.contentHeight = $(document.body).height()*1;
			}
			// this.contentHeight = $('.videogh1').height();
			$('div[name="flex"]').height((this.contentHeight / this.rows)-130);
			$('.content-mythe-one').height(this.contentHeight / this.rows*2.4);
			//this.contentHeight = $(document.body).height()*0.8;
			let _this = this;
			if (H5siOS() === true)
			{
				$('.h5video').prop("controls", true);
			}

			$(".right-side-toggle").on("click", function () {
				$(".right-sidebar").slideDown(50).toggleClass("shw-rside");
				$(".fxhdr").on("click", function () {
					body.toggleClass("fix-header"); /* Fix Header JS */
				});
				$(".fxsdr").on("click", function () {
					body.toggleClass("fix-sidebar"); /* Fix Sidebar JS */
				});

				/* ===== Service Panel JS ===== */

				var fxhdr = $('.fxhdr');
				if (body.hasClass("fix-header")) {
					fxhdr.attr('checked', true);
				} else {
					fxhdr.attr('checked', false);
				}
			});
		},
		// esc退出全屏
		checkFull() {
			var isFull =document.fullscreenElement ||document.webkitFullscreenElement ||document.mozFullScreenElement ||document.msFullscreenElement
			
			if (isFull === undefined) {
				isFull = false
			}
			return isFull
		},

		panelFullScreen(event) {
			$("#videoPanel").removeClass("videogh1")
			var elem = document.getElementById('Fullscreen');
			// document.getElementById("fixed_input").style.display="none";
			if (
			document.fullscreenEnabled ||
			document.webkitFullscreenEnabled ||
			document.mozFullScreenEnabled ||
			document.msFullscreenEnabled
			) {
				if (
					document.fullscreenElement ||
					document.webkitFullscreenElement ||
					document.mozFullScreenElement ||
					document.msFullscreenElement
				) {
					if (document.exitFullscreen) {
						document.exitFullscreen();
					} else if (document.webkitExitFullscreen) {
						document.webkitExitFullscreen();
					} else if (document.mozCancelFullScreen) {
						document.mozCancelFullScreen();
					} else if (document.msExitFullscreen) {
						document.msExitFullscreen();
					}
					$(".layout").css("display",  "block");
					console.log("========  updateUIExitFullScreen");
					this.updateUIExitFullScreen();
				} else {
					console.log('panelFullScreen3');
					$(".layout").css("display",  "none");
					if (elem.requestFullscreen) {
						elem.requestFullscreen();
					} else if (elem.webkitRequestFullscreen) {
						elem.webkitRequestFullscreen();
					} else if (elem.mozRequestFullScreen) {
						elem.mozRequestFullScreen();
					} else if (elem.msRequestFullscreen) {
						elem.msRequestFullscreen();
					}
					this.updateUIEnterFullScreen();
					if (document.addEventListener)
					{
						document.addEventListener('webkitfullscreenchange', this.updateUIExitFullScreen, false);
						document.addEventListener('mozfullscreenchange', this.updateUIExitFullScreen, false);
						document.addEventListener('fullscreenchange', this.updateUIExitFullScreen, false);
						document.addEventListener('MSFullscreenChange', this.updateUIExitFullScreen, false);
					}
				}
			} else {
				console.log('Fullscreen is not supported on your browser.');
			}
		},
		updateUIEnterFullScreen(){
			console.log(screen.height);
			$('div[name="flex"]').height(screen.height / this.rows-100);
		},
		updateUIExitFullScreen(){
			if($(document.body).width() < 768)
			{
				this.contentHeight = $(document.body).height()*0.4;
			}else
			{
				this.contentHeight = $(document.body).height()*0.88;
				$('div[name="flex"]').height(this.contentHeight / this.rows);
			}
			console.log(this.contentHeight);
			if (!document.fullscreenElement && !document.webkitIsFullScreen && !document.mozFullScreen && !document.msFullscreenElement){
				this.contentHeight = $(document.body).height()*0.73;
				$('div[name="flex"]').height((this.contentHeight / this.rows)+130);
			}
		},
		//进度条加号
		jia(){
			$("#timeline"+this.selectRow+this.selectCol).TimeSlider('mousewhs',-1);
		},
		//进度条减号
		jian(){
			$("#timeline"+this.selectRow+this.selectCol).TimeSlider('mousewhs',1);
		},
		//点击宫格
		videoClick(r, c, $event) {
			this.rtcid="rtchvideo"+r+c;
			$("video").removeClass('h5videoh');
			$("#gaovideohb"+r+c).addClass('h5videoh');
			$('.tab').hide();//隐藏所有内容
			$("#tab"+r+c).show();//显示当前选中项
			this.selectCol = c;
			this.selectRow = r;
			console.log(r, c);
			// if ($($event.target).parent().hasClass('videoClickColor')) {
			// 	$($event.target).parent().removeClass('videoClickColor');
			// } else {
			// 	$('#videoPanel div[class*="videoClickColor"]').removeClass('videoClickColor');
			// 	$('#videoPanel>div').eq(r - 1).children('div').eq(c - 1).addClass('videoClickColor');
			// 	//$('#videoPanel>div').eq(r - 1).children('div').eq(c - 1).children(".h5videowrapper").children(".h5video").style.opacity = "0.25";
			// }
			if(this.selectRow=="1"&&this.selectCol=="1"){
				console.log(this.SGtoken[0]);
				this.Gtoken=this.SGtoken[0];
			}else if(this.selectRow=="1"&&this.selectCol=="2"){
				console.log(this.SGtoken[1]);
				this.Gtoken=this.SGtoken[1];
			}else if(this.selectRow=="2"&&this.selectCol=="1"){
				console.log(this.SGtoken[2]);
				this.Gtoken=this.SGtoken[2];
			}else if(this.selectRow=="2"&&this.selectCol=="2"){
				console.log(this.SGtoken[3]);
				this.Gtoken=this.SGtoken[3];
			}else if(this.selectRow=="1"&&this.selectCol=="3"){
				console.log(this.SGtoken[4]);
				this.Gtoken=this.SGtoken[4];
			}else if(this.selectRow=="2"&&this.selectCol=="3"){
				console.log(this.SGtoken[5]);
				this.Gtoken=this.SGtoken[5];
			}else if(this.selectRow=="3"&&this.selectCol=="1"){
				console.log(this.SGtoken[6]);
				this.Gtoken=this.SGtoken[6];
			}else if(this.selectRow=="3"&&this.selectCol=="2"){
				console.log(this.SGtoken[7]);
				this.Gtoken=this.SGtoken[7];
			}else if(this.selectRow=="3"&&this.selectCol=="3"){
				console.log(this.SGtoken[8]);
				this.Gtoken=this.SGtoken[8];
			}
		},
		//点击宫格
		changePanel(event) {
			// return false;
			window.setTimeout(function() {
				this.functlist();
			}.bind(this),50)
			let data = $(event.target).data('row');
			let _this = this;
			let cols = data.split('|')[1];
			let rows = data.split('|')[0];
			//this.map.clear();
			Object.assign(this.$data, {
				rows: parseInt(rows),
				cols: parseInt(cols)
			});
			// clearInterval(this.PlaybackCB);
			if (data == '1|1') {
				if (this.v2 != undefined)
				{
					this.v2.disconnect();
					delete this.v2;
					this.v2 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v3 != undefined)
				{
					this.v3.disconnect();
					delete this.v3;
					this.v3 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v4 != undefined)
				{
					this.v4.disconnect();
					delete this.v4;
					this.v4 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v5 != undefined)
				{
					this.v5.disconnect();
					delete this.v5;
					this.v5 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v6 != undefined)
				{
					this.v6.disconnect();
					delete this.v6;
					this.v6 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v7 != undefined)
				{
					this.v7.disconnect();
					delete this.v7;
					this.v7 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v8 != undefined)
				{
					this.v8.disconnect();
					delete this.v8;
					this.v8 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v9 != undefined)
				{
					this.v9.disconnect();
					delete this.v9;
					this.v9 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
			}else if (data =='2|2') {
				if (this.v5 != undefined)
				{
					this.v5.disconnect();
					delete this.v5;
					this.v5 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v6 != undefined)
				{
					this.v6.disconnect();
					delete this.v6;
					this.v6 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v7 != undefined)
				{
					this.v7.disconnect();
					delete this.v7;
					this.v7 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v8 != undefined)
				{
					this.v8.disconnect();
					delete this.v8;
					this.v8 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
				if (this.v9 != undefined)
				{
					this.v9.disconnect();
					delete this.v9;
					this.v9 = undefined;
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).load();
					$("#gaovideohb"+this.selectRow+this.selectCol).get(0).poster = '';
				}
			}
			Vue.nextTick(function () {
				// console.log(_this.contentHeight / rows);
				//$('div[name="flex"]').height(($(".content").height() - 50) / rows);
				$('div[name="flex"]').height(_this.contentHeight / rows);
				
				$("#timeline12").TimeSlider({init_cells:_this.timedata});
				$("#timeline21").TimeSlider({init_cells:_this.timedata});
				$("#timeline22").TimeSlider({init_cells:_this.timedata});
				$("#timeline13").TimeSlider({init_cells:_this.timedata});
				$("#timeline23").TimeSlider({init_cells:_this.timedata});
				$("#timeline31").TimeSlider({init_cells:_this.timedata});
				$("#timeline32").TimeSlider({init_cells:_this.timedata});
				$("#timeline33").TimeSlider({init_cells:_this.timedata});
			})
		},
		stopVideo(event){
			return;
		},
		Play(data,startTimes,endTimes){
			var token=data;
			//关闭视频并改变图标
			
			this.icon="iconfont icon-zantingtingzhi";
			this.icon1="iconfont icon-zantingtingzhi";
			this.icon2="iconfont icon-zantingtingzhi";
			this.icon3="iconfont icon-zantingtingzhi";
			//console.log(data.token);
			//放入视频
			if(token==undefined){
				return false;
			}
			// var Gtoken=data.token
			
			var timevalue=this.xzvalue;
			console.log("timevalue11111",timevalue);
			var year = timevalue.getFullYear();
			var month = timevalue.getMonth() + 1;
			var strDate = timevalue.getDate();
			var strDate1 = timevalue.getDate()-1;
			var localOffset = Math.abs(timevalue.getTimezoneOffset() /60);
			if(this.Adswitch=="false"){
				if (month<10) {
					month = '0'+month;
				}
				if (strDate<10) {
					strDate = '0'+strDate;
				}
				if (strDate1<10) {
					strDate1 = '0'+strDate1;
				}
			}
			var timevalues='';
			var timevalues1='';
			var timevaluee='';
			var judgeFn = new RegExp(/\s+/g);
			if (startTimes) {
				if (judgeFn.test(startTimes)) {
					timevalues=startTimes.split(" ")[0]+'+'+startTimes.split(" ")[1];
					timevalues1=startTimes.split(" ")[0]+'+'+startTimes.split(" ")[1];
				}else{
					timevalues=startTimes;
					timevalues1=startTimes;
				}
			}else{
				// timevalues=year+"-"+month+"-"+strDate+"T"+"00:00:00"+"+0"+localOffset+":00";
				// timevalues1=year+"-"+month+"-"+strDate1+"T"+"00:00:00"+"+0"+localOffset+":00";
			}
			if (endTimes) {
				if (judgeFn.test(endTimes)) {
					timevaluee=endTimes.split(" ")[0]+'+'+endTimes.split(" ")[1];
				}else{
					timevaluee=endTimes;
				}
			}else{
				timevaluee=year+"-"+month+"-"+strDate+"T"+"23:59:59"+"+0"+localOffset+":00";
			}
			console.log("======",strDate,strDate1);
			console.log("timevaluee222222",timevalues,timevaluee,"------",localOffset);
			// return false;
			var root = this.$store.state.IPPORT;
			var wsroot = this.$store.state.WSROOT;
			var url=""
			if(this.Adswitch=="false"){
				url = root + "/api/v1/SearchDeviceRecordByTime?token="+token
				+"&start="+encodeURIComponent(timevalues1)+"&end="+encodeURIComponent(timevaluee)+"&session="+ this.$store.state.token;
			}else{
				url = root + "/api/v1/Search?type=record&token="+token
				+"&start="+encodeURIComponent(timevalues1)+"&end="+encodeURIComponent(timevaluee)+"&session="+ this.$store.state.token;
			}
			
			//return false;
			this.$http.get(url).then(result=>{
				console.log(url,result);
				if(this.Adswitch=="false"){
					let tim0 = new Date(timevalues).getTime();
					console.log(tim0);
					if(result.status == 200){
						var data=result.data;
						var timedata1=[];
						//console.log("length",data.record.length);
						for(var i=0;i<data.record.length;i++){
							var item=data.record[i];
							var item1
							if (i < data.record.length-1) {
								item1=data.record[i+1];
							}
							//时间转换
							var starf=new Date(item['strStartTime']).getTime();
							var starf1=new Date(item1['strStartTime']).getTime();
							var end=new Date(item['strEndTime']).getTime();
							if (starf<tim0&&end>tim0) {
								this.starttimes = timevalues
								console.log(starf,end,tim0,this.starttimes);
							}
							if (end<tim0&&starf1>tim0) {
								this.starttimes = item1['strStartTime'];
								console.log('零点没有视频');
							}
							if (new Date(data.record[0].strStartTime).getTime() >= tim0) {
								this.starttimes = data.record[0].strStartTime;
								console.log('零点没有视频');
							}

							var starf=new Date(starf);
							var end=new Date(end);
							var timeitem={
									beginTime :starf,
									endTime :end,
									style:{background:"rgba(60,196,60, 0.498039)"}
								};
								console.log(item["nType"]);
							//console.log("录像段时间段颜色",timeitem["style"].background); //录像段时间段颜色
							if(item["nType"]==="H5_REC_MANUAL"){
								timeitem["style"].background="rgba(60,196,60, 0.498039)"
								//console.log("录像段时间段颜色1",timeitem["style"].background);
							}else if(item["nType"]==="H5_REC_SCHEDULE"){
								timeitem["style"].background="#31b1ff"
							}else{
								timeitem["style"].background="rgba(238,17,17, 0.498039)"
								//console.log("录像段时间段颜色2",timeitem["style"].background);
							}
							timedata1.push(timeitem);
							// console.log("4545454",timeitem);
						}
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
					}
				}else{
					if(result.status == 200){
					var data=result.data;
					var timedata1=[];
					//console.log("length",data.record.length);
					for(var i=0;i<data.record.length;i++){
						var item=data.record[i];
						//时间转换
						var starf=new Date(item['strStartTime']).getTime();
						var end=new Date(item['strEndTime']).getTime();
						var starf=new Date(starf);
						var end=new Date(end);
						var timeitem={
								beginTime :starf,
								endTime :end,
								style:{background:"rgba(60,196,60, 0.498039)"}
							};
							console.log(item["nType"],555555555555555);
						//console.log("录像段时间段颜色",timeitem["style"].background); //录像段时间段颜色
						if(item["nType"]==="H5_REC_MANUAL"){
							timeitem["style"].background="rgba(60,196,60, 0.498039)"
							//console.log("录像段时间段颜色1",timeitem["style"].background);
						}else if(item["nType"]==="H5_REC_SCHEDULE"){
							timeitem["style"].background="#31b1ff"
						}else{
							timeitem["style"].background="rgba(238,17,17, 0.498039)"
							//console.log("录像段时间段颜色2",timeitem["style"].background);
						}
						timedata1.push(timeitem);
						// console.log("4545454",timeitem)
						$("#timeline"+this.selectRow+this.selectCol).TimeSlider('init',timevalue,timedata1);
					}
				}
				}
				if(this.selectRow=="1"&&this.selectCol=="1"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v1 != undefined)
					{
						this.v1.disconnect();
						delete this.v1;
						this.v1 = undefined;
						console.log("上this.v1",this.v1);
					}
					console.log('111',timevalues, '222',this.starttimes);
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						// begintime: timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					console.log(pbconf1,444444444);
					let conf = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					console.log('====================================');
					console.log(conf);
					console.log('====================================');
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v1 = new H5sPlayerRTC(conf);
						$("#rtcgaovideohb11").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v1 = new H5sPlayerWS(conf);
						$("#rtcgaovideohb11").removeClass("rtc_new");
					}
					// this.v1 = new H5sPlayerRTC(conf);
					console.log("v111111111111",this.v1)
					//return false;
					this.v1.connect();
				}else if(this.selectRow=="1"&&this.selectCol=="2"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v2 != undefined)
					{
						this.v2.disconnect();
						delete this.v2;
						this.v2 = undefined;
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB1,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf2 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v2 = new H5sPlayerRTC(conf2);
						$("#rtcgaovideohb12").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v2 = new H5sPlayerWS(conf2);
					}
					console.log("v22222222222222222",this.v2)
					//return false;
					this.v2.connect();
				}else if(this.selectRow=="2"&&this.selectCol=="1"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v3 != undefined)
					{
						this.v3.disconnect();
						delete this.v3;
						this.v3 = undefined;
						console.log("上this.v1",this.v1);
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB2,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf3 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v3 = new H5sPlayerRTC(conf3);
						$("#rtcgaovideohb21").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v3 = new H5sPlayerWS(conf3);
					}
					//return false;
					this.v3.connect();
				}else if(this.selectRow=="2"&&this.selectCol=="2"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v4 != undefined)
					{
						this.v4.disconnect();
						delete this.v4;
						this.v4 = undefined;
						console.log("上this.v1",this.v1);
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB3,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf4 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v4 = new H5sPlayerRTC(conf4);
						$("#rtcgaovideohb22").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v4 = new H5sPlayerWS(conf4);
					}
					//return false;
					this.v4.connect();
				}else if(this.selectRow=="1"&&this.selectCol=="3"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v5 != undefined)
					{
						this.v5.disconnect();
						delete this.v5;
						this.v5 = undefined;
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB4,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf2 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v5 = new H5sPlayerRTC(conf2);
						$("#rtcgaovideohb13").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v5 = new H5sPlayerWS(conf2);
					}
					console.log("v22222222222222222",this.v5)
					//return false;
					this.v5.connect();
				}else if(this.selectRow=="2"&&this.selectCol=="3"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v6 != undefined)
					{
						this.v6.disconnect();
						delete this.v6;
						this.v6 = undefined;
						console.log("上this.v1",this.v6);
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB5,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf3 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v6 = new H5sPlayerRTC(conf3);
						$("#rtcgaovideohb23").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v6 = new H5sPlayerWS(conf3);
					}
					//return false;
					this.v6.connect();
				}else if(this.selectRow=="3"&&this.selectCol=="1"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v7 != undefined)
					{
						this.v7.disconnect();
						delete this.v7;
						this.v7 = undefined;
						console.log("上this.v1",this.v1);
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB6,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf4 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v7 = new H5sPlayerRTC(conf4);
						$("#rtcgaovideohb31").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v7= new H5sPlayerWS(conf4);
					}
					//return false;
					this.v4.connect();
				}else if(this.selectRow=="3"&&this.selectCol=="2"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v8 != undefined)
					{
						this.v8.disconnect();
						delete this.v8;
						this.v8 = undefined;
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB7,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf2 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v8 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v8 = new H5sPlayerRTC(conf2);
						$("#rtcgaovideohb32").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v8 = new H5sPlayerWS(conf2);
					}
					console.log("v82222222222222222",this.v8)
					//return false;
					this.v8.connect();
				}else if(this.selectRow=="3"&&this.selectCol=="3"){
					this.selectCol1 = this.selectCol;
					this.selectRow1 = this.selectCol;
					if (this.v9 != undefined)
					{
						this.v9.disconnect();
						delete this.v9;
						this.v9 = undefined;
						console.log("上this.v1",this.v1);
					}
					var pbconf1 = {
						begintime: this.starttimes||timevalues,
						endtime: timevaluee,
						autoplay: 'true',
						showposter:"false", //'true' or 'false' show poster
						callback: this.PlaybackCB8,
						serverpb: this.Adswitch, 
						userdata:  this // user data
					};
					let conf3 = {
						videoid: "gaovideohb"+this.selectRow+this.selectCol,
						protocol: window.location.protocol, //http: or https:
						host: wsroot, //localhost:8080
						rootpath:'/', // '/'
						token:token,
						pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
						hlsver:'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token
					};
					if (this.$store.state.liveviewrtc1 == 'RTC' || (H5siOS() === true)){
						console.log(this.$store.state.liveviewrtc1,555552)
						this.v9 = new H5sPlayerRTC(conf3);
						$("#rtcgaovideohb33").addClass("rtc_new");
					}else{
						console.log(this.$store.state.liveviewrtc1,555551)
						this.v9 = new H5sPlayerWS(conf3);
					}
					//return false;
					this.v9.connect();
				}
				this.starttimes = '';
			})
			// return false;
		},
		//有用啦
		PlaybackCB(event, userdata){	
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval)
					}else{
						$("#timeline11").TimeSlider('set_time_to_middle', time);
					}
				},50)
			}
		},
		PlaybackCB1(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval1)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval1 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval1)
					}else{
						$("#timeline12").TimeSlider('set_time_to_middle', time);
					}
				},50)
				// }
			}
		},
		PlaybackCB2(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval2)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval2 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval2)
					}else{
						$("#timeline21").TimeSlider('set_time_to_middle', time);
					}
				},50)
				// }
			}
		},
		PlaybackCB3(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval3)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval3 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval3)
					}else{
						$("#timeline22").TimeSlider('set_time_to_middle', time);
					}
				},50)
				// }
			}
		},
		PlaybackCB4(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval4)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval4 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval4)
					}else{
						$("#timeline13").TimeSlider('set_time_to_middle', time);
					}
				},50)
				// }
			}
		},
		PlaybackCB5(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval5)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval5 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval5)
					}else{
						$("#timeline23").TimeSlider('set_time_to_middle', time);
					}
				},50)
			}
		},
		PlaybackCB6(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval6)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval6 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval6)
					}else{
						$("#timeline31").TimeSlider('set_time_to_middle', time);
					}
				},50)
			}
		},
		PlaybackCB7(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval7)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval7 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval7)
					}else{
						$("#timeline32").TimeSlider('set_time_to_middle', time);
					}
				},50)
			}
		},
		PlaybackCB8(event, userdata){
			if (this.region =="X1")this.region = 1;
			var msgevent = JSON.parse(event);
			if (msgevent.type === 'H5S_EVENT_PB_TIME'){
				clearInterval(this.PlaybackCBinterval8)
				var time = new Date(msgevent.pbTime.strTime).getTime();
				var times = new Date(msgevent.pbTime.strTime).getTime()+1000;
				this.PlaybackCBinterval8 = setInterval(()=>{
					time=time+50*Number(this.region)
					if (time>=times) {
						clearInterval(this.PlaybackCBinterval8)
					}else{
						$("#timeline33").TimeSlider('set_time_to_middle', time);
					}
				},50)
			}
		},
		enter(){
			document.getElementsByClassName("h5controls").style.display="block";
		},
		leave(){
			document.getElementsByClassName("h5controls").style.display="none";
		},
		enter1(){
			$(".back_Choice1").css("display","block");
			// document.getElementById("back_Choice").style.display="block";
		},
		leave1(){
			$(".back_Choice1").css("display","none");
			// document.getElementById("back_Choice").style.display="none";
		},
		//隐藏按钮
		hideover(r,c){
			$("#h"+r +""+c).children(".h5controls").css("display",  "block");
		},
		showout(r,c){
			$("#h"+r +""+c).children(".h5controls").css("display",  "none");
		},
		//回访，模式
		hback(){
			this.Adswitch=true;
		},
		nvrback(){
			this.Adswitch=false;
		},
		//关闭
		handleClose(){
			if (this.v1 != undefined){
				console.log('关闭')
				this.myModal1=false
				this.timelink=0
				this.v1.disconnect();
				delete this.v1;
				this.v1 = undefined;
			}
		},
		//模糊查询
		filterNode(value, data) {
			if (!value) return true;
			console.log(!value,data);
			if (this.$store.state.root=="Operator") {
				return data.strName.indexOf(value) !== -1;
			}else{
				return data.label.indexOf(value) !== -1;
			}
		}
	},
	computed: {
		watchvideo(){
			return this.$route.query
		},
	},
	//模糊查询
	watch: {
		filterText(val) {
			this.$refs.tree.filter(val);
		},
		watchvideo(data){
			// this.viewlist();
			// if (data.session) {
				if (data.token&&data.starttime) {
					this.Play(data.token,data.starttime,data.endtime)
				}
			// }
		},
		PlayVideo(){
			
		}
	}
}
</script>
<style lang="scss" scoped>
.playback{
	// display: flex;
	// justify-content: space-between;
	width: 100vw;
	height: 90vh;
	.playback_snap_zuo{
		width: 18%;
		height: 87vh;
		.snap_zuo_title{
			width: 100%;
			padding: 12px 20px;
			font-size: 16px;
			font-family: PingFang SC;
			font-weight: 500;
			margin-bottom: 10px;
		}
		.StatusPromptColor{
			color:#67cfb5 !important;
		}
	}
	.playback_snap_you{
		width: 100% !important;
		height: 100% !important;
		.fuhao{
			margin-left: 2px;
			color: #fff;
			line-height: 14px;
			font-size: 12px;
			padding: 0 2px;
			border:none;
			border-radius: 0;
			background:#343434 ;
		}
		width: 82%;
		height: 87vh;
		// .videogh1{
			// height: 74vh;
			// padding-bottom: 3px;
		// }
		#videoPanel{
			.videoClickColor{
				/* border: 2px solid #f44336 !important;
				box-sizing: border-box; */
				box-shadow: 0 0 0 2px #f44336;
				box-sizing: border-box;
				z-index: 10;
			}
			.videoColor {
				background-color: #222222;
				.palace{
					background-size: 10%;
					flex: 1 1 10%;
					border:1px solid black;
					position: relative;
					.h5controls {
						position:absolute;
						top:0;
						background: url('~@/views/liveview/imgs/liveview_buttback.png') no-repeat;
						background-size: 90px 35px;
						background-position-x:right;
						padding:0px;
						box-sizing:content-box;
						z-index:10000;
						width: 100%;
						height: 32px;
						display:none;
						text-align: right;
						line-height: 30px;
						button{
							border: none;
							background: none;
							color: #FFFFFF;
							margin-right: 8px;
						}
						/deep/.open_liveplay_butt{
							display: none;
            }
            .rotate{
							-webkit-transform: rotateY(180deg);
							transform: rotateY(180deg); 
            }
					}
				}
			}
			div[name='flex'] {
				display: flex;
				flex-wrap: wrap;
				border-bottom: 0px !important;
				:hover {
					/*background-color: #3c8dbc;*/
					cursor: pointer;
				}
				+[name='flex'] {
					border-left: 0px !important;
				}
			}
		}
		.tab{
			.mode_Choice{
				.back_Choice{
					width: 235px;
					position: relative;
					cursor:pointer;
					.back_Choice1{
						border-radius:10px; 
						display: none;
						width: auto;
						position: absolute;
						top: -60px;
						left: -20px;
						padding: 10px 20px;
						div:nth-child(1){
							margin-bottom: 10px;
						}
						.co_Baise{
							color: #fff !important;
						}
					}
					.back_zi{
						line-height: 46px;font-size: 20px;margin-left:10px;
					}
				}
			}
			.function_butt{
				width: 30%;
				min-width: 330px;
				max-width: 400px;
				display: flex;
				justify-content: space-around;
				align-items: center;
				.fixed_input{
					width: 60%;
					max-width: 130px;
				}
				.button_resume{
					border: none;
					background: none;
				}
				.ele{
					width: 40px !important;
					height: 20px;
					background: #2c7bf4;
					border: none;
					border-radius: 8px;
				}
			}
			.caveat_butt{
				display: flex;align-items:center;min-width: 180px;
				.mr-1{
					width: 15px;
					height: 15px;
					border-radius: 50px;
					border: 0;
					margin: 0 5px;
					vertical-align:middle;
					background-color: rgb(60,196,60);
				}
				.mr-2{
					width: 15px;
					height:15px;
					border-radius: 50px;
					border: 0;
					margin: 0 5px;
					vertical-align:middle;
					background-color: #ee1011;
				}
				.mr-0{
					width: 15px;
					height:15px;
					border-radius: 50px;
					border: 0;
					margin-right: 5px !important;
					vertical-align:middle;
					background-color: #31b1ff;
				}
			}
		}
	}
}
.g_flex{
	display: flex;
}
/deep/ .selectdrop{
	border: none !important;
	background-color:#2D2D2D !important;
}
/deep/ .selectdrop .hover{
	background: #181818 !important;
}
/deep/ .el-input--suffix .el-input__suffix{
	display: none !important;
}
/deep/ .ele .el-input--suffix .el-input__inner{
	padding: 0 10px;
	border:none !important;
	height: 20px !important;
	line-height: 20px !important;
}
/deep/ .ele .selectdrop{
	left: -19px !important;
}
/deep/ .ele .popper__arrow{
	display: none;
}
</style>