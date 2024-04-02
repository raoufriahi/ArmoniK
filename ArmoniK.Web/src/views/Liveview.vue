<template>
    <div class=liveview>
        <div id="watermarktoggle"></div>
        <!-- Data column on the left -->
		<div class="liveview_left">
            <el-input
                class="liveview_left_input"
                placeholder="Enter keywords to filter"
                v-model="filterText">
            </el-input>
            <el-collapse v-model="activeNames">
                <el-collapse-item name="1" id="headswitch">
                    <template slot="title">
                        <div style="display: flex;justify-content: space-between;width: 85%; align-items: center;">
                            <div>{{$t("message.live.device")}}</div>
                            <div class="liveview_colltitle">
                                <div class="liveview_titleicon1" @click.stop="Refresh('device')"></div>
                                <div @click.stop="headswitch1" class="liveview_titleicon"></div>
                            </div>
                        </div>
                    </template>
                    <el-tree
                        :data="data"
                        node-key="id"
                        :filter-node-method="filterNode"
                        ref="tree"
                        @node-click="handleNodeClick"
                        :props="defaultProps">
                        <span slot-scope="{ data }" style="width:100%;">
                            <div style="width:100%;display: flex;justify-content: space-between;">
                                <!-- draggable="true" 
                                    @dragstart="dragStart($event,data.token,data.label,data.streamprofile, data.name,data.disabled_me,node)" -->
                                <span 
                                    class="size_color" 
                                    draggable="true" 
                                    @dragstart="dragStart($event,data.token,data.label,data.streamprofile, data.name,data.disabled_me)" style="display: flex; justify-content: flex-start; align-items: center;">
                                    <span style="font-size: 12px;" :class="data.iconclass" :id="'icon'+data.token"></span>
                                    <span :class="data.iconclass1" style="padding-left: 4px;">{{data.label}}</span>
                                    <span v-if="data.length" style="padding-left: 4px;">{{data.online}}/{{data.length}}</span>
                                    <el-popover placement="bottom-end" trigger="manual" v-if="data.firstLevel" class="Status" popper-class="popperOptions" style="flex: 1; text-align: right;height: 36px;line-height: 36px;" :ref="refNamePopover+data.statusID">
                                        <div class="StatusPrompt" >
                                            <div style="width:100%;display: flex;justify-content: space-between;">
                                                <span style="font-size:12px;">
                                                    <span style="padding-left:5px">
                                                        <i class="iconfont icon-shexiangji" style="font-size:12px;"></i>
                                                        在线:{{data.online}}
                                                    </span>
                                                    <span class="el-tree-camera" style="padding-left:5px">
                                                        <i class="iconfont icon-shexiangji el-tree-camera" style="font-size:12px;"></i>
                                                        离线:{{data.offline}}
                                                    </span>
                                                    <span class="el-tree-camera" style="padding-left:5px">
                                                        <i class="iconfont icon-jinyong1 el-tree-camera " style="font-size:12px;"></i>
                                                        禁用:{{data.disable}}
                                                    </span>
                                                    <span class="el-tree-camera" style="padding-left:5px">
                                                        <i class="iconfont icon-kaiqishexiangtou1 el-tree-camera" style="font-size:12px;"></i>
                                                        空闲:{{data.idle}}
                                                    </span>
                                                </span>
                                            </div>
                                        </div>
                                        <el-button  slot="reference" type="text"  class="iconfont icon-xianshi2 statusBtn" style="margin-left: 20px;font-size:12px" @click="StatusPrompt(data.statusID,$event)"></el-button>
                                    </el-popover>
                                    <!-- <span v-if="data.firstLevel" @click="StatusPrompt" style="">
                                        <i class="iconfont icon-Close"></i>
                                    </span> -->
                                    <!-- <span v-if="data.offline" class="iconfont icon-kaiqishexiangtou" style="padding-left: 4px;font-size:10px;"> ({{data.offline}})</span> -->
                                </span>
                                <!-- <span :class="data.iconclass2" class="black" style="">{{$t("message.live.Videorecording")}}</span> -->
                                <div style="display:flex">
                                    <span :class="data.iconclass2" class="black" style="">●</span>
                                    <span :class="data.iconclass3" class="nowplay" >
                                        <div style="display:flex">
                                            <span class="dot">●</span>
                                            <span class="nowplayText">正在播放</span>
                                        </div>
                                    </span>
                                </div>
                            </div>
                        </span>
                    </el-tree>
                </el-collapse-item>
                <el-collapse-item name="1" id="headswitch1">
                    <template slot="title">
                        <div style="display: flex;justify-content: space-between;width: 85%; align-items: center;">
                            <div>{{$t("message.live.Region")}}</div>
                            <div class="liveview_colltitle">
                                <div class="liveview_titleicon1" @click.stop="Refresh('Region')"></div>
                                <div @click.stop="headswitch" class="liveview_titleicon"></div>
                            </div>
                        </div>
                    </template>
                   <el-tree
                        class="el_tree"
                        node-key="strToken" 
                        :default-expanded-keys="['root']" 
                        :data="data1" 
                        ref="tree1"
                        :filter-node-method="filterNode1"
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
                                <!-- <span :class="data.iconclass2" class="black" style="">{{$t("message.live.Videorecording")}}</span> -->
                                <div style="display:flex">
                                    <span :class="data.iconclass2" class="black" style="">●</span>
                                    <span :class="data.iconclass3" class="nowplay" >
                                        <div style="display:flex">
                                            <span class="dot">●</span>
                                            <span class="nowplayText">正在播放</span>
                                        </div>
                                    </span>
                                </div>
                            </div>
                        </span>
                        <!-- <span slot-scope="{data }" style="width:100%;">
                            <span>
                                <span class="iconfont icon-quyu"></span>
                                <span :class="data.iconclass1" style="padding-left: 4px;">{{data.strName}}</span>
                            </span>
                            <div v-if="data.cam.length!=0">
                                <el-tree class="el_tree1" :data="data.cam" :props="defaultProps1" @node-click="handleNodeClick">
                                    <span slot-scope="{ data }" style="width:100%;">
                                        <div style="width:100%;display: flex;justify-content: space-between;">
                                            <span  
                                                draggable="true" 
                                                @dragstart="dragStart($event,data.strToken,data.strName,data.streamprofile, data.name,data.disabled_me)">
                                                <span style="font-size: 12px;" :class="data.iconclass" :id="'icon'+data.strToken"></span>
                                                <span :class="data.iconclass1" style="padding-left: 4px;">{{data.strName}}</span>
                                            </span>
                                            <span :class="data.iconclass2" class="black" style="">{{$t("message.live.Videorecording")}}</span>
                                        </div>
                                    </span>
                                </el-tree>
                            </div>
                        </span> -->
                    </el-tree>
                </el-collapse-item>
                <el-collapse-item name="2" id="viewclis">
                    <template slot="title">
                        <div style="display: flex;justify-content: space-between;width: 85%; align-items: center;">
                            <div>{{$t("message.live.View")}}</div>
                            <div class="liveview_colltitle">
                                <div class="liveview_titleicon1" @click.stop="Refresh('view')"></div>
                                 <el-popover
                                    placement="right"
                                    :title="NewView"
                                    width="260"
                                    trigger="click">
                                    <div class="liveview_popover">
                                        <div class="liveview_popover_top">
                                            <div>{{ViewName}}</div>
                                            <el-input 
                                                placeholder="Enter name" 
                                                class="liveview_left_input"
                                                v-model="viewname"></el-input>
                                        </div>
                                        <div class="liveview_popover_but">
                                            <el-button @click.stop="viewadd">{{$t("message.camera.save")}}</el-button>
                                        </div>
                                    </div>
                                    <div @click.stop="view" slot="reference" class="liveview_titleicon2">
                                        <i class="iconfont icon-baocun1"></i>
                                    </div>
                                </el-popover>
                            </div>
                        </div>
                    </template>
                    <el-tree
                        class="el_tree"
                        node-key="strToken" 
                        :default-expanded-keys="['root']" 
                        :data="viewdata" 
                        @node-click="viewClick"
                        :props="defaultProps2">
                        <span slot-scope="{data }" style="width:100%;">
                            <div style="width:100%;display: flex;justify-content: space-between;">
                                <span>
                                    <span :class="data.icon"></span>
                                    <span style="padding-left: 6px;">{{data.strName}}</span>
                                </span>
                                <div style="width:20%;display: flex;justify-content: space-between;">
                                    <div @click.stop="Delview(data.strToken)" class="iconfont icon-ashbin"></div>
                                    <div @click.stop="Editview(data)" class="iconfont icon-zhongmingming"></div>
                                </div>
                            </div>
                        </span>
                    </el-tree>
                </el-collapse-item>
            </el-collapse>
		</div>
		<!-- Video bar on the right -->
        <div class="liveview_right" id="videoPanel">
            <div id="video_hed" style="position: relative;">
                <div name='flex' style="" class="videoColor" v-for="r in rows" :key="r">
                    <div
                        @drop="dropTarget($event,r,c)" 
                        @dragover.prevent="dragover($event)" 
                        class="palace" 
                        name="flex" 
                        v-for="c in cols" 
                        @contextmenu.prevent="stopVideo($event)" 
                        @click="videoClick(r,c,$event)" :key="c"
                        @dblclick="dbclick(r,c,$event)">
                        <v-liveplay 
                            v-bind:id="'h'+r+c" 
                            :h5id="'h'+r+c" 
                            :rows="rows" 
                            :cols="cols" 
                            :h5videoid="'hvideo'+r+c"
                            :canvasid="'canvas'+r+c">
                        </v-liveplay>
                    </div>
                </div>
            </div>
			<div class="liveview_group blocks">
				<el-button type="button" class="iconfont icon-tubiao_huaban1" data-row="1|1" @click="changePanel($event)"></el-button>
				<el-button type="button" class="iconfont icon-tubiao_huaban1fuben" data-row="1|3" @click="changePanel($event)"></el-button>
				<el-button type="button" class="iconfont icon-tubiao_huaban1fuben2" data-row="2|2" @click="changePanel($event)"></el-button>

				<el-button type="button" class="iconfont icon-sansi" data-row="1|4" @click="changePanel($event)"></el-button>
				<el-button type="button" class="iconfont icon-tubiao_huaban1fuben3" data-row="1|6" @click="changePanel($event)"></el-button>
				<el-button type="button" class="iconfont icon-tubiao_huaban1fuben4" data-row="1|7" @click="changePanel($event)"></el-button>

				<el-button type="button" class="iconfont icon-tubiao_huaban1fuben5" data-row="3|3" @click="changePanel($event)"></el-button>

				
				<el-button type="button" class="iconfont icon-tubiao_huaban1fuben6" data-row="1|13" @click="changePanel($event)"></el-button>

				<el-button type="button" class="iconfont icon-qietu16" data-row="4|4" @click="changePanel($event)"></el-button>
				<el-button type="button" class="iconfont icon-qietu25" data-row="5|5" @click="changePanel($event)"></el-button>
				<el-button type="button" class="iconfont icon-tubiao_huaban1fuben9" @click="panelFullScreen($event)"> </el-button>
                <el-button type="button"  class="iconfont icon-quanbuguanbi-01 offAllVideo" @click="Alloffvideo"></el-button>
			</div>
            <div id="menu" @click="aievent()" class="iconfont " :class="menu_data"></div>
		</div>
        <div class="liveview_right2">
            <el-tabs type="border-card" class="border_card">
                <el-tab-pane class="tab_pane">
                    <span slot="label"><i class="iconfont icon icon-wenjianjiqun"></i></span>
                    <div id="aicanvas"></div>
                </el-tab-pane>
                <el-tab-pane class="tab_pane">
                    <span slot="label"><i class="iconfont icon icon-hangrenlujing"></i></span>
                    <div id="aicanvasr"></div>
                </el-tab-pane>
                <el-tab-pane class="tab_pane">
                    <span slot="label"><i class="iconfont icon icon-cheliang-"></i></span>
                    <div id="aicanvash"></div>
                </el-tab-pane>
                <el-tab-pane class="tab_pane">
                    <span slot="label"><i class="iconfont icon icon-renlian"></i></span>
                    <div id="aicanvasl"></div>
                </el-tab-pane>
            </el-tabs>
        </div>
        <el-dialog
            :before-close="handleClose"
            class="dasboard_modal"
            :title="label.Playback"
            :visible.sync="myModal1"
            width="35%">
            <div style="display: flex;justify-content: space-between;">
                <span>{{$t("message.archive.StartTime")}}:{{rowstarf}}</span>
                <span>{{$t("message.archive.EndTime")}}:{{rowend}}</span>
            </div>
            <div class="text-center">
                <div style="position:relative;max-height:356px;">
                    <video class="videoo" id="pbplayarch"></video>
                    <div id="rtcid" class="rtc_new"></div>
                </div>
                <div class="block" @mousedown ="timelinndown($event)" @mouseup="timelinnup($event)">
                    <el-slider v-model="timelink" :max="max" @change="timelinn(timelink)" :show-tooltip="false"></el-slider>
                    <!-- <canvas class="timeline" id="timeline" style="width:100%;" height="50px">
                    </canvas> -->
                </div>
                <!-- <el-button :class="icon" @click="resume()" class="strart"></el-button> -->
                <div class="playback_function">
                    <div class="back_Choice"  @mouseenter="enter1()" @mouseleave="leave1()">
                        <div id="back_Choice" class="back_Choice1">
                            <div :class="{co_Baise:Adswitch=='true'}" @click="H5SPlayback()" class="co_black" >{{$t("message.playback.H5SPlayback")}}</div>
                            <div :class="{co_Baise:Adswitch=='false'}" @click="NVRPlayback()" class="co_black">{{$t("message.playback.NVRPlayback")}}</div>
                        </div>
                        <span class="back_zi iconfont icon-gengduo"></span>
                        <span v-if="Adswitch=='true'" class="">&nbsp;&nbsp;{{$t("message.playback.H5SPlayback")}}</span> 
                        <span v-else class="">&nbsp;&nbsp;{{$t("message.playback.NVRPlayback")}}</span>
                    </div>
                    <div :class="icon" @click="resume()" class="strart" style="margin-right:20px;"></div>
                    <!-- 倍速 -->
                    <el-select v-model="region" filterable remote class="ele" size="mini" style="width:70px" @change="Speed()">
                        <el-option
                            v-for="item in regiondata"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                    <!-- 实时时间 -->
                    <span style="margin-left:80px;">{{displayc}}</span>
                </div>
            </div>
        </el-dialog>
    </div>
</template>
<script>
import Vue from 'vue'
import liveplay from './liveview/Liveplay'
import uuid from '../assets/js/uuid'
import {IotEventExt} from '../assets/js/ioteventext'
import {H5sPlayerWS,H5sPlayerHls,H5sPlayerRTC,H5sPlayerAudBack} from '../assets/js/h5splayer.js'
export default {
    name: 'Liveview',
    components: {
        'v-liveplay': liveplay
    },
	data(){
		return{
            uuid:uuid(),
            v1:undefined,
            h5sid:{},
            ai_state:true,
            ai_obj:{},
            ai_websocket:'',
            label:{
                Name:this.$t("message.table.Name"),
                Token:this.$t("message.table.Token"),
                Time:this.$t("message.table.Time"),
                StartTime:this.$t("message.table.StartTime"),
                EndTime:this.$t("message.table.EndTime"),
                Playback:this.$t("message.archive.Playback"),
                Type:this.$t("message.table.Type")
            },
            ai_token:{},
            menu_data:'icon-xianshi1',
            aistate:false,
            myModal1:false,
            Adswitch:'false',//开关
            rowstarf:"",//跟进进度条开始时间
            rowend:"",//进度条结束时间
            max:0,//滑块最大值
            timelink:0,//滑块
            icon:'icon_start',
            region:'1.0',
            displayc:"",//实时时间
            regiondata:[{
                    value: "0.5",
                    label: "X0.5"
                }, {
                    value: "1.0",
                    label: "X1"
                }, {
                    value: "2.0",
                    label: "X2"
                }, {
                    value: "4.0",
                    label: "X4"
                }, {
                    value: "8.0",
                    label: "X8"
                }, {
                    value: "16.0",
                    label: "X16"
            }],
            aidata:[],
            viewlists:{},
            NewView:this.$t("message.live.NewView"),
            ViewName:this.$t("message.live.ViewName"),
            cur_query:this.$route.query || {},
            viewname:null,//View name
            viewdata:[],//View data
            proto: this.$store.state.liveviewrtc,
            activeNames: ['1','2'],//Left
			rows: 0,
			cols: 0,
			selectCol: 1,
			selectRow: 1,
			contentHeight: '',
            contentWidth: '',
            filterText:"",//Search bar
            data:[],
            data1:[],
            defaultProps: {
                children: 'children',
                label: 'label',
                token:"token",
                iconclass:"iconclass",
                length:'length',
                online:'online',
                offline:'offline',
            },
            defaultProps1: {
                children: 'cam',
                label: 'strName',
                cam:"cam"
            },
            defaultProps2:{
                label: 'strName',
            },
            objlist:{},
            dbclicks:true,
            drags:true,
            h55id:'',
            refNamePopover: 'popover-', // popover ref名称前缀
            displayc:"",//实时时间
            ai_index:true,
            icon:'icon_start',
            treeNodeId:[],
            timer:null,
            dbclickr:'',
            dbclickc:''
		}
    },
	mounted(){
        if (this.$route.query.view) {
        }else{
            if (localStorage.getItem(this.$store.state.user)) {
                if (JSON.parse(localStorage.getItem(this.$store.state.user))['src']==[]) {
                }else{
                    this.cols = 3;
                    this.rows = 3;
                }
            }else{
                this.cols = 3;
                this.rows = 3;
            }
        }
        window.clickFun = this.clickFun;
        this.viewplay();
        if (this.$route.query.fullscreen=='true') {
            $('.liveview_right2').hide();
            $('#menu').hide();
        }else{
            $('.liveview_right2').show();
            $('#menu').show();
        }
        setTimeout(() => {
            if (_this.cur_query.embed=='true') {
                if (_this.cur_query.fullscreen=="false") {
                    _this.updateUI(0.89);
                }else if (_this.cur_query.fullscreen=="true") {
                    _this.updateUI(0.98);
                }else{
                    _this.updateUI(0.89);
                }
            }else if(_this.cur_query.embed=='false'){
                if (this.$route.query.fullscreen=='true') {
                    this.updateUI(0.89);
                }else if(_this.cur_query.fullscreen=="false"){
                    this.updateUI(0.82);
                }else {
                    this.updateUI(0.82);
                }
            }else{
                if (this.$route.query.fullscreen=='true') {
                    this.updateUI(0.89);
                }else if(_this.cur_query.fullscreen=="false"){
                    this.updateUI(0.82);
                }else {
                    this.updateUI(0.82);
                }
            }
            this.licenseWaterMark();
            this.addWaterMarker();
            this.ai_h5sid();
        }, 1000);
        if(this.$store.state.root=="Operator"){
                $('#headswitch').hide()
                $('#viewclis').hide()
            }else{
                $('#headswitch1').hide()
            }
        // this.loadtest();
        this.$nextTick(()=>{
            if(this.timer) {
                clearTimeout(this.timer)
                this.timer = null
            }
            let __this = this;
            this.timer = setTimeout(()=> {
                __this.loadtest();
                __this.loadDevice();
                __this.NumberDevice();
                __this.cloudDevice();
                __this.Regional();
                __this.srcview();
            }, 1000)
        })
        this.viewlist();
        let _this = this
        this.$root.bus.$on('close',function(){
            $(_this.h55id).removeClass('dbclick')
            _this.dbclicks=true
        })
        _this.$root.bus.$on('closeToken',function(token){
            _this.closePlayvideo(token);
        });
        _this.$root.bus.$on('recToken',function(token){
            _this.elTreeBrec(token);
        });
        _this.$root.bus.$on('closerecToken',function(token){
            _this.closeelTreeBrec(token);
        });
        _this.$root.bus.$on('ai_click',function(data){
            _this.ai_index = !data
        });
        _this.$root.bus.$on('RootCloseToken',function(id,level,token){
            _this.closeRootPlayvideo(id,level,token);
        });
        _this.$root.bus.$on('RootrecToken',function(token){
            _this.RootrecToken(token);
        });
        _this.$root.bus.$on('RootcloserecToken',function(token){
            _this.RootcloserecToken(token);
        });
        _this.$root.bus.$on('dragToken',function(token){
            for (var k = 0; k < _this.data.length;k++){
                var data= _this.data[k].children;
                if (data.length>0) {
                    for (var i = 0; i < data.length;i++){
                        if (token==data[i].token) {
                            data[i].iconclass = 'iconfont icon-shexiangji el-tree-camera-play';
                            data[i].iconclass1 = 'el-tree-camera-play';
                            data[i].iconclass3 = 'none';
                        }
                    }
                }
            }
        })
	},
	methods:{
        H5SPlayback(){
            this.region='1.0';
            this.icon="icon_start";
            this.timelink=0;
            this.v1.disconnect();
            delete this.v1;
            this.v1 = undefined;
            $('#pbplayarch').get(0).load();
            this.Adswitch = 'true';
            console.log(this.Adswitchplay);
            this.Refresh1(this.Adswitchplay);
        },
        NVRPlayback(){
            this.region='1.0';
            this.icon="icon_start";
            this.timelink=0;
            this.v1.disconnect();
            delete this.v1;
            this.v1 = undefined;
            $('#pbplayarch').get(0).load();
            this.Adswitch = 'false';
            console.log(this.Adswitchplay);
            this.Refresh1(this.Adswitchplay);
        },
        //开始
        resume(){
            var strart=this.icon;
            console.log(strart);
            if(strart=="icon_start"){
                console.log(strart);
                this.icon="icon_stop";
                this.v1.pause();
            }
            if(strart=="icon_stop"){
                console.log(strart);
                this.icon="icon_start";
                this.v1.resume();
            }
        },
        ai_h5sid(date){
            this.ai_obj={}
            this.h5sid={}
            if (date) {
            }else{
                date = JSON.parse(localStorage.getItem(this.$store.state.user));
            }
            
            let obj = ['1|1','1|3',"2|2",'1|4','1|6','1|7','3|3'];
            if (obj.indexOf(date.strLayoutType)!=-1) {
                this.ai_state=true
            }else{
                this.ai_state=false
            }
            for (let i = 0; i < date.src.length; i++) {
                this.ai_obj[date.src[i].strToken]= 'canvas2'+date.src[i].strIndex;
                this.h5sid[date.src[i].strToken]= date.src[i].strIndex;
                
            }
        },
        //倍速
        Speed(){
            console.log( this.region);
            this.v1.speed(this.region);
        },
        enter1(){
			$(".back_Choice1").css("display","block");
			// document.getElementById("back_Choice").style.display="block";
		},
        leave1(){
			$(".back_Choice1").css("display","none");
			// document.getElementById("back_Choice").style.display="none";
		},
        //拖
        timelinndown(err){
            console.log('a',err)
            // this.v1.pause();
        },
        timelinnup(err){
            console.log('a',err)
            // this.v1.resume();
        },
        timelinn(timelink){
            console.log(timelink,this.max);
            this.v1.seek(timelink);
        },
        PlaybackCB(event, userdata){
            var msgevent = JSON.parse(event);
            if (msgevent.type === 'H5S_EVENT_PB_TIME')
            {
                this.displayc=msgevent.pbTime.strTime;
                var starf=new Date(this.rowstarf).getTime()/1000;
                var endd=new Date(msgevent.pbTime.strTime).getTime()/1000;
                var staefend=endd-starf;
                this.timelink=staefend;
                if (staefend>this.max) {
                    console.log(staefend,this.max);
                    // this.v1.resume();
                }
            }
        },
        //播放
        Refresh1(row){
            this.Adswitchplay=row;
            if (this.v1 != undefined){
                this.v1.disconnect();
                delete this.v1;
                this.v1 = undefined;
            }
            this.rowstarf=row.starf;
            this.rowend=row.end;
            let time =new Date().getTime();
            if (new Date(row.end).getTime()>time) {
                this.rowend = this.ai_time(time)
            }
            // this.url=row.url;
            this.myModal1=true
            var pbconf1 = {
                begintime: row.starf,
                endtime: row.end,
                showposter: 'false', //'true' or 'false' show poster
                callback: this.PlaybackCB,
                serverpb: this.Adswitch,
                userdata:  this // user data
            };
            this.$nextTick(()=>{
                let conf = {
                    videoid: "pbplayarch",
                    protocol: window.location.protocol, //http: or https:
                    host: this.$store.state.WSROOT, //localhost:8080
                    rootpath:'/', // '/'
                    token:row.token,
                    pbconf: pbconf1, //This is optional, if no pbconf, this will be live.
                    hlsver:'v1', //v1 is for ts, v2 is for fmp4
                    consolelog:'false',
                    session:this.$store.state.token
                };
                var end=new Date(row.end).getTime();
                var starf=new Date(row.starf).getTime();
                var starfend=(end-starf)/1000;//时间差
                this.max=starfend;
                if (this.$store.state.liveviewrtc1=='RTC') {
                    this.v1 = new H5sPlayerRTC(conf);
                }else{
                    $('#rtcid').removeClass("rtc_new");
                    this.v1 = new H5sPlayerWS(conf);
                }
                this.v1.connect();
                setTimeout(function(){
                    this.v1.start();
                }.bind(this),500);
            })
        },
        //关闭
        handleClose(){
            this.myModal1=false;
            if (this.v1 != undefined){
                this.region='1.0';
                this.icon="icon_start";
                this.timelink=0;
                this.v1.disconnect();
                delete this.v1;
                this.v1 = undefined;
                $('#pbplayarch').get(0).load();
            }
        },
        clickFun(e){
            console.log(e);
            let token = e.id.split('|')[0];
            let time = e.id.split('|')[1];
            let startTime = new Date(time).getTime()-5*60*1000;//开始时间
            let endTime = new Date(time).getTime()+30000;//结束时间
            let row = {
                token : token,
                starf : this.ai_time(startTime),
                end : this.ai_time(endTime)
            }
            this.myModal1=true;
            this.Refresh1(row)

        },
        ai_time(time){
            var timevalue=new Date(time);
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
            return year+"-"+month+"-"+strDate+"T"+""+getHours+":"+getMinutes+":"+getSeconds+""+"+0"+localOffset+":00";
        },
        aievent(){
            if (this.aistate) {
                console.log(this.aistate,2);
                $('.liveview_right').width('84%');
                $('.liveview_right2').width('0%');
                $('.liveview_right2').hide();
                this.aistate=!this.aistate;
                this.menu_data="icon-xianshi1"
            }else{
                $('.liveview_right').width('75%')
                $('.liveview_right2').width('16.5%')
                $('.liveview_right2').show();
                this.aistate=!this.aistate;
                this.menu_data="icon-yincang"
                
            }
        },
        ioeventai(){
            console.log(IotEventExt);
            let gEvent=[];
            let e1=undefined;
            let _this = this;
            let time1 = {};
            let time2 = {};
            function EventCB(event, userdata){
                var msgevent = JSON.parse(event);
                console.log(msgevent);
                let baseUrl = msgevent.images;
                let data = msgevent.object;
                // let width = msgevent.resolution.width;
                // let height = msgevent.resolution.height;
                let token = msgevent.token;
                // let baseUrl = "iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0 DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg=="
                for (let i = 0; i < data.length; i++) {
                    if (data[i].w<16||data[i].h<16) {
                        continue;
                    }
                    let aicanvas = document.getElementById("aicanvas");
                    let aicanvasr = document.getElementById("aicanvasr");
                    let aicanvash = document.getElementById("aicanvash");
                    let chlingth=aicanvas.childNodes;
                    if (chlingth.length>99) {
                        aicanvas.removeChild(chlingth[99]);
                    }
                    let chlingthr=aicanvasr.childNodes;
                    if (chlingthr.length>99) {
                        aicanvasr.removeChild(chlingthr[99]);
                    }
                    let chlingthh=aicanvash.childNodes;
                    if (chlingthh.length>99) {
                        aicanvash.removeChild(chlingthh[99]);
                    }
                    // console.log(chlingth.length);
                    let car_type = {'car':'轿车','van':'货车','truck':'卡车','bus':'公交车'}
                    let id = data[i].id;
                    let x = data[i].x;
                    let y = data[i].y;
                    let w = data[i].w;
                    let h = data[i].h;
                    let label = data[i].label;
                    let labels = data[i].labels;
                    let time_ai = data[i].time;
                    let type_ai;
                    if (label=='person'||label=='head') {
                        type_ai = '人';
                    }else if(label=='vehicle'){
                        type_ai = '车';
                    }else if(label=='hat'){
                        type_ai = '安全帽';
                    }else {
                        type_ai = '人脸';
                    }
                    let car = '类型:';
                    // $("#container21").prepend(`<div><canvas id="c${id}"></canvas></div>`);
                    var divs=document.createElement("div");
                    divs.innerHTML=`<div class="ai_theader c${id}" style="border-bottom:2px solid #181818;"> <div class="">
                    <div style="padding:8px 0px 8px 5px;"><span class="iconfont icon-ditu-dibiao"></span> &nbsp; ${_this.objlist[token]}</div>
                    <div style="display:flex;">
                    <div style="background:#181818;margin-bottom: 2px;"><canvas id="c${id}${label}"></canvas> </div>
                    <div><div style="padding:2px 0px 2px 4px;">目标检测:${type_ai}</div>
                    <div ${label == 'vehicle'&&labels != undefined?'style="padding:2px 0px 2px 4px;"':''}>${label == 'vehicle'&&labels != undefined?car+labels:''}</div>
                    <div style="padding:2px 0px 2px 4px;">${time_ai.split('T')[0]}</div><div style="padding:2px 0px 2px 4px;">${time_ai.split('T')[1].split('+')[0]}</div>
                    <div style="padding:2px 0px 2px 4px;">分辨率:${w}x${h}</div>
                    <div class='ai_replay iconfont icon-huifangtubiao' id="${token}|${time_ai}" style="padding:2px 0px 2px 14px;" onclick="clickFun(this)"></div></div></div></div>
                    <div class=" img_theader img${id}" style="padding:0 0 4px 4px; display:none;"><img src="data:image/jpeg;base64,${baseUrl}" width="263" height="190"></div>
                    </div>  `;
                    document.getElementById("aicanvas").prepend(divs)
                    let ai_canvas =document.getElementById("c"+id+label);
                    ai_canvas.width=150;
                    ai_canvas.height=150;
                    let context = ai_canvas.getContext('2d');
                    let img = new Image();
                    img.src = `data:image/jpeg;base64,${baseUrl}`;
                    // console.log(img, x, y,w,h,0,0,width,height);
                    // context.drawImage(img, x, y,w,h,0,0,width,height);
                    // context.drawImage(img,0,0);
                    img.onload = () => {
                        if (75>w&&75>h&&w<150) {
                            context.drawImage(img, x, y,w,h,(150-w*1.5)/2,(150-h*1.5)/2,w*1.5,h*1.5);
                        }else if(w>=150){
                            context.drawImage(img, x, y,w,h,0,0,150,150);
                        }else{
                            context.drawImage(img, x, y,w,h,(150-w)/2,(150-h)/2,w,h);
                        }
                        let date = JSON.parse(localStorage.getItem(_this.$store.state.user));
                        for (let i = 0; i < date.src.length; i++) {
                            if (date.src[i].strToken == token&&_this.ai_state &&_this.ai_index&&_this.$store.state.Ai_devicemarktoggle=='block') {
                                if (time1['canvas2'+date.src[i].strIndex]) {
                                    clearTimeout(time1['canvas2'+date.src[i].strIndex]);
                                }
                                let canvas2 =document.getElementById('canvas2'+date.src[i].strIndex);
                                let context2 = canvas2.getContext('2d');
                                canvas2.width=$('#'+'canvas2'+date.src[i].strIndex).width();
                                canvas2.height=$('#'+'canvas2'+date.src[i].strIndex).height();
                                context2.fillStyle = '#000 ';
                                context2.clearRect(0, 0, canvas2.width, canvas2.height);
                                if (w>120) {
                                    context2.fillRect(canvas2.width-120-10,22,canvas2.width,5+(h*120)/w);
                                    context2.drawImage(img, x, y,w,h,canvas2.width-120-5,25,120,(h*120)/w);
                                }else{
                                    context2.fillRect(canvas2.width-w-10,22,canvas2.width,5+h);
                                    context2.drawImage(img, x, y,w,h,canvas2.width-w-5,25,w,h);
                                }
                                time1['canvas2'+date.src[i].strIndex] = setTimeout(()=>{
                                    context2.clearRect(0, 0, canvas2.width, canvas2.height);
                                },10000)
                                if (label=='head') {
                                    if (time2["war"+date.src[i].strIndex]) {
                                        clearTimeout(time2["war"+date.src[i].strIndex]);
                                    }
                                    $("#war"+date.src[i].strIndex).show();
                                    $("#"+date.src[i].strIndex).addClass('h5videoh1');
                                    time2["war"+date.src[i].strIndex] = setTimeout(()=>{
                                        console.log(111);
                                        $("#war"+date.src[i].strIndex).hide();
                                        $("#"+date.src[i].strIndex).removeClass('h5videoh1');
                                    },10000)
                                }
                            }
                        }
                    };
                    $('.c'+id).click(function(e){
                        console.log(this);
                        $(".ai_theader").css('border','none');
                        $(".ai_theader").css('border-bottom','2px solid #181818');
                        $(".img_theader").css('display','none');
                        this.style.border = '1px solid #409EFF';
                        this.children[1].style.display='block';
                    })
                    if (type_ai == '人'||type_ai == '安全帽') {
                        _this.ai_types('r',id,token,type_ai,time_ai,baseUrl,w,h,x,y);
                    }else if (type_ai == '车') {
                        _this.ai_types('h',id,token,type_ai,time_ai,baseUrl,w,h,x,y,labels);
                    }else{
                        _this.ai_types('l',id,token,type_ai,time_ai,baseUrl,w,h,x,y);
                    }
                }
            }
            let obj = {type:{types:["person_vehicle","vehicle","face",'head','hat'],uuid:this.uuid}};
            var conf1 = {
                protocol: window.location.protocol, //http: or https:
                host:this.$store.state.WSROOT, //localhost:8080
                rootpath:'/', // '/'
                callback: EventCB, 
                userdata: null, // user data
                session: this.$store.state.token, //session got from login
                Subscription: obj,
                type:'ai'
            };
            e1 = new IotEventExt(conf1);
            e1.connect();
            this.ai_websocket = e1;
        },
        ai_types(c,id,token,type_ai,time_ai,baseUrl,w,h,x,y,labels){
            let car = '类型:';
            let car_type = {'car':'轿车','van':'货车','trcuk':'卡车','bus':'公交车'}
            var divr=document.createElement("div");
            divr.innerHTML=`<div class="ai_theader c${id}" style="border-bottom:2px solid #181818;"> <div class="">
            <div style="padding:8px 0px 8px 5px;"><span class="iconfont icon-ditu-dibiao"></span> &nbsp; ${this.objlist[token]}</div>
            <div style="display:flex;">
            <div style="background:#181818;margin-bottom: 2px;"><canvas id="${c}${id}"></canvas> </div>
            <div><div style="padding:2px 0px 2px 4px;">目标检测:${type_ai}</div><div style="padding-left:4px;">${type_ai == '车'&&car_type[labels]!=undefined?car+car_type[labels]:''}</div>
            <div style="padding:2px 0px 2px 4px;">${time_ai.split('T')[0]}</div><div style="padding:2px 0px 2px 4px;">${time_ai.split('T')[1].split('+')[0]}</div>
            <div class='ai_replay iconfont icon-huifangtubiao' id="${token}|${time_ai}" style="padding:2px 0px 2px 14px;" onclick="clickFun(this)"></div></div></div></div>
            <div class=" img_theader img${id}" style="padding:0 0 4px 4px; display:none;"><img src="data:image/jpeg;base64,${baseUrl}" width="263"></div>
            </div>  `;
            document.getElementById("aicanvas"+c).prepend(divr)
            let air_canvas =document.getElementById(c+id);
            air_canvas.width=150;
            air_canvas.height=150;
            let contextr = '';
            // console.log(air_canvas);
            try {
                contextr = air_canvas.getContext('2d');
                let img = new Image();
                img.src = `data:image/jpeg;base64,${baseUrl}`;
                img.onload = () => {
                    if (75>w&&75>h) {
                        contextr.drawImage(img, x, y,w,h,(150-w*1.5)/2,(150-h*1.5)/2,w*1.5,h*1.5);
                    }else{
                        contextr.drawImage(img, x, y,w,h,(150-w)/2,(150-h)/2,w,h);
                    }
                };
                $('.c'+id).click(function(e){
                    $(".ai_theader").css('border','none');
                    $(".ai_theader").css('border-bottom','2px solid #181818');
                    $(".img_theader").css('display','none');
                    this.style.border = '1px solid #409EFF';
                    this.children[1].style.display='block';
                })
            } catch (error) {
                return
            }
            // console.log(img, x, y,w,h,0,0,width,height);
            // context.drawImage(img, x, y,w,h,0,0,width,height);
            // context.drawImage(img,0,0);
            
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
        closeelTreeBrec(token){
            for (var k = 0; k < this.data.length;k++){
                var data= this.data[k].children;
                if (data.length>0) {
                    for (var i = 0; i < data.length;i++){
                        // if (data[i].bRec==true) {
                            if (token==data[i].token) {
                                data[i].iconclass2 = '';
                            }    
                        // }
                    }
                }
            }
        },
        elTreeBrec(token){
            for (var k = 0; k < this.data.length;k++){
                var data= this.data[k].children;
                if (data.length>0) {
                    for (var i = 0; i < data.length;i++){
                        // if (data[i].bRec==true) {
                            if (token==data[i].token) {
                                data[i].iconclass2 = 'none';
                            }    
                        // }
                    }
                }
            }
        },
        closePlayvideo(token){
            for (var k = 0; k < this.data.length;k++){
                var data= this.data[k].children;
                if (data.length>0) {
                    for (var i = 0; i < data.length;i++){
                        if (data[i].bOnline==true) {
                            if (token==data[i].token) {
                                data[i].iconclass = 'iconfont icon-shexiangji ';
                                data[i].iconclass1 = '';
                                data[i].iconclass3 = '';
                            }    
                        }
                    }
                }
            }
        },
        closeRootPlayvideo(id,level,token){
            for (var k = 0; k < this.data1.length;k++){
                var data= this.data1[k].cam;
                if (data.length>0) {
                    for (var i = 0; i < data.length;i++){
                        if (data[i].bOnline==true) {
                            if (id==data[i].treeid) {
                                data[i].iconclass = 'iconfont icon-shexiangji ';
                                data[i].iconclass1 = '';
                                data[i].iconclass3 = '';
                            }else{
                                if (level==this.data1[k].firstLevel){
                                    if (token==data[i].strToken) {
                                        data[i].iconclass = 'iconfont icon-shexiangji ';
                                        data[i].iconclass1 = '';
                                        data[i].iconclass3 = '';
                                    } 
                                }
                            }
                        }else{
                            if (data[i].cam) {
                                this.rootfirst(data[i].cam,id,level,token,data[i].firstLevel);
                            }
                        }
                    }
                }
            }
        },
        rootfirst(data,id,level,token,dataLevel){
            for (var i = 0; i < data.length;i++){
                if (data[i].bOnline==true) {
                    if (id==data[i].treeid) {
                        data[i].iconclass = 'iconfont icon-shexiangji ';
                        data[i].iconclass1 = '';
                        data[i].iconclass3 = '';
                    }else{
                        if (level==dataLevel){
                            if (token==data[i].strToken) {
                                data[i].iconclass = 'iconfont icon-shexiangji ';
                                data[i].iconclass1 = '';
                                data[i].iconclass3 = '';
                            } 
                        }
                    }
                }else{
                    if (data[i].cam) {
                        this.roottwo(data[i].cam,id,level,token,data[i].firstLevel);
                    }
                }
            }
        },
        roottwo(data,id,level,token,dataLevel){
            for (var i = 0; i < data.length;i++){
                if (data[i].bOnline==true) {
                    if (id==data[i].treeid) {
                        data[i].iconclass = 'iconfont icon-shexiangji ';
                        data[i].iconclass1 = '';
                        data[i].iconclass3 = '';
                    }else{
                        if (level==dataLevel){
                            if (token==data[i].strToken) {
                                data[i].iconclass = 'iconfont icon-shexiangji ';
                                data[i].iconclass1 = '';
                                data[i].iconclass3 = '';
                            } 
                        }
                    }
                }else{
                    if (data[i].cam) {
                        this.rootthree(data[i].cam,id,level,token,data[i].firstLevel);
                    }
                }
            }
        },
        rootthree(data,id,level,token,dataLevel){
            for (var i = 0; i < data.length;i++){
                if (data[i].bOnline==true) {
                    if (id==data[i].treeid) {
                        data[i].iconclass = 'iconfont icon-shexiangji ';
                        data[i].iconclass1 = '';
                        data[i].iconclass3 = '';
                    }else{
                        if (level==dataLevel){
                            if (token==data[i].strToken) {
                                data[i].iconclass = 'iconfont icon-shexiangji ';
                                data[i].iconclass1 = '';
                                data[i].iconclass3 = '';
                            } 
                        }
                    }
                }else{
                    if (data[i].cam) {
                        this.rootfour(data[i].cam,id,level,token,data[i].firstLevel);
                    }
                }
            }
        },
        rootfour(data,id,level,token,dataLevel){
            for (var i = 0; i < data.length;i++){
                if (data[i].bOnline==true) {
                    if (id==data[i].treeid) {
                        data[i].iconclass = 'iconfont icon-shexiangji ';
                        data[i].iconclass1 = '';
                        data[i].iconclass3 = '';
                    }else{
                        if (level==dataLevel){
                            if (token==data[i].strToken) {
                                data[i].iconclass = 'iconfont icon-shexiangji ';
                                data[i].iconclass1 = '';
                                data[i].iconclass3 = '';
                            } 
                        }
                    }
                }else{
                    if (data[i].cam) {
                        this.rootfive(data[i].cam,id,level,token,data[i].firstLevel);
                    }
                }
            }
        },
        rootfive(data,id,level,token,dataLevel){
            for (var i = 0; i < data.length;i++){
                if (data[i].bOnline==true) {
                    if (id==data[i].treeid) {
                        data[i].iconclass = 'iconfont icon-shexiangji ';
                        data[i].iconclass1 = '';
                        data[i].iconclass3 = '';
                    }else{
                        if (level==dataLevel){
                            if (token==data[i].strToken) {
                                data[i].iconclass = 'iconfont icon-shexiangji ';
                                data[i].iconclass1 = '';
                                data[i].iconclass3 = '';
                            } 
                        }
                    }
                }
            }
        },
        // 区域开启录像
        RootrecToken(token){
            for (var k = 0; k < this.data1.length;k++){
                var data= this.data1[k].cam;
                if (data.length>0) {
                    for (var i = 0; i < data.length;i++){
                        if (token==data[i].strToken) {
                            data[i].iconclass2 = 'none';
                            this.$set(this.data1, data[i].iconclass2, "none");
                        }
                        if (data[i].cam) {
                            this.RootRecFirst(data[i].cam,token);
                        }
                    }
                }
            }
        },
        RootRecFirst(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = 'none';
                    this.$set(this.data1, data[i].iconclass2, "none");
                }
                if (data[i].cam) {
                    this.RootRecTwo(data[i].cam,token);
                }
            }
        },
        RootRecTwo(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = 'none';
                    this.$set(this.data1, data[i].iconclass2, "none");
                }
                if (data[i].cam) {
                    this.RootRecThree(data[i].cam,token);
                }
            }
        },
        RootRecThree(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = 'none';
                    this.$set(this.data1, data[i].iconclass2, "none");
                }
                if (data[i].cam) {
                    this.RootRecFour(data[i].cam,token);
                }
            }
        },
        RootRecFour(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = 'none';
                    this.$set(this.data1, data[i].iconclass2, "none");
                }
                if (data[i].cam) {
                    this.RootRecFive(data[i].cam,token);
                }
            }
        },
        RootRecFive(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = 'none';
                    this.$set(this.data1, data[i].iconclass2, "none");
                }
            }
        },
        // 区域关闭录像
        RootcloserecToken(token){
            for (var k = 0; k < this.data1.length;k++){
                var data= this.data1[k].cam;
                if (data.length>0) {
                    for (var i = 0; i < data.length;i++){
                        if (token==data[i].strToken) {
                            data[i].iconclass2 = '';
                            this.$set(this.data1, data[i].iconclass2, "");
                        }
                        if (data[i].cam) {
                            this.CloseRootRecFirst(data[i].cam,token);
                        }
                    }
                }
            }
        },
        CloseRootRecFirst(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = '';
                    this.$set(this.data1, data[i].iconclass2, "");
                }
                if (data[i].cam) {
                    this.CloseRootRecTwo(data[i].cam,token);
                }
            }
        },
        CloseRootRecTwo(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = '';
                    this.$set(this.data1, data[i].iconclass2, "");
                }
                if (data[i].cam) {
                    this.CloseRootRecThree(data[i].cam,token);
                }
            }
        },
        CloseRootRecThree(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = '';
                    this.$set(this.data1, data[i].iconclass2, "");
                }
                if (data[i].cam) {
                    this.CloseRootRecFour(data[i].cam,token);
                }
            }
        },
        CloseRootRecFour(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = '';
                    this.$set(this.data1, data[i].iconclass2, "");
                }
                if (data[i].cam) {
                    this.CloseRootRecFive(data[i].cam,token);
                }
            }
        },
        CloseRootRecFive(data,token){
            for (var i = 0; i < data.length;i++){
                if (token==data[i].strToken) {
                    data[i].iconclass2 = '';
                    this.$set(this.data1, data[i].iconclass2, "");
                }
            }
        },
        licenseWaterMark() {
			let _this = this;
			var url =_this.$store.state.IPPORT + "/api/v1/GetSystemInfo?session=" + _this.$store.state.token;
            // console.log("------------",url)
			_this.$http.get(url).then(result => {
                if (result.status == 200) {
                    if (result.data.strLicenseType=='Enterprise'||result.data.strLicenseType=='Customization') {     
                        this.$store.state.Certificatetime='false'
                        sessionStorage.Certificatetime = 'false'
                        if(localStorage.getItem('watermarktoggle')==null){
                            if(this.$store.state.Certificatetime=="true"){
                                document.getElementById("watermarktoggle").style.display='block';
                            }else if(this.$store.state.Certificatetime=="false"){
                                document.getElementById("watermarktoggle").style.display='none';
                            }
                        }else{
                            document.getElementById("watermarktoggle").style.display=this.$store.state.watermarktoggle;
                        }
                    }else{
                        this.$store.state.Certificatetime='true'
                        sessionStorage.Certificatetime = 'true'
                        if(localStorage.getItem('watermarktoggle')==null){
                            if(this.$store.state.Certificatetime=="true"){
                                document.getElementById("watermarktoggle").style.display='block';
                            }else if(this.$store.state.Certificatetime=="false"){
                                document.getElementById("watermarktoggle").style.display='none';
                            }
                        }else{
                            document.getElementById("watermarktoggle").style.display=this.$store.state.watermarktoggle;
                        }
                        }
                }
			}).catch(error => {
                console.log("GetSystemInfo");
			});
		},
        Alloffvideo(){
            this.$store.state.offVideo='close'+uuid();
        },
        viewplay(){
            let data = this.cur_query.view;
            let FullScreen = this.cur_query.fullscreen||false;
            if (FullScreen=="true") {
                $('.liveview_left').addClass('liveview_lefts');
                $('.liveview_right').addClass('liveview_rights');
            }
            setTimeout(()=>{
                if(this.viewlists[data]){
                    if (this.viewlists[data].src.length<=0) {
                        this.viewClick(this.viewlists[data]);
                    }
                }else{
                    let data = localStorage.getItem(this.$store.state.user);
                    let srcdata = JSON.parse(data);
                    if (srcdata.src.length<=0) {
                        var viewdata={
                            strName:this.$store.state.user,
                            strLayoutType:'3|3',
                            src:[]
                        }
                        this.viewClick(viewdata);
                    }
                }
            },2000)
        },
        autoplay(){
            let data = localStorage.getItem(this.$store.state.user);
            let srcdata = JSON.parse(data)
            if (srcdata.src.length>0) {
                this.viewClick(srcdata);
            }
        },

        viewlist(){
            var url = this.$store.state.IPPORT + "/api/v1/GetSrc"+"?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.status === 200) {
                    console.log(result.data.src);
                    for (let i = 0; i < result.data.src.length; i++) {
                        this.objlist[result.data.src[i].strToken]=result.data.src[i].strName
                    }
                    this.autoplay();
                    this.ioeventai();
                }
            })

        },
        //编辑视图
        Editview(data){
            // console.log(data,this.$store.state.liveviewadd)
            // return
            var url = this.$store.state.IPPORT + "/api/v1/DelView?token="+data.strToken+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.status === 200) {
                    this.Editadd(data)
                }
            })
        },
        Editadd(data){
            // console.log(this.$store.state.liveviewadd);
            var viewdata={
                strName:data.strName,
                strToken:uuid(4, 16).toLowerCase(),
                strLayoutType:this.rows+'|'+this.cols,
                src:this.$store.state.liveviewadd
            }
            var viewjson=JSON.stringify(viewdata)
            // console.log(viewjson)
            // return
            var url = this.$store.state.IPPORT + "/api/v1/AddView?session="+ this.$store.state.token;
            this.$http.post(url,viewjson,{headers: {'Content-Type': 'application/json'}}).then(result=>{
                if (result.status === 200) {
                    // console.log(result,url)
                    this.Refresh('view')
                    this.$message({
                        message: this.$t("message.camera.Save_successfully"),
                        type: 'warning'
                    });
                }
            })

        },
        //删除视图
        Delview(strToken){
                    this.$message({
                        message: this.$t("message.camera.Delete_successful"),
                        type: 'warning'
                    });
        },
        //播放视图
        async viewClick(data){
            // console.log(data)
            this.$store.state.liveviewadd=[]
            var _this=this
            await this.changePanel(data,'viewClick')
            if(data.src.length==0){
                var vdata={
                    viewparameter:'viewparameter'
                }
                _this.$store.state.liveplay=vdata
                return
            }
            for(var i=0;i<data.src.length;i++){
                if (this.objlist[data.src[i].strToken] == undefined) return
                var label=''
                if(data.src[i].strStream=='main'){
                    // label=data.src[i].strName+"--"+this.$t('message.live.mainstream');
                    label=this.objlist[data.src[i].strToken]+"--"+this.$t('message.live.mainstream')
                }else if(data.src[i].strStream=='sub'){
                    // label=data.src[i].strName+"--"+this.$t('message.live.substream');
                    label=this.objlist[data.src[i].strToken]+"--"+this.$t('message.live.substream')
                }
                console.log(label)
                var vdata={
                    token:data.src[i].strToken,
                    streamprofile:data.src[i].strStream,
                    name:label,
                    label:data.src[i].strName,
                    vid:data.src[i].strIndex,
                    viewparameter:'viewparameter'
                }
                var viewdata={
                    strIndex:data.src[i].strIndex,
                    strToken:data.src[i].strToken,
                    strStream:data.src[i].strStream
                }
                _this.$store.state.liveplay=vdata
                await _this.$store.state.liveviewadd.push(viewdata)
            }
        },
        //保存视图
        viewadd(){
            if(this.viewname==null){
                return
            }
            var viewdata={
                strName:this.viewname,
                strToken:uuid(4, 16).toLowerCase(),
                strLayoutType:this.rows+'|'+this.cols,
                src:this.$store.state.liveviewadd
            }
            console.log(viewdata)
            var viewjson=JSON.stringify(viewdata)
            if(viewdata.strLayoutType=='1|1'){
                viewdata.icon='iconfont icon-tubiao_huaban11'
            }else if(viewdata.strLayoutType=='1|3'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben1'
            }else if(viewdata.strLayoutType=='2|2'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben21'
            }else if(viewdata.strLayoutType=='1|4'){
                viewdata.icon='iconfont icon-sansi'
            }else if(viewdata.strLayoutType=='1|6'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben31'
            }else if(viewdata.strLayoutType=='1|7'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben41'
            }else if(viewdata.strLayoutType=='3|3'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben51'
            }else if(viewdata.strLayoutType=='1|13'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben61'
            }else if(viewdata.strLayoutType=='4|4'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben71'
            }else if(viewdata.strLayoutType=='5|5'){
                viewdata.icon='iconfont icon-tubiao_huaban1fuben8'
            }
            console.log(viewjson)
            // return
            var url = this.$store.state.IPPORT + "/api/v1/AddView?session="+ this.$store.state.token;
            this.$http.post(url,viewjson,{headers: {'Content-Type': 'application/json'}}).then(result=>{
                console.log(result,url);
                if (result.status === 200) {
                    this.viewdata=[];
                    this.srcview()
                    // this.viewdata.push(viewdata)
                }
            })
            this.viewname=null
            
        },
              viewadd_v2(data){
            
            console.log(this.$store.state.liveviewadd);
            var viewdata={
                strName:data,
                strToken:uuid(4, 16).toLowerCase(),
                strLayoutType:this.rows+'|'+this.cols,
                src:this.$store.state.liveviewadd
            }
            var viewjson=JSON.stringify(viewdata);
            if(data == 'view_1') {
            viewdata.strLayoutType=='1|1';
            viewdata.icon='iconfont icon-tubiao_huaban11'
            } else if(data == 'view_2') {
              viewdata.strLayoutType=='1|13';
            viewdata.icon='iconfont icon-tubiao_huaban1fuben61'

            } else if(data == 'view_3') {
              viewdata.strLayoutType=='4|4';
            viewdata.icon='iconfont icon-tubiao_huaban1fuben71'

            } else if(data == 'view_4') {
              viewdata.strLayoutType=='1|6';
            viewdata.icon='iconfont icon-tubiao_huaban1fuben31'

            }  
            this.viewdata.push(viewdata);

        },
        
        
        // 'http://localhost:8080/api/v1/AddView?session=59733292-eda0-42dd-b676-ed71c7a664cf'
        srcview(){
           this.viewadd_v2('view_1');
           this.viewadd_v2('view_2');
           this.viewadd_v2('view_3');
           this.viewadd_v2('view_4');
        },
        getview(arr,arr1){
            if(arr.strLayoutType=='1|1'){
                arr.icon='iconfont icon-tubiao_huaban11'
            }else if(arr.strLayoutType=='1|3'){
                arr.icon='iconfont icon-tubiao_huaban1fuben1'
            }else if(arr.strLayoutType=='2|2'){
                arr.icon='iconfont icon-tubiao_huaban1fuben21'
            }else if(arr.strLayoutType=='1|6'){
                arr.icon='iconfont icon-tubiao_huaban1fuben31'
            }else if(arr.strLayoutType=='1|7'){
                arr.icon='iconfont icon-tubiao_huaban1fuben41'
            }else if(arr.strLayoutType=='3|3'){
                arr.icon='iconfont icon-tubiao_huaban1fuben51'
            }else if(arr.strLayoutType=='1|13'){
                arr.icon='iconfont icon-tubiao_huaban1fuben61'
            }else if(arr.strLayoutType=='4|4'){
                arr.icon='iconfont icon-tubiao_huaban1fuben71'
            }else if(arr.strLayoutType=='5|5'){
                arr.icon='iconfont icon-tubiao_huaban1fuben8'
            }
            for(var i in arr.src){
                for(var j in arr1){
                    if(arr.src[i].strToken == arr1[j].strToken){
                        arr.src[i].strName = arr1[j].strName;
                        // console.log(arr.src[i])
                    }
                }
            }
            // if(arr.src && arr.src.length>0){
            //     for (var i = 0; i < arr.src.length; i++) {
            //         arr.src[i] = getview(arr.src[i],arr1);
            //     }
            // }
            return arr;
        },
        view(){},
        //Refresh
        Refresh(type){
            if(type=='device'){
                this.data=[];
                this.loadDevice();
            }else if(type=='view'){
                this.viewdata=[];
                this.srcview();
            }
        },
        //Watermark
        addWaterMarker(){
			if(!document.getElementById("watermarktoggle")){
				return
			}
            var date=new Date();
            var Y = date.getFullYear() + '-';
            var M = (date.getMonth()+1 < 10 ? '0'+(date.getMonth()+1) : date.getMonth()+1) + '-';
            var D = date.getDate() + ' ';
            var dates=Y+M+D;
            var watermarkstring= this.$store.state.watermarkstring;
            // console.log(watermarkstring);

            var can = document.createElement('canvas');
            var body = document.body;
            body.appendChild(can);
            can.width=300; //Canvas width
            can.height=200;//Height of the canvas
            can.style.display='none';
            var cans = can.getContext('2d');
            cans.rotate(-20*Math.PI/140); //The rotation angle of the text in the canvas
            cans.font = "16px Microsoft JhengHei"; //The font of the text in the canvas
            cans.fillStyle = "rgba(17, 17, 17, 1)";//The color of the text in the canvas
            cans.textAlign = 'left'; //The horizontal position of the text in the canvas
            cans.textBaseline = 'Middle'; //The vertical position of the text in the canvas
            cans.fillText(watermarkstring,can.width/3,can.height/2); //The spacing ratio of the text in the canvas
            document.getElementById("watermarktoggle").style.backgroundImage="url("+can.toDataURL("image/png")+")";
        },
        //Tree node click
        handleNodeClick(data,e){
            $(this.h55id).removeClass('dbclick')
            this.dbclicks=true
            this.$root.bus.$emit('dbclick',true)
            if(data.streamprofile==undefined){
                data.streamprofile='main'
            }
            console.log(data)
            if (data.token&&data.bOnline==true) {
                data['iconclass'] = 'iconfont icon-shexiangji el-tree-camera-play';
                data['iconclass1'] = 'el-tree-camera-play';
                data['iconclass3'] = 'none';
            }
            if (data.strToken&&data.bOnline==true) {
                data['iconclass'] = 'iconfont icon-shexiangji el-tree-camera-play';
                data['iconclass1'] = 'el-tree-camera-play';
                data['iconclass3'] = 'none';
            }
            // var a=data.token.replace(/#/g,"%23")
            let _this =this;
            if(data.disabled_me==false){
                // document.getElementById("icon"+data.token).style.color="#5fbfa7";
                if (data.token) {
                    let vid = 'h' + _this.$data.selectRow + _this.$data.selectCol;
                    console.log(vid);
                    var vdata={
                        token:data.token,
                        streamprofile:data.streamprofile,
                        name:data.name,
                        label:data.label,
                        vid:vid
                    }
                    var viewdata={
                        strIndex:vid,
                        strToken:data.token,
                        strStream:data.streamprofile,
                    }
                    _this.$store.state.liveplay=vdata
                    for (let i = 0; i < _this.$store.state.liveviewadd.length; i++) {
                        if (_this.$store.state.liveviewadd[i].strIndex==vid) {
                            _this.$store.state.liveviewadd.splice(i,1);
                            i--;
                        }
                    }
                    _this.$store.state.liveviewadd.push(viewdata)
                    console.log("----------------------",vdata,_this.$store.state.liveviewadd);
                    // _this.$root.bus.$emit('liveplay', data.token, data.streamprofile, data.name,data.label,vid);
                    this.$root.bus.$emit('deviceStatus',data.token,vid);
                }
                if (data.strToken) {
                    console.log(data.strToken)
                    let vid = 'h' + _this.$data.selectRow + _this.$data.selectCol;
                    console.log(vid);
                    var vdata={
                        token:data.strToken,
                        streamprofile:data.streamprofile,
                        name:data.name,
                        label:data.label,
                        vid:vid
                    }
                    var viewdata={
                        strIndex:vid,
                        strToken:data.strToken,
                        strStream:data.streamprofile,
                        treeid:data.treeid
                    }
                    var treeid={
                        strToken:data.strToken,
                        treeid:data.treeid,
                        firstlevel:e.parent.data.firstLevel
                    }
                    this.treeNodeId.push(treeid)
                    localStorage.setItem('treeid',JSON.stringify(this.treeNodeId));
                    _this.$store.state.liveplay=vdata
                    for (let i = 0; i < _this.$store.state.liveviewadd.length; i++) {
                        if (_this.$store.state.liveviewadd[i].strIndex==vid) {
                            _this.$store.state.liveviewadd.splice(i,1);
                            i--;
                        }
                    }
                    _this.$store.state.liveviewadd.push(viewdata)
                    console.log("----------------------",vdata,_this.$store.state.liveplay);
                    // _this.$root.bus.$emit('liveplay', data.strToken, data.streamprofile, data.name,data.strName,vid);
                    this.$root.bus.$emit('deviceStatus',data.strToken,vid);
                }
                
                _this.$nextTick(()=>{
                    setTimeout(()=>{
                    for(var i=1;i<=this.rows;i++){
                        for(var c=1;c<=this.cols;c++){
                            var video= document.getElementById("hvideo"+i+c)
                            console.log('video.paused++++',video.poster);
                            if(video.poster==""||video.poster=="http://localhost:6080/"||video.poster==_this.$store.state.IPPORT){
                                this.selectCol = c;
                                this.selectRow =i;
                                $(".h5container").removeClass('h5videoh');
                                $("#h"+this.selectRow+this.selectCol).addClass('h5videoh');
                                // console.log('video.paused1',video.poster,i,c);
                                return false
                            }else{
                                console.log('video.paused1',video.poster);
                            }
                        }
                    }
                    },300)
                })
                var viewdata={
                    strName:this.$store.state.user,
                    strLayoutType:this.rows+'|'+this.cols,
                    src:this.$store.state.liveviewadd
                }
                localStorage.setItem(this.$store.state.user,JSON.stringify(viewdata));
                this.ai_h5sid();
            }else{
               console.log("不可用");
            }
        },
        //拖动播放
        dragStart(ev,token,label,streamprofile,name,disabled_me){
            this.drags=false
            // console.log(ev,token,label,streamprofile,name,disabled_me,"124");
            var drag={
                token:token,
                label:label,
                streamprofile:streamprofile,
                name:name,
                disabled_me:disabled_me,
            }
            this.drag=drag;
            ev.dataTransfer.setData("Text",ev.target.id);
        },
        dragover (ev) {
            // console.log(ev,"123",ev.target.id)
            // ev.preventDefalut()
        },
        dropTarget (ev,r,c) {
            if (this.drags) return
            this.drags =true
            let _this =this;
            var data=this.drag;
            if(data.streamprofile==undefined){
                data.streamprofile='main'
            }
            // return false;
            if(data.disabled_me==false){
                // document.getElementById("icon"+data.token).style.color="#5fbfa7";
                if (data.token) {
                    let vid = 'h' + r + c;
                    var vdata={
                        token:data.token,
                        streamprofile:data.streamprofile,
                        name:data.name,
                        label:data.label,
                        vid:vid
                    }
                    var viewdata={
                        strIndex:vid,
                        strToken:data.token,
                        strStream:data.streamprofile
                    }
                    _this.$store.state.liveplay=vdata
                    _this.$store.state.liveviewadd.push(viewdata)
                    console.log("----------------------",vdata,_this.$store.state.liveplay);
                    // _this.$root.bus.$emit('liveplay', data.token, data.streamprofile, data.name,data.label,vid);
                }
                _this.$nextTick(()=>{
                    setTimeout(()=>{
                    for(var i=1;i<=this.rows;i++){
                        for(var c=1;c<=this.cols;c++){
                            var video= document.getElementById("hvideo"+i+c)
                            // console.log('video.paused',video);
                            if(video.poster==""||video.poster=="http://localhost:6080/"||video.poster==_this.$store.state.IPPORT){
                                this.selectCol = c;
                                this.selectRow =i;
                                $(".h5container").removeClass('h5videoh');
                                $("#h"+this.selectRow+this.selectCol).addClass('h5videoh');
                                return false
                            }else{
                                console.log('video.paused1',!video.poster);
                            }
                        }
                    }
                    },300)
                })
                    
            }else{
               console.log("不可用");
            }
        },
		//点击宫格
		videoClick(r, c, $event) {
            this.selectCol = c;
            this.selectRow = r;
            console.log(r, c);
            $(".h5container").removeClass('h5videoh');
            $("#h"+r+c).addClass('h5videoh');
        },
        dbclick(r,c,event){
            this.dbclickr = r
            this.dbclickc = c
            if (!($('#hvideo'+r+c).get(0).paused)){
                if (this.dbclicks) {
                    console.log(r,c,this.cols*this.rows);
                    if (this.cols*this.rows==6) {
                        if (r*c == 2 || r*c == 3) {
                            this.h55id = "#h"+r+c;
                            document.getElementsByClassName('Six_Palace')[c-1].setAttribute('class','palace sixts')
                            this.dbclicks=false;
                            this.$root.bus.$emit('dbclick', r,c);
                        }else{
                        this.h55id = "#h"+r+c;
                        $("#h"+r+c).addClass('dbclick');
                        this.dbclicks=false;
                        this.$root.bus.$emit('dbclick', r,c);
                        }
                    }else if (this.cols*this.rows==13) {
                        if (r*c == 6 || r*c == 8) {
                            this.h55id = "#h"+r+c;
                            document.getElementsByClassName('videoflexitem')[c-1].setAttribute('class','palace sixts')
                            this.dbclicks=false;
                            this.$root.bus.$emit('dbclick', r,c);
                        }else{
                        this.h55id = "#h"+r+c;
                        $("#h"+r+c).addClass('dbclick');
                        this.dbclicks=false;
                        this.$root.bus.$emit('dbclick', r,c);
                        }
                    }else if(this.cols*this.rows==7){
                        this.h55id = "#h"+r+c;
                        document.getElementsByClassName('Seven_Palace')[c-1].setAttribute('class','palace sixts')//Seven_Palace
                        this.dbclicks=false;
                        this.$root.bus.$emit('dbclick', r,c);
                    }else{
                        this.h55id = "#h"+r+c;
                        $("#h"+r+c).addClass('dbclick');
                        this.dbclicks=false;
                        this.$root.bus.$emit('dbclick', r,c);
                    }
                }else{
                    if (this.cols*this.rows==6) {
                        if (r*c == 2 || r*c == 3) {
                            this.h55id = "";
                            $(".sixts").addClass('Six_Palace');
                            $(".sixts").removeClass('sixts');
                            this.dbclicks=true;
                            this.$root.bus.$emit('dbclick', r,c);
                        }else{
                            this.h55id = "";
                            $("#h"+r+c).removeClass('dbclick');
                            this.dbclicks=true;
                            this.$root.bus.$emit('dbclick', r,c);
                        }
                    }else if (this.cols*this.rows==13) {
                        if (r*c == 6 || r*c == 8) {
                            this.h55id = "";
                            $(".sixts").addClass('videoflexitem');
                            $(".sixts").removeClass('sixts');
                            this.dbclicks=true;
                            this.$root.bus.$emit('dbclick', r,c);
                        }else{
                            this.h55id = "";
                            $("#h"+r+c).removeClass('dbclick');
                            this.dbclicks=true;
                            this.$root.bus.$emit('dbclick', r,c);
                        }
                    }else if(this.cols*this.rows==7){
                        this.h55id = "";
                        $(".sixts").addClass('Seven_Palace');
                        $(".sixts").removeClass('sixts');
                        this.dbclicks=true;
                        this.$root.bus.$emit('dbclick', r,c);
                    }else{
                        this.h55id = "";
                        $("#h"+r+c).removeClass('dbclick');
                        this.dbclicks=true;
                        this.$root.bus.$emit('dbclick', r,c);
                    }
                }
            };
            
        },
        stopVideo(event){
            return;
        },
		// ui宽度
		updateUI(x) {
            if($(document.body).width() < 768)
            {
                this.contentHeight = $(document.body).height()*0.4;
            }else
            {
                this.contentHeight = $(document.body).height()*x;
            }
            $('div[name="flex"]').height(this.contentHeight / this.rows);
            //this.contentHeight = $(document.body).height()*0.8;
            let _this = this;
            // if (H5siOS() === true)
            // {
            //     $('.h5video').prop("controls", true);
            // }
        },
		//点击宫格
        changePanel(event,viewClick) {
            $(this.h55id).removeClass('dbclick');
            this.dbclicks=true;
            this.h55id = "";
            this.$root.bus.$emit('dbclick',true)
            console.log(viewClick,event)
            var data=''
            if(viewClick=='viewClick'){
                data=event.strLayoutType
            }else{
                data = $(event.target).data('row');
            }
            // return
            let _this = this;
            let cols = data.split('|')[1];
            let rows = data.split('|')[0];
            //this.map.clear();
            Object.assign(this.$data, {
                rows: parseInt(rows),
                cols: parseInt(cols)
            });
            console.log(this.rows,this.cols);
            this.$root.bus.$emit('strLayoutTypes',this.rows+'|'+this.cols);
            Vue.nextTick(function () {
                if(data=='1|6'||data=='1|7'||data=='1|13'){
                    // console.log("121");
                    
                }else{
                    // console.log("45845454545");
                    $("#videoPanel .videoColor .palace").removeClass("Seven_Palace");
                    $("#videoPanel .videoColor .palace").removeClass("Six_Palace");
                    $("#videoPanel .videoColor .palace").removeClass("videoflexitem");
                    $("#videoPanel .videoColor .palace").removeClass("fours_Palace");
                }
                var list_gong=$(".palace");
                if(data=='1|4'){
                    // console.log("23")
                    list_gong.removeClass("videoflexitem");
                    list_gong.removeClass("Seven_Palace");
                    list_gong.removeClass("Six_Palace");

                    list_gong.addClass("fours_Palace");
                }
                if(data=='1|6'){
                    // console.log("23")
                    list_gong.removeClass("videoflexitem");
                    list_gong.removeClass("Seven_Palace");
                    list_gong.removeClass("fours_Palace");

                    list_gong.addClass("Six_Palace");
                }
                if(data=='1|7'){
                    // console.log("234")
                    list_gong.removeClass("videoflexitem");
                    list_gong.removeClass("Six_Palace");
                    list_gong.removeClass("fours_Palace");

                    list_gong.addClass("Seven_Palace");
                }
                if(data=='1|13'){
                    // console.log("2345")
                    list_gong.removeClass("Six_Palace");
                    list_gong.removeClass("Seven_Palace");
                    list_gong.removeClass("fours_Palace");

                    list_gong.addClass("videoflexitem");
                }
                //$('div[name="flex"]').height(($(".content").height() - 50) / rows);
                $('div[name="flex"]').height(_this.contentHeight / rows);
                var cors=_this.cols*_this.rows;
                console.log(cors)
                if(cors>9){
                    $('.ptz_id_show').hide()
                    $('.info_id_show').hide()
                    $('.ptzcontrols').hide()
                    $('.information').hide()
                    $('.liveplay_ptz').hide()
                    $('.open_button').hide();
                    $('.open_liveplay_butt').hide();
                }
                if(cors<=9){
                    $('.ptz_id_show').show()
                    $('.info_id_show').show()
                    $('.open_button').show();
                    // $('.open_liveplay_butt').show();
                }
            })
        },
        //全屏
        panelFullScreen(event) {
            if (this.dbclickr!='') {
                this.dbclick(this.dbclickr,this.dbclickc);
                this.dbclickr = '';
                this.dbclickc = '';
            }
            var elem = document.getElementById('video_hed');
            //var elem = $("#videoPanel");
            console.log('panelFullScreen', event);
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
                    console.log("========  updateUIExitFullScreen");
                    this.updateUIExitFullScreen();
                } else {
                     console.log('panelFullScreen3');

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

            $('div[name="flex"]').height(screen.height / this.rows);
        },
        updateUIExitFullScreen(){
            if (!document.fullscreenElement && !document.webkitIsFullScreen && !document.mozFullScreen && !document.msFullscreenElement)
            {
                $('div[name="flex"]').height(this.contentHeight / this.rows);
            }
        },
        //The best way to write
        headswitch(){
            $('#headswitch').show()
            $('#headswitch1').hide()
        },
        headswitch1(){
            $('#headswitch1').show()
            $('#headswitch').hide()
        },
        //Homework
        loadtest(){
            var root = this.$store.state.IPPORT;
            if(this.$store.state.root=="Operator"){
                return false
            }
            this.$store.state.Recviewadd=[];
            var url = root + "/api/v1//GetSrcCamera?session="+ this.$store.state.token;
            // return falsel;
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    var data =  result.data;
                    var srcGroup = {children: []};
                    srcGroup.label=this.$t('message.live.camera');
                    srcGroup.iconclass="iconfont  icon-shexiangji";
                    srcGroup.length=data.src.length;
                    var number = 0;
                    var numberbDisable = 0;
                    var offlineNumber = 0;
                    var disableNumber = 0;
                    var IdleNumber =0;
                    for(var i=0; i< data.src.length; i++){
                        var item = data.src[i];
                        if (item.bOnline&&!item.bDisable) {
                            number=number+1;
                        }
                        if(item['nOriginalType'] == 'H5_CH_GB'){
                            numberbDisable=numberbDisable+1;
                            continue;
                        }else{
                            // 主副流
                            var node=[{
                            token : item['strToken'],
                            streamprofile : "main",
                            label :this.$t('message.live.mainstream'),
                            name:item['strName']+"--"+this.$t('message.live.mainstream'),
                            iconclass : 'mdi mdi-playlist-play fa-fw',
                            disabled_me:false
                            },{
                            token : item['strToken'],
                            streamprofile : "sub",
                            label :this.$t('message.live.substream'),
                            name:item['strName']+"--"+this.$t('message.live.substream'),
                            iconclass : 'mdi mdi-playlist-play fa-fw',
                            disabled_me:false
                            }]
                            var newItem ={
                                token : item['strToken'],
                                label : item['strName'],
                                iconclass : 'iconfont  icon-shexiangji',
                                iconclass1 : '',
                                iconclass2 : 'iconfont  icon-shexiangtou',
                                name:item['strName']+"--"+this.$t('message.live.mainstream'),
                                children:node,
                                disabled_me:false,
                                statusID:item['strToken']};
                            if(item['bOnline']==true){
                                // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                                newItem['bOnline'] = true;
                            }else if(item['bOnline']==false){
                                newItem['bOnline'] = false;
                            }
                            if(!item['bOnline']){
                                // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                                newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                                newItem['iconclass1'] = 'el-tree-camera';
                                offlineNumber=offlineNumber+1;
                            }
                            if(item['nType'] == 'H5_CLOUD')
                                newItem['iconclass'] = 'mdi mdi-cloud-upload fa-fw';
                            
                            if(item['bRec'] == true){
                                // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                                newItem['iconclass2'] = 'none';
                                this.$store.state.Recviewadd.push(item['strToken']);
                                newItem['bRec'] = true;
                            }else if(item['bRec']==false){
                                newItem['bRec'] = false;
                            }
                            
                            if(item['bDisable'] == true&&this.$store.state.devicemarktoggle=='none'){
                                numberbDisable = numberbDisable+1;
                                continue;
                            }
                            if(item['bDisable'] == true){
                                if (this.$store.state.devicemarktoggle==undefined||this.$store.state.devicemarktoggle=='block') {
                                    disableNumber=disableNumber+1;
                                }
                            }
                            if (item['bDisable'] == true) {
                                newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                                newItem['iconclass1'] = 'el-tree-camera';
                            }
                            if (item['bIdle'] == true) {
                                IdleNumber=IdleNumber+1;
                                newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                                newItem['iconclass1'] = 'el-tree-camera';
                            }
                            if (localStorage.getItem(this.$store.state.user)) {
                                let localToken = JSON.parse(localStorage.getItem(this.$store.state.user))['src'];
                                for (let i = 0; i < localToken.length; i++) {
                                    const playtoken = localToken[i]['strToken'];
                                    if (playtoken==item['strToken']&&item['bOnline']==true) {
                                        newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera-play';
                                        newItem['iconclass1'] = 'el-tree-camera-play';
                                        newItem['iconclass3'] = 'none';
                                    }
                                }
                            }
                            if(!item['bOnline']){
                                srcGroup.children.push(newItem);
                            }else{
                                srcGroup.children.unshift(newItem);
                            }
                        }
                    }
                    srcGroup.firstLevel=true,
                    srcGroup.length=data.src.length-numberbDisable;
                    srcGroup.online=number;
                    // srcGroup.offline=srcGroup.length-number;
                    srcGroup.offline=offlineNumber;
                    srcGroup.disable=disableNumber;
                    srcGroup.idle = IdleNumber;
                    this.data.push(srcGroup);
                    console.log(this.data);
                } 
            })

        },
        //写作业
        loadDevice() {
            var children1 =[{
                 children:'',
                token:uuid(4, 16).toLowerCase(),
                label:'Charles de Gaulle Airport',
                iconclass:'iconfont icon-kaiqishexiangtou'
            },
            {
                children:'',
                token:uuid(4, 16).toLowerCase(),
                label:'Orly Airport',
                iconclass:'iconfont icon-kaiqishexiangtou'
            },
             {
                children:'',
                token:uuid(4, 16).toLowerCase(),
                label:'Villejuif',
                iconclass:'iconfont icon-kaiqishexiangtou'
            },
            {
                children:'',
                token:uuid(4, 16).toLowerCase(),
                label:'Nanterre',
                iconclass:'iconfont icon-kaiqishexiangtou'
            },
            {
                children:'',
                token:uuid(4, 16).toLowerCase(),
                label:'Defense',
                iconclass:'iconfont icon-kaiqishexiangtou'
            }];
 
        },
        
        
        
        loadSrc(strToken) {
            var root = this.$store.state.IPPORT;
            let _this =this;
            
            var url = root + "/api/v1/GetDeviceSrc?token="+ strToken + "&session=" + this.$store.state.token;

            this.$http.get(url).then(result => {
                console.log(result);
                if (result.status == 200)
                {
                    var number = 0;
                    var numberbDisable = 0;
                    var offlineNumber = 0;
                    var disableNumber = 0;
                    var IdleNumber =0;
                    var data =  result.data;
                    var srcGroup = [];
                    for(var i=0; i< data.src.length; i++){
                        var item = data.src[i];
                        if (item.bOnline&&!item.bDisable) {
                            number=number+1;
                        }

                        // 主副流
                        var node=[{
                        token : item['strToken'],
                        streamprofile : "main",
                        label :this.$t('message.live.mainstream'),
                        name:item['strName']+"--"+this.$t('message.live.mainstream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        },{
                        token : item['strToken'],
                        streamprofile : "sub",
                        label :this.$t('message.live.substream'),
                        name:item['strName']+"--"+this.$t('message.live.substream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        }]
                        for(var l=0; l< node.length; l++){
                            if(item['bDisable'] == true){
                                node[l].disabled_me =true;
                            }
                        }
                        
                        var newItem ={
                                token : item['strToken'],
                                label : item['strName'],
                                iconclass : 'iconfont  icon-shexiangji',
                                iconclass1 : '',
                                name:item['strName']+"--"+this.$t('message.live.mainstream'),
                                children:node,
                                disabled_me:false};
                        if(item['bOnline']==true){
                            // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                            newItem['bOnline'] = true;
                        }else if(item['bOnline']==false){
                            newItem['bOnline'] = false;
                        }
                        if(!item['bOnline']){
                            // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                            newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                        }
                        if(!item['bOnline']&&item['bIdle'] == false){ 
                            offlineNumber=offlineNumber+1;
                        }
                        if(item['nType'] == 'H5_CLOUD')
                            newItem['iconclass'] = 'mdi mdi-cloud-upload fa-fw';

                        if(item['bRec'] == true){
                            // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                            newItem['iconclass2'] = 'none';
                            this.$store.state.Recviewadd.push(item['strToken']);
                            newItem['bRec'] = true;
                        }else if(item['bRec']==false){
                            newItem['bRec'] = false;
                        }

                        if(item['bDisable'] == true){
                            newItem['disabled_me'] =true;
                            // newItem['iconclass1'] = 'camera';
                            newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                        }
                        if(item['bDisable'] == true){
                            if (this.$store.state.devicemarktoggle==undefined||this.$store.state.devicemarktoggle=='block') {
                                disableNumber=disableNumber+1;
                            }
                        }
                        if(item['bDisable'] == true&&this.$store.state.devicemarktoggle=='none'){
                            numberbDisable = numberbDisable+1;
                            continue;
                        }
                        if (item['bIdle'] == true) {
                            newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                            IdleNumber=IdleNumber+1;
                        }
                        if (localStorage.getItem(this.$store.state.user)) {
                            let localToken = JSON.parse(localStorage.getItem(this.$store.state.user))['src'];
                            for (let i = 0; i < localToken.length; i++) {
                                const playtoken = localToken[i]['strToken'];
                                if (playtoken==item['strToken']&&item['bOnline']==true) {
                                    newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera-play';
                                    newItem['iconclass1'] = 'el-tree-camera-play';
                                    newItem['iconclass3'] = 'none';
                                }
                            }
                        }
                        // srcGroup.push(newItem);
                        if(!item['bOnline']||item.bDisable){
                            srcGroup.push(newItem);
                        }else{
                            srcGroup.unshift(newItem);
                        }
                    }
                    var srcData = this.data.find(item => item.token === strToken);
                    if(srcData){
                        srcData.children=srcGroup;
                        srcData.length=result.data.src.length-numberbDisable;
                        srcData.online=number;
                        // srcData.offline=srcData.length-srcData.online;
                        srcData.offline=offlineNumber;
                        srcData.disable=disableNumber;
                        srcData.idle = IdleNumber;
                    }
                }
            }).catch(error => {
                console.log('GetSrc failed', error);
            });
        },
        //数字仓机
        NumberDevice() {
            var root = this.$store.state.IPPORT;
            if(this.$store.state.root=="Operator"){
                return false
            }
            //url
            var url = root + "/api/v1/GetGbDevice?session="+ this.$store.state.token;

            //重组
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    var data=result.data.dev;
                    // console.log(data)
                    if(Array.isArray(data)){
                        data.sort((a,b)=>{
                            if(a.strName===b.strName) return 0;

                            return a.strName > b.strName? 1:-1 
                        })
                        for(var i=0; i<data.length; i++){
                            var item=data[i];

                            this.data.push({
                                token:item.strToken,
                                label:item.strName,
                                iconclass:"iconfont icon-shexiangji",
                                children:[],
                                firstLevel:true,
                                length:'',
                                online:0,
                                offline:0,
                                statusID:data[i].strToken
                            })
                            this.NumberSrc(item.strToken)
                        }
                        // console.log(this.data)
                    }
                }
            })
        },
        NumberSrc(strToken) {
            var root = this.$store.state.IPPORT;
            let _this =this;
            this.$store.state.Recviewadd=[];
            var url = root + "/api/v1/GetGbDeviceSrc?token="+ strToken + "&session=" + this.$store.state.token;

            this.$http.get(url).then(result => {
                if (result.status == 200)
                {   
                    var number = 0;
                    var numberbDisable = 0;
                    var offlineNumber = 0;
                    var disableNumber = 0;
                    var IdleNumber =0;
                    var data =  result.data;
                    var srcGroup = [];
                    for(var i=0; i< data.src.length; i++){
                        var item = data.src[i];
                        if (item.bOnline&&!item.bDisable) {
                            number=number+1;
                        }
                        // 主副流
                        var node=[{
                        token : item['strToken'],
                        streamprofile : "main",
                        label :this.$t('message.live.mainstream'),
                        name:item['strName']+"--"+this.$t('message.live.mainstream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        },{
                        token : item['strToken'],
                        streamprofile : "sub",
                        label :this.$t('message.live.substream'),
                        name:item['strName']+"--"+this.$t('message.live.substream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        }]
                        var newItem ={
                                token : item['strToken'],
                                label : item['strName'],
                                iconclass : 'iconfont  icon-shexiangji',
                                iconclass1:'',
                                name:item['strName']+"--"+this.$t('message.live.mainstream'),
                                children:node,
                                disabled_me:false};
                        if(item['bOnline']==true){
                            // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                            newItem['bOnline'] = true;
                        }else if(item['bOnline']==false){
                            newItem['bOnline'] = false;
                        }
                        if(!item['bOnline']){
                            //  newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                            newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                        }
                        if(!item['bOnline']&&item['bIdle'] == false){ 
                            offlineNumber=offlineNumber+1;
                        }
                        if(item['nType'] == 'H5_CLOUD')
                            newItem['iconclass'] = 'mdi mdi-cloud-upload fa-fw';

                        if(item['bRec'] == true){
                            // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                            newItem['iconclass2'] = 'none';
                            this.$store.state.Recviewadd.push(item['strToken']);
                            newItem['bRec'] = true;
                        }else if(item['bRec']==false){
                            newItem['bRec'] = false;
                        }
                        if (item['bDisable'] == true) {
                            newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                        }
                        if(item['bDisable'] == true){
                            if (this.$store.state.devicemarktoggle==undefined||this.$store.state.devicemarktoggle=='block') {
                                disableNumber=disableNumber+1;
                            }
                        }
                        
                        if(item['bDisable'] == true&&this.$store.state.devicemarktoggle=='none'){
                            numberbDisable = numberbDisable+1;
                            continue;
                        }
                        if (item['bIdle'] == true) {
                            newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                            IdleNumber=IdleNumber+1;
                        }
                        if (localStorage.getItem(this.$store.state.user)) {
                            let localToken = JSON.parse(localStorage.getItem(this.$store.state.user))['src'];
                            for (let i = 0; i < localToken.length; i++) {
                                const playtoken = localToken[i]['strToken'];
                                if (playtoken==item['strToken']&&item['bOnline']==true) {
                                    newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera-play';
                                    newItem['iconclass1'] = 'el-tree-camera-play';
                                    newItem['iconclass3'] = 'none';
                                }
                            }
                        }
                    srcGroup.push(newItem);
                    }
                    var srcData = this.data.find(item => item.token === strToken)
                    // console.log(srcData)
                    if(srcData){
                        srcData.children=srcGroup
                        srcData.length=result.data.src.length-numberbDisable;
                        srcData.online=number;
                        // srcData.offline=srcData.length-srcData.online;
                        srcData.offline=offlineNumber;
                        srcData.disable=disableNumber;
                        srcData.idle = IdleNumber;
                    }
                }
            }).catch(error => {
                console.log('GetSrc failed', error);
            });
        },
        //级联
        cloudDevice() {
            var root = this.$store.state.IPPORT;
            if(this.$store.state.root=="Operator"){
                return false
            }
            var url = root + "/api/v1/GetCloudDevice?session="+ this.$store.state.token;

            //重组
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    var data=result.data.dev;
                    // console.log(data)
                    if(Array.isArray(data)){
                        data.sort((a,b)=>{
                            if(a.strName===b.strName) return 0;

                            return a.strName > b.strName? 1:-1 
                        })
                        for(var i=0; i<data.length; i++){
                            var item=data[i];

                            this.data.push({
                                token:item.strToken,
                                label:item.strName,
                                iconclass:"iconfont icon-shexiangji",
                                children:[],
                                firstLevel:true,
                                length:'',
                                online:0,
                                offline:0,
                                statusID:data[i].strToken
                            })
                            this.cloudSrc(item.strToken)
                        }
                        // console.log(this.data)
                    }
                }
            })
        },
        cloudSrc(strToken) {
            var root = this.$store.state.IPPORT;
            this.$store.state.Recviewadd=[];
            var url = root + "/api/v1/GetCloudDeviceSrc?token="+ strToken + "&session=" + this.$store.state.token;

            this.$http.get(url).then(result => {
                if (result.status == 200)
                {   
                    var number = 0;
                    var numberbDisable = 0;
                    var offlineNumber = 0;
                    var disableNumber = 0;
                    var IdleNumber =0;
                    var data =  result.data;
                    var srcGroup = [];
                    for(var i=0; i< data.src.length; i++){
                        var item = data.src[i];
                        if (item.bOnline&&!item.bDisable) {
                            number=number+1;
                        }
                        // 主副流
                        var node=[{
                        token : item['strToken'],
                        streamprofile : "main",
                        label :this.$t('message.live.mainstream'),
                        name:item['strName']+"--"+this.$t('message.live.mainstream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        },{
                        token : item['strToken'],
                        streamprofile : "sub",
                        label :this.$t('message.live.substream'),
                        name:item['strName']+"--"+this.$t('message.live.substream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        }]
                        var newItem ={
                                token : item['strToken'],
                                label : item['strName'],
                                iconclass : 'iconfont  icon-shexiangji',
                                iconclass1:'',
                                name:item['strName']+"--"+this.$t('message.live.mainstream'),
                                children:node,
                                disabled_me:false};
                        if(item['bOnline']==true){
                            // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                            newItem['bOnline'] = true;
                        }else if(item['bOnline']==false){
                            newItem['bOnline'] = false;
                        }
                        if(!item['bOnline']){
                            // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                            newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                        }
                        if(!item['bOnline']&&item['bIdle'] == false){ 
                            offlineNumber=offlineNumber+1;
                        }

                        // if(item['nType'] == 'H5_CLOUD')
                        //     newItem['iconclass'] = 'iconfont  icon-shexiangji';

                        if(item['bRec'] == true){
                            // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                            newItem['iconclass2'] = 'none';
                            this.$store.state.Recviewadd.push(item['strToken']);
                            newItem['bRec'] = true;
                        }else if(item['bRec']==false){
                            newItem['bRec'] = false;
                        }
                         if (item['bDisable'] == true) {
                            newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                        }
                        if(item['bDisable'] == true){
                            if (this.$store.state.devicemarktoggle==undefined||this.$store.state.devicemarktoggle=='block') {
                                disableNumber=disableNumber+1;
                            }
                        }
                        
                        if(item['bDisable'] == true&&this.$store.state.devicemarktoggle=='none'){
                            numberbDisable = numberbDisable+1;
                            continue;
                        }
                        if (item['bIdle'] == true) {
                            newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                            newItem['iconclass1'] = 'el-tree-camera';
                            IdleNumber=IdleNumber+1;
                        }
                        if (localStorage.getItem(this.$store.state.user)) {
                            let localToken = JSON.parse(localStorage.getItem(this.$store.state.user))['src'];
                            for (let i = 0; i < localToken.length; i++) {
                                const playtoken = localToken[i]['strToken'];
                                if (playtoken==item['strToken']&&item['bOnline']==true) {
                                    newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera-play';
                                    newItem['iconclass1'] = 'el-tree-camera-play';
                                    newItem['iconclass3'] = 'none';
                                }
                            }
                        }

                    srcGroup.push(newItem);
                    }
                    var srcData = this.data.find(item => item.token === strToken)
                    // console.log(srcData)
                    if(srcData){
                        srcData.children=srcGroup
                        srcData.length=result.data.src.length-numberbDisable;
                        srcData.online=number;
                        // srcData.offline=srcData.length-srcData.online;
                        srcData.offline=offlineNumber;
                        srcData.disable=disableNumber;
                        srcData.idle = IdleNumber;
                    }
                }
            }).catch(error => {
                console.log('GetSrc failed', error);
            });
        },

        Regional(){
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v1/GetRegion?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                var oldarr=result.data.root;
                var oldarr1=result.data.src;
                var dataroot=this.getchild(oldarr,oldarr1);
                if (localStorage.getItem(this.$store.state.user)) {
                    let localToken = JSON.parse(localStorage.getItem(this.$store.state.user))['src'];
                    for(var k in dataroot.cam){
                        for (let i = 0; i < localToken.length; i++) {
                            const playtoken = localToken[i]['strToken'];
                            if (playtoken==dataroot.cam[k].strToken&&dataroot.cam[k].bOnline==true) {
                                dataroot.cam[k].iconclass = 'iconfont icon-shexiangji el-tree-camera-play';
                                dataroot.cam[k].iconclass1 = 'el-tree-camera-play';
                                dataroot.cam[k].iconclass3 = 'none';
                            }
                        }
                        
                    }
                }
                this.datapush(dataroot)
            })
        },
        datapush(data){
            data['firstLevel'] =data.strToken;
            this.a(data)
        },
        a(data){
            console.log(data);
            if(data.node.length!=0){
                for (let i = 0; i < data.node.length; i++) {
                    data.node[i]['firstLevel'] =data.node[i].strToken;
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
                    data.node[i]['firstLevel'] =data.node[i].strToken;
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
                    data.node[i]['firstLevel'] =data.node[i].strToken;
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
                    data.node[i]['firstLevel'] =data.node[i].strToken;
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
                    if (data.node[i].strName!='设备暂时不可用') {
                        data.node[i]['firstLevel'] =data.node[i].strToken;
                        data.cam.push(data.node[i])
                    }
                }
            }
            delete data.node;
        },
        getchild(arr,arr1) {
            this.$store.state.Recviewadd=[];
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
                            arr.cam[i].treeid= arr1[j].strToken+uuid();
                            arr.cam[i].iconclass2='';
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

                            if(arr1[j].bRec == true){
                                // arr.cam[i].iconclass2  = 'iconfont icon-radioboxfill none';
                                arr.cam[i].iconclass2= 'none';
                                this.$store.state.Recviewadd.push(arr1[j].strToken);
                            } 

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
            if (arr.cam.length>0) {
                if (localStorage.getItem(this.$store.state.user)) {
                    let localToken = JSON.parse(localStorage.getItem(this.$store.state.user))['src'];
                    for(var k in arr.cam){
                        for (let i = 0; i < localToken.length; i++) {
                            const playtoken = localToken[i]['strToken'];
                            if (playtoken==arr.cam[k].strToken&&arr.cam[k].bOnline==true) {
                                arr.cam[k].iconclass = 'iconfont icon-shexiangji el-tree-camera-play';
                                arr.cam[k].iconclass1 = 'el-tree-camera-play';
                                arr.cam[k].iconclass3 = 'none';
                            }
                        }
                    }
                }
            }
            return arr;
        },
        //模糊查询
        filterNode(value, data, node) {
            // 如果什么都没填就直接返回
            if (!value) return true;
            // 如果传入的value和data中的label相同说明是匹配到了
            if (data.label.indexOf(value) !== -1) {
            return true;
            }
            // 否则要去判断它是不是选中节点的子节点
            return this.checkBelongToChooseNode(value, data, node);
        },
        filterNode1(value, data, node) {
            // 如果什么都没填就直接返回
            if (!value) return true;
            // 如果传入的value和data中的label相同说明是匹配到了
            if (data.strName.indexOf(value) !== -1) {
            return true;
            }
            // 否则要去判断它是不是选中节点的子节点
            // return this.checkBelongToChooseNode(value, data, node);
        },
        // 判断传入的节点是不是选中节点的子节点
        checkBelongToChooseNode(value, data, node) {
            const level = node.level;
            // 如果传入的节点本身就是一级节点就不用校验了
            if (level === 1) {
            return false;
            }
            // 先取当前节点的父节点
            let parentData = node.parent;
            // 遍历当前节点的父节点
            let index = 0;
            while (index < level - 1) {
            // 如果匹配到直接返回
            if (parentData.data.label.indexOf(value) !== -1) {
                return true;
            }
            // 否则的话再往上一层做匹配
            parentData = parentData.parent;
            index ++;
            }
            // 没匹配到返回false
            return false;
        },
    },
    computed: {
        contractFile() {
            return this.$store.state.videoid
        },  
        watchview(){
            return this.$route.query.view
        },
        fullscreen(){
            return this.$route.query.fullscreen||false;
        
        }
    },
    beforeDestroy() {
        this.handleClose();
        // this.$root.bus.$off('liveplay');
        this.ai_websocket.keepaliveTimer({types:'unsubscribe',uuid:this.uuid});
    },
    //模糊查询
    watch: {
        fullscreen(data){
            if (this.$route.query.embed=='true') {
                if (data=="false") {
                    $('.liveview_left').removeClass('liveview_lefts');
                    $('.liveview_right').removeClass('liveview_rights');
                    $('.liveview_right2').show();
                     $('#menu').show();
                    this.updateUI(0.89);
                }else if (data=='true') {
                    $('.liveview_left').addClass('liveview_lefts');
                    $('.liveview_right').addClass('liveview_rights');
                    $('.liveview_right2').hide();
                    $('#menu').hide();
                    this.updateUI(0.98);
                }
            }else if(this.$route.query.embed=='false'){
                    this.updateUI(0.89);
            }else{
                if (data=='true') {
                    $('.liveview_left').addClass('liveview_lefts');
                    $('.liveview_right').addClass('liveview_rights');
                    $('.liveview_right2').hide();
                    $('#menu').hide();
                    this.updateUI(0.89)   
                }else{
                    $('.liveview_left').removeClass('liveview_lefts');
                    $('.liveview_right').removeClass('liveview_rights');
                    $('.liveview_right2').show();
                    $('#menu').show();
                    this.updateUI(0.82)  
                }
            }
        },
        watchview(data){
            if(this.viewlists[data]){
                this.viewClick(this.viewlists[data]);
            }
        },
        contractFile(value){
            var viewdata={
                strName:this.$store.state.user,
                strLayoutType:this.rows+'|'+this.cols,
                src:this.$store.state.liveviewadd
            }
            localStorage.setItem(this.$store.state.user,JSON.stringify(viewdata));
            this.ai_h5sid(viewdata);
        },
        filterText(val) {
            if (this.$store.state.root=="Operator") {
                this.$refs.tree1.filter(val);
            }else{
                this.$refs.tree.filter(val);
                this.$refs.tree1.filter(val);
            }
        }
    }
    
}
</script>

<style lang="scss">
.liveview{
    .icon_start{
        width: 32px;
        height: 32px;
        background: url('~@/views/replay/imgs/Playback_start.png') no-repeat center;
        background-size: 100%;
    }
    .icon_stop{
        width: 32px;
        height: 32px;
        background: url('~@/views/replay/imgs/Playback_stop.png') no-repeat center;
        background-size: 100%;
    }
    .dasboard_modal{
        .el-select.el-select--mini{
            .el-input__inner{
                background: none !important;
            }
        }
        .playback_function{
            width: 100%;
            display: flex;
            // padding: 0 20%;
            justify-content: flex-start;
            margin-top: 16px;
            .back_Choice{
                z-index: 1111;
                margin-right: 80px;
                width: 120px;
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
                    font-size: 20px;margin-left:10px;
                }
            }
        }
        .videoo{
            width: 100%;
            height: 356px;
            object-fit: fill;
        }
    }
    #menu{
        width: 20px;
        height: 40px;
        background: #181818;
        line-height: 40px;
        text-align: center;
        position: absolute;
        color:#fff;
        right: 0%;
        top: 49%;
        z-index: 101;
    }
    #menu:hover{
        cursor:pointer
    }
    .ai_replay:hover{
        cursor:pointer
    }
    .el-tree-node{
        overflow: auto;
    }
    padding-top: 10px;
    display: flex;
    // flex-wrap: wrap;
    justify-content: space-between;
    /* 水印 */
    #watermarktoggle{
        position: fixed;
        z-index: 100;
        width: 100%;
        height: 100%;
        left: 0;
        top: 0;
        display: none;
        pointer-events: none;
        margin-top: 40px;
    }
    .liveview_left{
        width: 16%;
        // min-width: 290px;
        height: 90vh;
        margin: 0 5px;
        overflow: auto;
        &::-webkit-scrollbar{
            display: none;
        }
        .liveview_left_input{
            margin: 10px 0;
        }
        // #headswitch1{
        //     display: none;
        // }
        //录像管理
        .black{
            display: none;font-size: 12px;color: #606266; padding-left: 4px;line-height: 24px;color: #f00;
        }
        .nowplay{
            display: none;font-size: 12px;color: #606266; padding-left: 4px;line-height: 23.5px;color: #30d158;
            .dot{
                font-size: 12px;line-height: 23.5px;padding-right: 4px;
            }
            .nowplayText{
                line-height: 26px;
            }
        }
        // .none{
        //     display: block;
        // }
        
        .el_tree .el-tree-node__content{
            min-height: 24px;
            height: auto;
        }
        .el-collapse{
            .el-collapse-item{
                height: 40vh;
                .el-collapse-item__wrap{
                    .el-collapse-item__content{
                        height: 35vh;
                        padding: 0 10px;
                        overflow: auto;
                        // ::-webkit-scrollbar{
                        //     display: none;
                        // }
                    }
                }
            }
        }
    }
    .StatusPrompt {
        display:none;
    }
    .StatusPromptColor{
        color:#67cfb5 !important;
    }
    .liveview_lefts{
        width: 0%;
    }
    .liveview_right{
        width: 84%;
        height: 90vh;
        position: relative;
        .dbclick{
            position: absolute;
            top: 1px;
            left: 1px;
            width: 83.45vw;
            height: 82.5vh;
            z-index: 102 !important;
        }
        /* 播放片段 */
        .h5videoh{
            /* border: 2px solid #f44336 !important;
            box-sizing: border-box; */
            box-shadow: 0 0 0 2px #f44336;
            // box-sizing: border-box;
            z-index: 101;
        }
        .h5videoh1{
            /* border: 2px solid #f44336 !important;
            box-sizing: border-box; */
            box-shadow: 0 0 0 2px #fff100;
            // box-sizing: border-box;
            z-index: 101;
            /* IE10、Firefox and Opera，IE9以及更早的版本不支持 */
            animation-name: breath;       /* 动画名称 */
            animation-duration: 1.5s;       /* 动画时长3秒 */
            animation-timing-function: ease-in-out;   /* 动画速度曲线：以低速开始和结束 */
            animation-iteration-count: infinite;   /* 播放次数：无限 */
            /* Safari and Chrome */
            -webkit-animation-name: breath;     /* 动画名称 */
            -webkit-animation-duration: 1.5s;     /* 动画时长3秒 */
            -webkit-animation-timing-function: ease-in-out; /* 动画速度曲线：以低速开始和结束 */
            -webkit-animation-iteration-count: infinite; /* 播放次数：无限 */
        }
        @keyframes breath {
            from { box-shadow: 0 0 0 2px #fff100;}       /* 动画开始时的不透明度 */
            50% { box-shadow:none;}    /*   动画50% 时的不透明度 */
            to  { box-shadow: 0 0 0 2px #fff100;}       /* 动画结束时的不透明度 */ 
        }
        @-webkit-keyframes breath {
            from { box-shadow: 0 0 0 2px #fff100;}       /* 动画开始时的不透明度 */
            50% { box-shadow:none;}    /*    动画50% 时的不透明度 */
            to  { box-shadow: 0 0 0 2px #fff100;}       /* 动画结束时的不透明度 */ 
        }
        .h5videohs{
            border: 1px solid #409EFF !important;
        }
        .videoColor {
            .palace{
                background-size: 10%;
                flex: 1 1 10%;
            }
            .fours_Palace{
                flex: 1 1 33.33%!important;
                height: 33.33% !important;
                &:nth-child(1){
                    flex: 1 1 100% !important;
                    height: 66.66% !important;
                }
            }
            /* 六 */
            .Six_Palace{
                flex: 1 1 33.33%!important;
                height: 33.33% !important;
                &:nth-child(1){
                    flex: 1 1 66.66% !important;
                    height: 66.66% !important;
                }
                &:nth-child(3){
                    width: 33.33% !important;
                    position: absolute;
                    top: 33.33%;
                    right: 0;
                }
            }
            .sixts{
                height: 100% !important;
                width: 100% !important;
                position: absolute;
                top: 0;
                left: 0;
                z-index: 1000;
            }
            /* 十三宫格 */
            .videoflexitem{
                flex: 1 1 25% !important;
                width: 25% !important;
                height: 25% !important;
                &:nth-child(6){
                    position: absolute;
                    top: 50%;
                    left: 0;
                }
                &:nth-child(7){
                    flex: 1 1 50% !important;
                    height: 50% !important;
                }
                &:nth-child(8){
                    flex: 1 1 25% !important;
                    position: absolute;
                    top: 50%;
                    right: 0;
                }
            }
            /* 七宫格 */
            .Seven_Palace{
                flex: 1 1 33.33%;
                height: 33.33% !important;
                &:nth-child(1){
                    height: 100% !important;
                }
                &:nth-child(3){
                    width: 33.33% !important;
                    position: absolute;
                    top: 33.33%;
                    right: 0;
                }
                &:nth-child(2){
                    width: 33.33% !important;
                    position: absolute;
                    top: 33.33%;
                    right: 33.33%;
                }
                &:nth-child(6){
                    width: 33.33% !important;
                    position: absolute;
                    bottom: 0;
                    right: 33.33%;
                }
                &:nth-child(7){
                    width: 33.33% !important;
                    position: absolute;
                    bottom: 0;
                    right: 0;
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
        .liveview_group{
            width: 100%;
            padding: 20px 20%;
            display: flex;
            justify-content: space-between;
            button{
                border: none;
                background: none;
                font-size: 30px;
            }
        }
    }
    .liveview_right2{
        display: none;
        width: 0%;
        height: 90vh;
        .el-tabs__nav.is-top{
            width: 100%;
        }
        .el-tabs__item{
            width: 25% !important;
            text-align: center;
        }
        // #aicanvas{
        //     // height: 2000px;
        // }
        .border_card{
            border: none !important;
            .icon{
                font-size: 20px;
            }
            .el-tabs__header{
                border: none !important;
            }
            .tab_pane{
                height: 82vh;
                overflow: auto;
                overflow-x: auto; 
                overflow-y: auto;
                &::-webkit-scrollbar {/*滚动条整体样式*/
                    width: 8px;     /*高宽分别对应横竖滚动条的尺寸*/
                    height: 8px;
                    scrollbar-arrow-color:red;
                }
                &::-webkit-scrollbar-thumb {/*滚动条里面小方块*/
                    border-radius: 5px;
                    -webkit-box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                    box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                    background: rgba(218, 218, 218,0.2);
                    scrollbar-arrow-color:red;
                }
                &::-webkit-scrollbar-track {/*滚动条里面轨道*/
                    -webkit-box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                    box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                    border-radius: 0;
                    background: rgba(218, 218, 218,0.1);
                }
                .datanodecam{
                    width: 100%;
                    height: 100%;
                    display: flex;
                    justify-content: space-between;
                    align-items: center;
                }
                &:nth-child(4){
                    margin-left: 10px;
                }
            }
            .el-tabs__item {
                border:none;
            }
            // .el-tabs__header.is-top{
            //     background: #181818;
            // }
        }
    }
    .liveview_right3{
        width: 84%;
    }
    .liveview_rights{
        width: 100%;
        #video_hed{
            height: 100%;
        }
        .liveview_group{
            display: none;
        }
    }
}
.el-popover.el-popper{
    .liveview_popover{
        .liveview_popover_top{
            display: flex;
            width: 100%;
            margin-bottom: 10px;
            div{
                width: 35%;
                line-height: 40px;
            }
            .liveview_left_input{
                width: 65%;
                .el-input__inner{
                    width: 100%;
                    border: none;
                }
            }
        }
        .liveview_popover_but{
            width: 100%;
            display: flex;
            justify-content: flex-end;
            button{
                border-radius: 5px;
                border: none;
            }
        }
    }
}
.none{
    display: block !important;
}
</style>