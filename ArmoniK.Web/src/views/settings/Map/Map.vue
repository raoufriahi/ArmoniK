<template>
    <div class="Map">
        <div id="map" ref="rootmap" @drop="dropTarget($event)" @dragover.prevent="dragover($event)" >
			<div class="Map_core iconfont icon-target-lock" @click="Mapcore"></div>
			<div class="Map_functionbut">
				<div>
					<CButton @click="Mapbox" class="map_boxbut" type="submit">
						<i class="iconfont icon-box-select" style="margin-right: 4px;"></i>
						<p id="map_boxbut" style="display: initial;">Frame selection</p>
					</CButton>
					<CButton @click="Delcam(boxarr)" class="map_fullbut" type="submit">
						<i class="iconfont icon-ashbin"></i>
						Delete
					</CButton>
					<CButton @click="FullScreen" class="map_fullbut" type="submit">
						<i class="iconfont icon-fullscreen"></i>
						Full screen
					</CButton>
				</div>
				<div>
					<CButton @click="Mapview" class="map_viewbut" type="submit">
						<i class="iconfont icon-zhongmingming"></i>
						Modify the field of view
					</CButton>
				</div>
				<div>
					<CButton @click="Delcam(features,'all')" class="map_offbut" type="submit">
						<i class="iconfont icon-ashbin"></i>
						Clear configuration
					</CButton>
				</div>
			</div>
			<div class="Map_Region">
				<el-input
					prefix-icon="iconfont icon-sousuo"
					suffix-icon="iconfont icon-shoucang"
					class="liveview_left_input_search"
					placeholder="Please enter a location"
					@change="Mapsearch"
					@keyup.enter.native="Mapsearch"
					v-model="search">
				</el-input>
				<el-collapse v-model="activeNames">
					<el-collapse-item name="1" id="headswitch1">
						<template slot="title">
							<div style="display: flex;justify-content: space-between;width: 85%; align-items: center;">
								<div>Device</div>
							</div>
						</template>
						<el-input
							class="liveview_left_input"
							placeholder="Enter area keywords"
							v-model="filterText">
						</el-input>
						<el-tree
							class="el_tree"
							id = 'Maptree'
							node-key="strToken" 
							:default-expanded-keys="['root']" 
							:filter-node-method="filterNode"
							:data="data1"
							ref="tree" 
							@node-click="handleNodeClick"
							:props="defaultProps1">
								<span slot-scope="{ data }" style="width:100%;">
									<div style="width:100%;display: flex;justify-content: space-between;">
										<span  
											draggable="true" 
											@dragstart="dragStart($event,data.strToken,data.strName,data.streamprofile, data.name,data.disabled_me)">
											<span v-if="data.iconclass" style="font-size: 12px;" :class="data.iconclass" :id="'icon'+data.strToken"></span>
											<span v-else style="font-size: 12px;" class="iconfont icon-quyu" :id="'icon'+data.strToken"></span>
											<span :class="data.iconclass1" style="padding-left: 4px;">{{data.strName}}</span>
										</span>
									</div>
							  </span>
							<!-- <span slot-scope="{data }" style="width:100%;">
								<span>
									<span class="iconfont icon-quyu"></span>
									<span :class="data.iconclass1" style="padding-left: 4px;">{{data.strName}}</span>
								</span>
								<div v-if="data.cam.length!=0">
									<el-tree 
										class="el_tree1" 
										:data="data.cam" 
										:props="defaultProps1" 
										@node-click="handleNodeClick">
										<span slot-scope="{ data }" style="width:100%;">
											<div style="width:100%;display: flex;justify-content: space-between;">
												<span  
													draggable="true" 
													@dragstart="dragStart($event,data.strToken,data.strName,data.streamprofile, data.name,data.disabled_me)">
													<span style="font-size: 12px;" :class="data.iconclass" :id="'icon'+data.strToken"></span>
													<span :class="data.iconclass1" style="padding-left: 4px;">{{data.strName}}</span>
												</span>
											</div>
										</span>
									</el-tree>
								</div>
							</span> -->
						</el-tree>
					</el-collapse-item>
				</el-collapse>
			</div>
			<div id="popup" class="ol-popup">
				<a href="#" id="popup-closer" class="ol-popup-closer"></a>
				<div id="popup-content" class="popup-content">
					<div class="Map_Popup_title">
						<video class="h5video" id="Mapvideoid" autoplay webkit-playsinline playsinline></video>
					</div>
					<div class="Map_Popup_content">
						<div class="Map_content_name">
							<i class="iconfont icon-jiankong"></i>
							<p id="Map_content_token" ></p>
						</div>
						<div @click='Mapplay' class="Map_content_play iconfont icon-bofang"></div>
					</div>
				</div>
			</div>
			<el-dialog
				:before-close="handleClose"
				class="dasboard_modal"
				:title="videoname"
				:visible.sync="myModal"
				width="50%">
				<div class="text-center" style="position: relative;">
					<video class="videoo" id="Mapplayvideoid"></video>
					<!-- <div class="playback_function">
						<div></div>
						<div class="playback_function_icon">
							<div class="iconfont icon-full"></div>
						</div>
					</div> -->
					<!-- 码率 -->
					<div class="information"  style="display:none padding:0px">
						<div class="information_con">
							<div class="information1">
								<div class="information_title">{{$t('message.live.Video')}}</div>
								<div class="information_content" v-for="(a,index) in informationVideo" :key="index">
									<div class="information_content_left">{{a.name}}</div>
									<div class="information_content_right">{{a.data}}</div>
								</div>
							</div>
							<div class="information1">
								<div class="information_title">{{$t('message.live.Audio')}}</div>
								<div class="information_content" v-for="(a,index) in informationAudio" :key="index">
									<div class="information_content_left">{{a.name}}</div>
									<div class="information_content_right">{{a.data}}</div>
								</div>
							</div>
						</div>
					</div>
					<!-- 主副码流 -->
					<div id="videonameid" class="elqualitytoggle videoname" 
						@dragover.prevent="dragover($event)"
						draggable="true" 
						@dragstart="drag($event)">
						{{videoname}}--{{picturequality}}
						<input type="button" :value="valuebutton"  @click="Bitstream($event.target.value)" id="inputid" class="spanpicturequality"/>
					</div>
					<!-- 按钮 -->
					<div class="liveplay_butt">
						<div style="display: flex;justify-content: flex-end;">
							<button class="open_liveplay_butt" type="text" v-if="$store.state.liveviewrtc == 'RTC'">RTC</button>
							<button class="open_liveplay_butt" type="text" v-else>WS</button>
							<!-- <CDropdown
								class="fw_butt"
								style=""
								color="link"
								placement="bottom-start"
								:caret="false">
								<template #toggler-content>
									<button class="iconfont icon-erweima1" @click="qrcode" style="margin-left:0px"></button>
								</template>
								<div class="bottom_QR">
									<div class="bottom_scan">扫一扫</div>
									<div class="bottom_QRcode">
										<div>
											<div ref="qrcodead" id="qrcodead1" style="margin-bottom: 16px;"></div>
											<div>Android</div>
										</div>
										<div>
											<div ref="qrcodeios" id="qrcodeios1" style="margin-bottom: 16px;"></div>
											<div>iOS</div>
										</div>
									</div>
								</div>
							</CDropdown> -->
							<button class="info_id_show iconfont" :class="(Audioslider==0)?'icon-jingyin':'icon-mn_shengyin_fill'" @click="Audiosetup"></button>
							<button class="info_id_show iconfont icon-liuliang" @click="Information"></button>
							<button class="iconfont" :class="Shoutwheatclass" @click="Shoutwheat"></button>
							<button class="iconfont icon-picfill" @click="DoSnapshotWeb"></button>
							<button class="iconfont icon-camerafill" @click="DoSnapshot"></button>
							<button class="iconfont icon-videofill" @click="DoManualRecordStart"></button>
							<button class="iconfont icon-radioboxfill" @click="DoManualRecordStop"></button>
							<button class="iconfont" :class="Fullscren" @click="FullScreenVideo"></button>
						</div>
					</div>
					<div id="Audio_slider" class="Audio_slider">
						<span :class="(Audioslider==0)?'Audio_slider_jingyin':'Audio_slider_shengyin_fill'">
							<i class="iconfont icon-mn_shengyin_fill"></i>
						</span>
						<el-slider :step='0.1' :show-tooltip="false" :max='1' @input='Audiochange' v-model="Audioslider" style="width:80%;margin-left:10px"></el-slider>
					</div>
				</div>
			</el-dialog>
		</div>
		
    </div>
</template>

<script>
import '../../../assets/js/adapter'
import {H5sPlayerWS,H5sPlayerHls,H5sPlayerRTC,H5sPlayerAudBack} from '../../../assets/js/h5splayer.js'
import {H5siOS,H5sPlayerCreate} from '../../../assets/js/h5splayerhelper.js'
import "ol/ol.css";

import OSM from "ol/source/OSM";

import TileLayer from 'ol/layer/Tile'
import VectorLayer from 'ol/layer/Vector'
import VectorSource from 'ol/source/Vector'
import XYZ from 'ol/source/XYZ'
import { Map, View, Feature } from 'ol'
import { Style, Icon } from 'ol/style'
import Text from 'ol/style/Text'
import TileImage from "ol/source/TileImage";
import TileGrid from "ol/tilegrid/TileGrid";
import Fill from 'ol/style/Fill'
import { Point } from 'ol/geom'

import { OverviewMap, ScaleLine, MousePosition, defaults } from "ol/control";//比例尺
import {createStringXY} from 'ol/coordinate';//比例尺单位

import Overlay from "ol/Overlay";//弹窗
import { fromLonLat, transform, toLonLat ,get} from "ol/proj";//弹窗
import Extent from 'ol/interaction/Extent';

import Draw from 'ol/interaction/Draw';//框选
import {createBox} from 'ol/interaction/Draw';
import Stroke from 'ol/style/Stroke';
import CircleStyle from 'ol/style/Circle';
// import {defaults} from 'ol/control/util.js'

export default {
	data() {
		return {
			valuebutton:'辅码流',
			picturequality:'主码流',
			Fullscren:'icon-full',
			Audioslider:0,//音频
			search:"",//搜索
			Shoutwheatclass:"icon-voicefill1",//语音图标
			boxbut:true,//框选
			boxclick:true,//点击框选
			videoname:null,//视频名称
			setcore:null,//设置中心点
			setzoom:null,//设置中心点
			data1:[],
			Maptoken:undefined,
			activeNames:[],//设备
			filterText:'',
			myModal:false,//弹窗
			map: null,
			x: 0,
			y: 0,
			boxarr:[],//删除数组
			coordinates: [],
			camsrc:[],
			defaultProps1: {
				children: 'cam',
				label: 'strName',
				cam:"cam"
			},
			features: [],
			flagLayer: null,
			h5handler:undefined,
			timerRunInfo:null,
			informationshow: false,
			informationAudio:[],
            informationVideo:[],
			tokenshou:"",
			streamprofileInformation:"",
			tableData: this.EVENT.mapEvent,
			orgarr:{},
			Mapintel:null,
			treeClickCount:null,
			timer:null,
		};
	},
	//模糊查询
    watch: {
      filterText(val) {
        console.log("filter",val);
		this.$refs.tree.filter(val);
	  }
	},
	beforeDestroy() {
        this.handleClose();
		clearInterval(this.Mapintel)
	},
	created(){
		this.getcam();
		this.Regional();
	},
	mounted() {
		this.Mapintel = setInterval(() => {
			// console.log(this.tableData);
			let arr = [];
			this.camsrc.map((item)=>{
				arr.push(item.strToken)
			})
			this.tableData.map((res)=>{
				let object = {strToken:res.Token,strName:res.Token}
				if (arr.indexOf(res.Token)=='-1') {
					this.camsrc.push(object);
				}
				this.features.forEach((item,i)=>{
					if (item.id_==res.Token) {
						this.flagLayer.getSource().removeFeature(this.flagLayer.getSource().getFeatureById(item.id_))
						this.features.splice(i,1)
					}
				})
			})
			this.mapEventFeature(this.tableData)
		}, 1000);
		window.addEventListener('resize', () => {
            if(!this.checkFull()){
                // 退出全屏后要执行的动作
                this.Fullscren='icon-full';
                var $container =$('#Mapplayvideoid');
                var liveplay_butt = $container.children(".liveplay_butt");
                var suoxiao = liveplay_butt.children(".icon-suoxiao");
                suoxiao.removeClass("icon-suoxiao");
                suoxiao.addClass("icon-full");
            }
        });
		// this.Mapsearch()
		setTimeout(()=>{
			this.initPointMap();
		},200)
	},
	methods:{
		mapEventFeature (coordinates) {
			// return
			const _that = this
			// 设置图层
			_that.flagLayer = new VectorLayer({
				source: new VectorSource()
			})
			// 添加图层
			_that.map.addLayer(_that.flagLayer)
			// 循环添加feature
			for (let i = 0; i <coordinates.length; i++) {
				// 创建feature
				let feature = new Feature({
					geometry: new Point([coordinates[i].Detail.mobilePosition.Longitude,coordinates[i].Detail.mobilePosition.Latitude]),
				})
				// 设置ID
				feature.setId(coordinates[i].Detail.strDevToken)
				// 设置样式
				feature.setStyle(_that.getStyls(feature))
				// 放入features
				_that.features.push(feature)
			} // for 结束
			// 批量添加fe	ature
			_that.flagLayer.getSource().addFeatures(_that.features)
		},
		//码流按钮
        Bitstream(event){
            // return false
            if(event===this.$t("message.live.substream")){
                var streamprofile='sub'
                var token=this.tokenshou
                this.valuebutton=this.$t("message.live.mainstream")
                this.picturequality='Secondary stream';
                var root = this.$store.state.IPPORT;
                var url = root + "/api/v1/GetVidStreamStatus?token="+token+"&stream="+streamprofile+"&session="+ this.$store.state.token;
				console.log(url);
                //重组
                this.$http.get(url).then(result=>{
                    if(result.status == 200){
                        this.Mapplay(streamprofile)
                    }
                })
            }else if(event===this.$t("message.live.mainstream")){
                var streamprofile='main'
                var token=this.tokenshou
				this.valuebutton=this.$t("message.live.substream")
                this.picturequality='Main stream';
                var root = this.$store.state.IPPORT;
                var url = root + "/api/v1/GetVidStreamStatus?token="+token+"&stream="+streamprofile+"&session="+ this.$store.state.token;
				console.log(url);
                //重组
                this.$http.get(url).then(result=>{
                    if(result.status == 200){
                        this.Mapplay(streamprofile)
                    }
                })
            }
        },
		// 判断全屏
		checkFull(){
            //判断浏览器是否处于全屏状态 （需要考虑兼容问题）
            //火狐浏览器
            var isFull = document.mozFullScreen||
            document.fullScreen ||
            //谷歌浏览器及Webkit内核浏览器
            document.webkitIsFullScreen ||
            document.webkitRequestFullScreen ||
            document.mozRequestFullScreen ||
            document.msFullscreenEnabled
            if(isFull === undefined) {
            isFull = false
            }
            return isFull;
        },
		//开始录像
        DoManualRecordStart(){
            if (this.h5handler == undefined)
            {
                return;
            }
            let _this =this;

            var record = "token=" + this.tokenshou + "&duration=300";
            console.log("record cmd", record);

            var url = this.$store.state.IPPORT + "/api/v1/ManualRecordStart?" + record  + "&session="+ this.$store.state.token;

            this.$http.get(url).then(result => {
                console.log(result);
                if (result.status == 200)
                {
                    this.$message({
                        message: 'Manual Start Record successfully',
                        type: 'success'
                    });
                    var $container = $("#"+this.h5id);
                    var $controlrec = $container.children(".breath_light");
                    $controlrec.css("display", "block");
                    for (let i = 0; i < this.$store.state.Recviewadd.length; i++) {
                        if (this.$store.state.Recviewadd[i]==this.tokenshou) {
                            this.$store.state.Recviewadd.splice(i,1);
                            i--;
                        }
                    }
                    this.$root.bus.$emit('RootrecToken',this.tokenshou);
                    this.$root.bus.$emit('recToken',this.tokenshou);
                    this.$store.state.Recviewadd.push(this.tokenshou);
                    localStorage.setItem('bRec',JSON.stringify(this.$store.state.Recviewadd));
                }
            }).catch(error => {
                console.log('Record failed!', error);
                this.$message({
                    message: 'Record failed !',
                    type: 'warning'
                });
            });
        },
        //关闭录像
        DoManualRecordStop(event){
            if (this.h5handler == undefined)
            {
                return;
            }
            let _this =this;
            // var record = "token=" + this.currtoken + "&duration=300";
            var record = "token=" + this.tokenshou;
            console.log("record cmd", record);

            var url = this.$store.state.IPPORT + "/api/v1/ManualRecordStop?" + record  + "&session="+ this.$store.state.token;

            this.$http.get(url).then(result => {
                console.log(result);
                if (result.status == 200)
                {
                    this.$message({
                        message: 'Manual Stop Record successfully',
                        type: 'success'
                    });
                    var $container = $("#"+this.h5id);
                    var $controlrec = $container.children(".breath_light");
                    $controlrec.css("display", "none");

                    for(var i=0;i<this.$store.state.Recviewadd.length;i++){
                        if(this.$store.state.Recviewadd[i]==this.tokenshou){
                            this.$store.state.Recviewadd.splice(i,1)
                            i--;
                            console.log(this.$store.state.Recviewadd)
                        }
                    }
                    this.$root.bus.$emit('RootcloserecToken',this.tokenshou);
                    this.$root.bus.$emit('closerecToken',this.tokenshou);
                    localStorage.setItem('bRec',JSON.stringify(this.$store.state.Recviewadd));
                }
            }).catch(error => {
                this.$message({
                    message: 'Record failed !',
                    type: 'warning'
                });
            });
        },
		DoSnapshot(){
			if (this.h5handler == undefined)
            {
                return;
            }
            let _this =this;
            var snapshot = "token=" + this.tokenshou;
            var url = this.$store.state.IPPORT + "/api/v1/Snapshot?" + snapshot  + "&session="+ this.$store.state.token;
            this.$http.get(url).then(result => {
                console.log(result);
                if (result.status == 200)
                {
                    this.$message({
                        message: 'Snapshot successfully',
                        type: 'success'
                    });
                }
            }).catch(error => {
                console.log('Snapshot failed!', error);
                this.$Notice.info({
                    title: "Snapshot failed !"
                })
                this.$message({
                    message: 'Snapshot failed !',
                    type: 'warning'
                });
            });
		},
		DoSnapshotWeb(){
			if (this.h5handler == undefined)
            {
                return;
            }
            var fileName = '1';
            const date = new Date();
            fileName = this.tokenshou + '_' + date.getFullYear() + '-' + (date.getMonth() + 1)
                         + '-' + date.getDate() + '-' + date.getHours() + '-' + date.getMinutes() + '-' + date.getSeconds();
            console.log(fileName);
            var video = $("#Mapplayvideoid").get(0);
            var w = video.videoWidth;//video.videoWidth * scaleFactor;
            var h = video.videoHeight;//video.videoHeight * scaleFactor;
            var canvas = document.createElement('canvas');
            canvas.width = w;
            canvas.height = h;
            var ctx = canvas.getContext('2d');
            ctx.drawImage(video, 0, 0, w, h);
            var MIME_TYPE = "image/png";
            var imgURL = canvas.toDataURL(MIME_TYPE,1.0);
            // console.log(imgURL);

            var dlLink = document.createElement('a');
            dlLink.download = fileName;
            dlLink.href = imgURL;
            dlLink.dataset.downloadurl = [MIME_TYPE, dlLink.download, dlLink.href].join(':');

            document.body.appendChild(dlLink);
            dlLink.click();
            document.body.removeChild(dlLink);
		},
		Shoutwheat(){
			if (this.h5handler == undefined)
            {
                return;
            }
            var tokenshou=this.tokenshou
            var conf2 = {
                protocol: window.location.protocol, //http: or https:
                host: window.location.host, //localhost:8080
                rootpath:'/', // '/' or window.location.pathname
                token:tokenshou,
                session:this.$store.state.token //session got from login
            };
            
            
            var Shoutwheat=this.Shoutwheatclass;
            if(Shoutwheat=="icon-voicefill1"){
                console.log("大开");
                this.audioback = new H5sPlayerAudBack(conf2);
                this.audioback.connect();
                this.Shoutwheatclass="icon-voicefill";
            }else{
                console.log("关闭2");
                this.audioback.disconnect();
                delete this.audioback;
                this.audioback = undefined;
                this.Shoutwheatclass="icon-voicefill1";
            }
		},
		Information(){
            // var $container = $("#Mapplayvideoid");
            // var $controlsin = $container.children(".information");
            if (this.informationshow == false)
            {
                $(".information").css("display", "block");
                this.informationshow = true;
            }else
            {
                $(".information").css("display", "none");
                this.informationshow = false;
                clearInterval(this.timerRunInfo);
                return false
            }
            this.Informationdate()
            this.timerRunInfo = setInterval(() => {
                this.Informationdate()
            },8000)
		},
		Audiosetup(){
            if (this.audioshow == false){
                // $controlsin.css("display", "block");
                $("#Audio_slider").show();
                this.audioshow = true;
            }else{
                // $controlsin.css("display", "none");
                $("#Audio_slider").hide();
                this.audioshow = false;
            }
		},
		Audiochange(){
            //要打开音频设置
            console.log("要打开音频设置",this.Audioslider)
            document.getElementById('Mapplayvideoid').volume=this.Audioslider
        },
		FullScreenVideo(){
            var elem = $(".text-center").get(0);
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
                    this.Fullscren='icon-full';
                } else {
                    if (elem.requestFullscreen) {
                        elem.requestFullscreen();
                    } else if (elem.webkitRequestFullscreen) {
                        elem.webkitRequestFullscreen();
                    } else if (elem.mozRequestFullScreen) {
                        elem.mozRequestFullScreen();
                    } else if (elem.msRequestFullscreen) {
                        elem.msRequestFullscreen();
                    }
                    this.Fullscren='icon-suoxiao';
                }
            } else {
                console.log('Fullscreen is not supported on your browser.');
            }
		},
		Informationdate(){
            var root = this.$store.state.IPPORT;
			let picturequality = this.picturequality=="主码流"?"main":"sub";
            //url
            var url = root + "/api/v1/GetVidStreamStatus?token="+this.tokenshou+"&stream="+picturequality+"&session="+ this.$store.state.token;
            //重组
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    console.log(result)
                    var item=result.data
                    var informationAudio=[{
                            name:this.$t("message.live.Codec"),
                            data:item.strAudioType
                        },{
                            name:this.$t("message.live.SampleRate"),
                            data:item.nAudioSampleRate
                        },{
                            name:this.$t("message.live.SampleBit"),
                            data:item.nAudioSampleBit
                        },{
                            name:this.$t("message.live.Channels"),
                            data:item.nAudioChannels
                        },{
                            name:this.$t("message.live.Bitrate"),
                            data:(item.nAudioBitrate/1024).toFixed(1)+'kpbs'
                        }]
                         if(this.height){
                            var informationVideo = [
                                {
                                    name: this.$t("message.live.Codec"),
                                    data: item.strVideoType,
                                },
                                {
                                    name: this.$t("message.live.Width"),
                                    data: this.width,
                                },
                                {
                                    name: this.$t("message.live.Height"),
                                    data: this.height,
                                },
                                {
                                    name: this.$t("message.live.FPS"),
                                    data: item.nVideoFPS,
                                },
                                {
                                    name: this.$t("message.live.Bitrate"),
                                    data: (item.nVideoBitrate / 1024).toFixed(1) + "kpbs",
                                },
                                ];
                            }else{
                        var informationVideo=[{
                            name:this.$t("message.live.Codec"),
                            data:item.strVideoType
                        },{
                            name:this.$t("message.live.Width"),
                            data:item.nVideoWidth
                        },{
                            name:this.$t("message.live.Height"),
                            data:item.nVideoHeight
                        },{
                            name:this.$t("message.live.FPS"),
                            data:item.nVideoFPS
                        },{
                            name:this.$t("message.live.Bitrate"),
                            data:(item.nVideoBitrate/1024).toFixed(1)+'kpbs'
                        }]
                    }
                        this.informationAudio=informationAudio
                        this.informationVideo=informationVideo
                }
            })
        },
		//搜索
		Mapsearch(){
			var _this=this
			let view = _this.map.getView();
			var placeSearch = new AMap.PlaceSearch();  //构造地点查询类
			placeSearch.search(this.search, function(status, result) {
				if (status === 'complete' && result.info === 'OK') {
					var poiArr = result.poiList.pois;
					console.log(poiArr[0])
					view.setCenter([poiArr[0].location.M,poiArr[0].location.O]);
				}
			})
		},
		//回到中心点
		Mapcore(){
			var _this=this
			let view = _this.map.getView();
			view.setZoom(_this.setzoom);
			view.setCenter(_this.setcore);
		},
		//删除
		async Delcam(boxarr,all){
			console.log(boxarr)
			var flagLayer=this.flagLayer
			var _this=this
			var root = this.$store.state.IPPORT;
			for(var i=0; i<boxarr.length; i++){
				console.log(boxarr[i].id_)
				var url = root + "/api/v1/MapDelCam?token="+boxarr[i].id_+"&session="+ this.$store.state.token;
				await this.$http.get(url).then(result=>{
					if(result.status == 200){
						console.log('GetMapCamList',result)
						
						if(all=='all'){
							flagLayer.getSource().removeFeature(flagLayer.getSource().getFeatureById(boxarr[i].id_))
							_this.features=[]
						}else{
							for(var l=0; l<_this.features.length; l++){
								console.log(_this.features[l].id_)
								if(_this.features[l].id_==boxarr[i].id_){
									flagLayer.getSource().removeFeature(flagLayer.getSource().getFeatureById(boxarr[i].id_))
									_this.features.splice(l,1)
									console.log(_this.features)
								}
							}
						}
					}
				}).catch(error => {
					console.log('删除摄像机', error);
				});
			}
			
		},
		//点击框选
		Mapbox(){
			var _this=this
			var style = new Style({
				fill: new Fill({
					color: 'RGBA(58,187,254, 0.4)'
				}),
				//划线的时候的图样
				stroke: new Stroke({
					color: 'rgba(255,255,255, 0.1)',
					width: 2
				}),
				image: new CircleStyle({
					radius: 5,
					stroke: new Stroke({
						color: 'rgba(96,96,96, 0.1)'
					}),
					fill: new Fill({
						color: 'rgba(96,96,96, 0.1)'
					})
				})
			});
			var layer = new VectorLayer({
				source: new VectorSource(),
				style: style
			});
			var draw = new Draw({
				source: layer.getSource(),
				type: 'Circle',
				style: style,
				geometryFunction: createBox()
			});
			var btnclick=  document.getElementById('map_boxbut')
			
			btnclick.innerText = 'Cancel'
			_this.map.addInteraction(draw);
			draw.on('drawend',function (evt) {
				var extent = evt.feature.getGeometry().getExtent()
				//获取屏幕内的Feature _this.map.getView().calculateExtent()
				var boxarr= _this.map.getLayers().getArray()[1].getSource().getFeaturesInExtent(extent)//获取指定位置的Feature
				_this.boxarr=boxarr
				console.log(boxarr)
				_this.map.addLayer(layer);
				_this.map.removeInteraction(draw);
				// console.log(layer)
				// layer.getSource().clear();
				_this.map.removeLayer(layer);
	            btnclick.innerText = "Frame selection"
			});
		},
		//获取摄像机
		getcam(){
			var root = this.$store.state.IPPORT;
            var url = root + "/api/v1/GetMapCamList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
				if(result.status == 200){
					console.log('GetMapCamList',result)
					this.coordinates=result.data.cam
					this.camsrc=result.data.src
				}
			}).catch(error => {
                console.log('获取摄像机', error);
            });
		},
		//设置视野
		Mapview(){
			var zoom = this.map.getView().getZoom();  //获取当前地图的缩放级别getCenter
			var center = this.map.getView().getCenter()
			console.log(zoom,center)
			var root = this.$store.state.IPPORT;
            var url = root + "/api/v1/SetMapConf?zoom="+zoom+"&longitude="+center[0]+"&latitude="+center[1]+"&session="+ this.$store.state.token;
			console.log(url);
            this.$http.get(url).then(result=>{
				if(result.status == 200){
					console.log(result)
					if(result.data.bStatus){
						this.$message({
                            message: result.data.strCode,
                            type: 'success'
                        });
					}else{
						this.$message({
                            message: result.data.strCode,
                            type: 'warning'
                        });
					}
				}
            }).catch(error => {
                console.log('设置视野', error);
            });
		},
		//点击设备
		handleNodeClick(data, checked, indeterminate){
			//记录点击次数
			this.treeClickCount++;
			//单次点击次数超过2次不作处理,直接返回,也可以拓展成多击事件
			if (this.treeClickCount >= 2) {
				return;
			}
			let this_ = this
			//计时器,计算300毫秒为单位,可自行修改
			this.timer = window.setTimeout(() => {
				if (this.treeClickCount == 1) {
					//把次数归零
					this.treeClickCount = 0;
					//单击事件处理
					console.log(111);
					this.Maptoken=data.strToken;
					
				} else if (this.treeClickCount > 1) {
					//把次数归零
					this.treeClickCount = 0;
					//双击事件
					console.log(222);
					this.features.map((item)=>{
						if (item.id_ == data.strToken) {
							var center = item.geometryChangeKey_.target.extent_;
							this.map.getView().setCenter(center)
							this.map.getView().setZoom(16.2199293484723)
						}
					})
				}
			}, 300);
        },
		//拖动设备
        dragStart(ev,token,label,streamprofile,name,disabled_me){
            // console.log(ev,token,label,streamprofile,name,disabled_me,"124");
            var drag={
                token:token,
                label:label,
                streamprofile:streamprofile,
                name:name,
                disabled_me:disabled_me,
            }
            ev.dataTransfer.setData("Text",ev.target.id);
		},
		dragover (ev) {
            // console.log(ev,"123",ev.target.id)
            // ev.preventDefalut()
        },
        dropTarget (ev) {
			var _this=this
			_this.map.on('singleclick', _this.changeXY)
            // console.log(ev)
        },
		//地图弹窗播放
		Mapplay(stream){
			var videoid=$( "#Map_content_token" ).data( "token" );
			this.tokenshou = videoid;
			this.myModal=true
			let streamprofile = stream||"main"
			this.$nextTick(()=>{
				if (this.h5handler != undefined)
					{
						this.h5handler.disconnect();
						delete this.h5handler;
						this.h5handler = undefined;
					}
					let conf = {
						videoid:'Mapplayvideoid',
						protocol: window.location.protocol, //http: or https:
						host: this.$store.state.WSROOT, //localhost:8080
						streamprofile: streamprofile, // {string} - stream profile, main/sub or other predefine transcoding profile
						rootpath: '/', // '/'
						token: videoid,
						hlsver: 'v1', //v1 is for ts, v2 is for fmp4
						session: this.$store.state.token //session got from login
					};
					console.log(conf);
					if (this.$store.state.liveviewrtc == 'RTC' || (H5siOS() === true)){
						this.h5handler = new H5sPlayerRTC(conf);
					}else{
						this.h5handler = new H5sPlayerWS(conf);
					}
					this.h5handler.connect();
			})
		},
		//关闭
        handleClose(){
			if (this.h5handler != undefined)
			{
				this.myModal=false
				this.h5handler.disconnect();
				delete this.h5handler;
				this.h5handler = undefined;
			}
        },
		//创建地图
		initPointMap() {
			
			var Mapurl=JSON.parse(this.$store.state.Mapurl);
			console.log(Mapurl)
			var _this=this
			var tileLayer=''
			var overviewMap=''
			if(!Mapurl){
				tileLayer=[new TileLayer({
					source: new XYZ({
						title:'Gothe',
						url:'http://wprd0{1-4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&style=7&x={x}&y={y}&z={z}'
					})
				})]
				overviewMap=[new TileLayer({
					source: new XYZ({
						title:'Gothe',
						url:'http://wprd0{1-4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&style=7&x={x}&y={y}&z={z}'
					})
				})]
			}else{
				if(Mapurl[0].name=="Gothe"){
					tileLayer=[new TileLayer({
						source: new XYZ({
							title:Mapurl[0].name,
							url:Mapurl[0].url
						})
					})]
					overviewMap=[new TileLayer({
						source: new XYZ({
							title:Mapurl[0].name,
							url:Mapurl[0].url
						})
					})]
				}else if(Mapurl[0].name=="Google"){
					tileLayer=[new TileLayer({
						source: new XYZ({
							title:Mapurl[0].name,
							url:Mapurl[0].url,
							wrapX:false
						})
					})]
					overviewMap=[new TileLayer({
						source: new XYZ({
							title:Mapurl[0].name,
							url:Mapurl[0].url,
							wrapX:false
						})
					})]
				}else{
					tileLayer=[new TileLayer({
						source: new XYZ({
							title:'高德',
							url:'http://wprd0{1-4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&style=7&x={x}&y={y}&z={z}'
						})
					})]
					overviewMap=[new TileLayer({
					source: new XYZ({
						title:'高德',
						url:'http://wprd0{1-4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&style=7&x={x}&y={y}&z={z}'
					})
				})]
				}
			}
			var root = this.$store.state.IPPORT;
            var url = root + "/api/v1/GetMapConf?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
				if(result.status == 200){
					console.log(result)
					this.setcore=[result.data.center.strLongitude, result.data.center.strLatitude];
					this.setzoom=result.data.strZoom;
					this.map = new Map({
						target: "map",
						layers: tileLayer,
						controls: defaults().extend([
							// 添加一个鹰眼控件
							new OverviewMap({
								// 实例化一个OverviewMap类的对象，并加入到地图中
								collapsed: false,
								view: new View({
									projection: "EPSG:4326",
									center: [result.data.center.strLongitude, result.data.center.strLatitude],
								}),
								layers: overviewMap
							}),
							// 添加比例尺
							new ScaleLine({
								units: "metric"
							}),
							new MousePosition({
								coordinateFormat: createStringXY(6), // 保留6位小数位
							})
						]),
						
						view: new View({
							projection: "EPSG:4326",    //坐标系
							center: [result.data.center.strLongitude, result.data.center.strLatitude],  //坐标
							zoom: result.data.strZoom,
							// 限制缩放级别，可以和extent同用限制范围
							maxZoom: 19,
							// 最小级别，越大则面积越大
							minZoom: 4,
							zoomControl: true// 地图是否可被缩放控制
						}),
					});
					this.handleAddBatchFeature();
					this.addPopup();
					this.map.on('singleclick', this.changeXY)
					this.map.on("moveend",function(e){
						var zoom = _this.map.getView().getZoom();  //获取当前地图的缩放级别getCenter
						var center = _this.map.getView().getCenter()
						// console.log(zoom,center);
					})
				}
			}).catch(error => {
                console.log('GetSrc failed', error);
            });
		},
		/**
		 * 变换XY坐标  添加摄像机图标  关闭弹窗
 		 * @param e
		**/
		changeXY (e) {
			var closer = document.getElementById("popup-closer");
			// console.log(this.map.getFont())
			var _that = this
			_that.x = e.coordinate[0]
			_that.y = e.coordinate[1]
			console.log(_that.x, _that.y)
			if(this.Maptoken!=undefined){
				var root = this.$store.state.IPPORT;
				var url = root + "/api/v1/MapAddCam?&token="+this.Maptoken+"&longitude="+_that.x+"&latitude="+_that.y+"&session="+ this.$store.state.token;
				this.$http.get(url).then(result=>{
					if(result.status == 200){
						console.log('创建',result)
						if(result.data.bStatus){
							this.MapAddCam(_that.x,_that.y)
							this.getcam();
						}
					}
				})
			}
			if (!_that.map.hasFeatureAtPixel(e.pixel)) {
				if (_that.h5handler != undefined)
				{
					_that.h5handler.disconnect();
					delete _that.h5handler;
					_that.h5handler = undefined;
				}
				document.getElementById('Map_content_token').innerHTML=''
				_that.overlay.setPosition(undefined);
				closer.blur();
				return false;
			}
		},
		//添加设备
		MapAddCam(x,y){
			console.log(x,y)
			var _that = this
			// 创建feature
			let feature = new Feature({
				geometry: new Point([x,y]),
			})
			// 设置ID
			feature.setId(_that.Maptoken)
			// 设置样式
			feature.setStyle(_that.getStyls(feature))
			// 放入features
			_that.features.push(feature)
			
			// 批量添加fe	ature
			_that.flagLayer.getSource().addFeatures(_that.features)
			console.log(_that.features,feature)

			this.Maptoken=undefined
		},
		//点击显示弹窗
		addPopup(x,y) {
            // 使用变量存储弹窗所需的 DOM 对象
            var container = document.getElementById("popup");
            var closer = document.getElementById("popup-closer");
            var content = document.getElementById("popup-content");

            // 创建一个弹窗 Overlay 对象
            this.overlay = new Overlay({
                element: container, //绑定 Overlay 对象和 DOM 对象的
                autoPan: true, // 定义弹出窗口在边缘点击时候可能不完整 设置自动平移效果
                autoPanAnimation: {
                    duration: 250 //自动平移效果的动画时间 9毫秒
                }
            });
            // 将弹窗添加到 map 地图中
            this.map.addOverlay(this.overlay);
            let _that = this;
            /**
             * 添加单击响应函数来处理弹窗动作
             */
            this.map.on("click", function(evt) {
				if (_that.map.hasFeatureAtPixel(evt.pixel)) {
					var feature = _that.map.getFeaturesAtPixel(evt.pixel)
					// console.log(feature,evt)
					if(feature[0].id_==undefined){
						return
					}
					// "EPSG:3857", "EPSG:4326" 转换
					let coordinate = transform(
						evt.coordinate,
						"EPSG:3857",
						"EPSG:4326"
					);
					console.log(_that.camsrc)
					for(var i=0; i<_that.camsrc.length; i++){
						
						if(feature[0].id_==_that.camsrc[i].strToken){
							let strName = _that.orgarr[feature[0].id_ ]||_that.camsrc[i].strName;
							console.log(strName,_that.orgarr[feature[0].id_ ]);
							document.getElementById("Map_content_token").innerHTML = strName;
							$( "#Map_content_token" ).data( "token" ,feature[0].id_ ); 
							_that.videoname=strName;
						}
					}
					if (_that.h5handler != undefined)
					{
						_that.h5handler.disconnect();
						delete _that.h5handler;
						_that.h5handler = undefined;
					}
					let conf = {
						videoid:'Mapvideoid',
						protocol: window.location.protocol, //http: or https:
						host: _that.$store.state.WSROOT, //localhost:8080
						streamprofile: 'main', // {string} - stream profile, main/sub or other predefine transcoding profile
						rootpath: '/', // '/'
						token: feature[0].id_,
						hlsver: 'v1', //v1 is for ts, v2 is for fmp4
						session: _that.$store.state.token //session got from login
					};
					console.log(conf);
					if (_that.$store.state.liveviewrtc == 'RTC' || (H5siOS() === true)){
						console.log(_that.$store.state.liveviewrtc)
						_that.h5handler = new H5sPlayerRTC(conf);
					}else{
						console.log(_that.$store.state.liveviewrtc)
						_that.h5handler = new H5sPlayerWS(conf);
					}
					_that.overlay.setPosition(evt.coordinate); //把 overlay 显示到指定的 x,y坐标
					
				}
			});
			
            /**
             * 为弹窗添加一个响应关闭的函数
             */
            closer.onclick = function() {
				if (_that.h5handler != undefined)
				{
					_that.h5handler.disconnect();
					delete _that.h5handler;
					_that.h5handler = undefined;
				}
				document.getElementById('Map_content_token').innerHTML
                _that.overlay.setPosition(undefined);
                closer.blur();
                return false;
            };
		},
		
		handleAddBatchFeature () {
			console.log(this.coordinates)
			// return
			const _that = this
			// 设置图层
			_that.flagLayer = new VectorLayer({
				source: new VectorSource()
			})
			// 添加图层
			_that.map.addLayer(_that.flagLayer)
			// 循环添加feature
			for (let i = 0; i < this.coordinates.length; i++) {
				// 创建feature
				let feature = new Feature({
					geometry: new Point([_that.coordinates[i].position.strLongitude, _that.coordinates[i].position.strLatitude]),
				})
				// 设置ID
				feature.setId(_that.coordinates[i].strToken)
				// 设置样式
				feature.setStyle(_that.getStyls(feature))
				// 放入features
				_that.features.push(feature)
			} // for 结束
			// 批量添加fe	ature
			_that.flagLayer.getSource().addFeatures(_that.features)
		},
		/**
		 * 设置Style
		 */
		getStyls (feature) {
			let Styles = []
			Styles.push(
				new Style({
					image: new Icon({
						src: this.$store.state.IPPORT+'/img/dev.png',
						anchor: [0.5, 1]
					}),
				})
			)
			// 绘制圆角矩形
			let canvas = document.createElement('canvas')
			let context = canvas.getContext('2d')
			let length = (feature.id_ + '标记点').length + 1
			canvas.width = length * 15
			canvas.height = 35
			let x = 0
			let y = 0
			let w = canvas.width
			let h = canvas.height
			let r = 15
			// 缩放
			context.scale(0.8, 0.8)
			context.fillStyle = 'rgba(255,255,255,1)'
			// 绘制圆角矩形
			context.beginPath()
			context.moveTo(x + r, y)
			context.arcTo(x + w, y, x + w, y + h, r)
			context.arcTo(x + w, y + h, x, y + h, r)
			context.arcTo(x, y + h, x, y, r)
			context.arcTo(x, y, x + w, y, r)
			// 设置阴影
			context.shadowColor = 'rgba(0, 0, 0, 0.2)' // 颜色
			context.shadowBlur = 5 // 模糊尺寸
			context.shadowOffsetX = 2 // 阴影Y轴偏移
			context.shadowOffsetY = 2 // 阴影X轴偏移
			// ----
			context.closePath()
			// 填充
			context.fill()
			// 设置style
			/**Styles.push(
				new Style({
					image: new Icon({
						img: canvas,
						imgSize: [w, h],
						anchor: [0, 1]
					}),
					text: new Text({
						textAlign: 'center',
						token:'100',
						text: '标记点' + feature.id_,
						offsetX: 28,
						offsetY: -17
					})
				})
			)*/
			// console.log(Styles)
			return Styles
		},
		//刷新
        Refresh(type){
			this.data1=[];
			this.Regional();
        },
		//区域
		Regional(){
			var root = this.$store.state.IPPORT;
			var url = root + "/api/v1/GetRegion?session="+ this.$store.state.token;
			this.$http.get(url).then(result=>{
				console.log(result);
				result.data.src.map((item)=>{
					this.orgarr[item.strToken] = item.strName;
				})
				var oldarr=result.data.root;
				var oldarr1=result.data.src;
				// console.log(oldarr,oldarr1)
				var dataroot=this.getchild(oldarr,oldarr1);
				this.datapush(dataroot)
			})
		},
		datapush(data){
			this.a(data)
		},
		a(data){
			console.log(data);
			if(data.node.length!=0){
				for (let i = 0; i < data.node.length; i++) {
					data.cam.push(data.node[i])
				}
			}
			delete data.node
			for (let i = 0; i < data.cam.length; i++) {
				if (data.cam[i].strName!='设备暂时不可用') {
					this.b(data.cam[i])
					this.data1=[];
					this.data1.push(data);
				}
			}
		},
		b(data){
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
		c(data){
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
					if (data.cam[i].cam) {
						this.d(data.cam[i])
					}
				}
			}
		},
		d(data){
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
				if (data.cam[i].cam) {
						this.e(data.cam[i])
				}}
			}
		},
		e(data){
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
                            arr.cam[i].strName = arr1[j].strName;
                            arr.cam[i].name=arr1[j].strName+"--"+this.$t('message.live.mainstream')
                            arr.cam[i].bOnline = arr1[j].bOnline;
                            arr.cam[i].iconclass="iconfont  icon-kaiqishexiangtou1"
                            arr.cam[i].disabled_me=false
                            if(!arr1[j].bOnline)
                                arr.cam[i].iconclass = 'iconfont icon-kaiqishexiangtou';

                            if(arr1[j].nConnectType == 'H5_CLOUD')
                                arr.cam[i].iconclass = 'iconfont  icon-kaiqishexiangtou1';

                            if(arr1[j].bRec == true)
                                arr.cam[i].iconclass2  = 'iconfont icon-radioboxfill none';

                            if(arr1[j].bDisable== true){
                                arr.cam[i].disabled_me=true;
                                arr.cam[i].iconclass1= 'camera';
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
			console.log(diff)
			for(var i in arr.cam){
				for(var j in diff){
					if(arr.cam[i].strToken == diff[j]){
						arr.cam[i].strName = '设备暂时不可用';
						arr.cam[i].iconclass="iconfont  icon-kaiqishexiangtou"
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
		//全屏
        FullScreen(){
            var elem = $("#map").get(0);
            //var elem = $("#videoPanel");
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
                    // this.Electronicoff();
                    // this.Electronicopen();
                } else {
                    // this.Electronicoff();
                    // this.Electronicopen();
                    if (elem.requestFullscreen) {
                        elem.requestFullscreen();
                    } else if (elem.webkitRequestFullscreen) {
                        elem.webkitRequestFullscreen();
                    } else if (elem.mozRequestFullScreen) {
                        elem.mozRequestFullScreen();
                    } else if (elem.msRequestFullscreen) {
                        elem.msRequestFullscreen();
                    }
                }
            } else {
                console.log('Fullscreen is not supported on your browser.');
            }
        },
		//模糊查询
        filterNode(value, data) {
			console.log(value,data)
            if (!value) return true;
            return data.strName.indexOf(value) !== -1;
        }

	}
}
</script>

<style lang="scss" scoped>
.Map{
	position: relative;
	#map{
		width: 100%;
		height: 90vh;
		.ol-popup {
			width: 200px;
			min-height: 150px;
			position: absolute;
			-webkit-filter: drop-shadow(0 1px 4px rgba(0, 0, 0, 0.2));
			filter: drop-shadow(0 1px 4px rgba(0, 0, 0, 0.2));
			// padding: 15px;
			border-radius: 10px;
			border: 1px solid #cccccc;
			bottom: 12px;
			left: -100px;
			background-color: #67686A;
			border-radius: 10px;
			&:after,&:before {
				top: 100%;
				border: solid transparent;
				content: " ";
				height: 0;
				width: 0;
				position: absolute;
				pointer-events: none;
			}
			&:after {
				border-top-color: #67686A;
				border-width: 10px;
				left: 100px;
				margin-left: -10px;
			}
			&:before {
				border-top-color: #67686A;
				border-width: 11px;
				left: 100px;
				margin-left: -11px;
			}
			.ol-popup-closer {
				text-decoration: none;
				position: absolute;
				top: 2px;
				right: 8px;
				z-index: 2;
				&:after {
					content: "✖";
				}
				.popup-content {
					width: 100%;
					
				}
			}
		}
		
	}
	.Map_functionbut{
		min-width: 550px;
		position: absolute;
		top: 30px;
		right: 30px;
		z-index: 10;
		display: flex;
		justify-content: space-between;
		div{
			padding: 4px 8px;
			background-color: RGBA(0, 0, 0, 0.8);
			button{
				font-size: 14px;
				font-family: PingFang SC;
				color: #FFFFFF;
				line-height: 20px;
				i{
					font-size: 18px;
				}
			}
		}
	}
	.Map_core{
		position: absolute;
		bottom: 60px !important;
    	right: 0.35em;
		z-index: 10;
		color: #000;
    	font-size: 24px;
	}
	.Map_Region{
		position: absolute;
		top: 30px;
		left: 30px;
		z-index: 10;
	}
	.dasboard_modal .el-dialog{
		margin-top: 18vh;
        .playback_function{
            display: flex;
            justify-content: space-between;
			.playback_function_icon{
				display: flex;
				div{
					margin-right: 10px;
				}
			}
        }
        .videoo{
            object-fit: fill;
			width: 100%;
			height: 100%
        }
    }
	.text-center{
		&:hover .liveplay_butt{
            display: block;
        }
	}
}
.Map_Popup_title{
	width: 100%;
	height: 110px;
	background-color: #000;
}
.Map_Popup_title .h5video{
	object-fit: fill;
	width: 100%;
	height: 100%
}
.Map_Popup_content{
	width: 100%;
	margin: 10px 0;
	padding: 0 10px;
	line-height: 30px;
	height: 30px;
	display: flex;
	justify-content: space-between;
}
.Map_Popup_content #Map_content_token{
	width: 125px;
	margin: 0;
	overflow: hidden; white-space: nowrap; text-overflow: ellipsis
}
.Map_content_name{
	width: 90%;
	display: flex;
	color: #FFFFFF;
}
.Map_content_play{
	color: #FFFFFF;
}
.Map_content_name i{
	border-radius: 50px;
	padding: 0 7px;
	background-color: #3E3E40;
	color: #2D9BF4;
	margin-right: 5px;
}
/* 比例尺 */
.ol-zoom {
	bottom: 0.5em !important;
	right: 0.5em;
	top: auto;
    left: auto;
}
.ol-scale-line {
	left: 180px !important;
}

.ol-mouse-position{
	color: #000;
}
.liveplay_butt{
	position: absolute;
	display: none;
	top: 0;
	z-index:1012;
	width: 100%;
	height: 40px;
	line-height: 30px;
	background: url('~@/views/liveview/imgs/liveview_buttback.png') no-repeat;
	background-size: 290px 35px;
	background-position-x:right;
	text-align: right;
	padding: 0 15px;
	// /deep/.open_liveplay_butt{
	// 	// display: none;
	// }
	.rotate{
		-webkit-transform: rotateY(180deg);
		transform: rotateY(180deg); 
	}
	.bottom_QR{
		font-size:16px;
		font-weight:600;
		
		text-align: center;
		
		.bottom_scan{
			font-size:16px;
			font-weight:600;
			margin-bottom: 20px;
			padding: 0 20px;
		}
		.bottom_QRcode{
			display: flex;
			div{
				padding: 0 20px;
			}
		}
	}
	button{
		background: none;
		border: none;
		padding: 0;
		margin: 0;
		color: #FFFFFF;
		margin-left: 12px;
	}
	.fw_butt{
		padding:0;display: inline;
		border: none;
		top: -2px;
		right: -12px;
		/deep/ .btn-link{
			padding: 0px 12px 0px 0px !important;
		}
	}
}
// .elqualitytoggle{
// 	// display:block;
// }
// .spanpicturequality{
// 	// display: none !important;
// }
//主副码流
.videoname{ 
	position: absolute;
	bottom: 5px;
	font-size:14px;
	background-color: rgba(0,0,0,0.3);
	color: #FFFFFF;
	padding: 8px 24px;
	text-align: center;
	.spanpicturequality{
		position: absolute;
		bottom: 5px;
		right: -73px;
		border-radius: 10px;
		background-color: #67cfb5;
		border: none;
		width:68px;
		font-size: 10px;
		height: 25px;
		padding: 0 2px;
	}
}
// 音频
.Audio_slider{
	display: none;
	width: 150px;
	position: absolute;
	top: 40px;
	left: 50px;
	z-index:1012;
	.Audio_slider_jingyin{
		color:grey;
	}
	.Audio_slider_shengyin_fill{
		color:#409EFF;
	}
	.el-slider__runway{
		height: 4px;
	}
}
.information {
            position:absolute;
            bottom:40px;
            left: 10px;
            background:rgba(0,0,0,0.5);
            box-sizing:content-box;
            /* z-index:1100; */
            width: 330px;
            height: 150px;
            display:none;
            color: #FFFFFF;
            .information_con{
                width: 100%;
                height: 90%;
                display: flex;
                justify-content: space-between;
                flex-wrap: wrap;
                align-content: space-between;
                .information1{
                    width: 50%;
                    .information_title{
                        width: 100%;
                        height: 30px;
                        line-height: 30px;
                        background-color: rgba(0, 0, 0, 0.7);
                        padding: 0 10px;
                    }
                    .information_content{
                        width: 100%;
                        display: flex;
                        justify-content: space-between;
                        padding: 0 2px;
                        .information_content_left{
                            width: 50%;
                            color: #3ABBFE;
                            text-align: left;
                        }
                        .information_content_right{
                            width: 50%;
                            color: #3ABBFE;
                            text-align: left;
                        }

                    }
                }
            }
        }
</style>