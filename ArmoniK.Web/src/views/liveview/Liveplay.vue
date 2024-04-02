<template>
    <div class="liveplay h5container" @drop="dropTarget()">
        <video class="h5video" :id="videoid" autoplay webkit-playsinline playsinline></video>
        <canvas class="h5vcanvas" :id="canvas2id"></canvas>                            
        <canvas v-show="canvasdisplay" :id="canvas3id" style="position:absolute;bottom:0;right:0;"></canvas>
        <canvas v-show="canvasdisplay" :id="canvasid" style="position:absolute;bottom:0;right:0;"></canvas>
        <div class="breath_light"></div>
        <div :id="videonameid" class="elqualitytoggle" 
            @dragover.prevent="dragover($event)"
            draggable="true" 
            @dragstart="drag($event)">
            {{videoname}} {{picturequality}}
            <input type="button" :value="valuebutton"  @click="Bitstream($event.target.value)" :id="inputid" class="spanpicturequality"/>
        </div>
        <!-- <div :id="rtcid" class=""></div> -->
        <div :id="warid" class="warid iconfont icon-jinggao"></div>
        <!-- 画质 -->
        <span class="spanquality elqualitytoggle" v-show="!canvasdisplay" :id="spanqualityid">
            <el-dropdown placement='top-end' trigger="click"  @command="handleCommand" class=" "  :id="qualityid">
                <span :id="elqualityid" class="el-dropdown-link">
                    <i class="el-icon-video-camera el-icon--left"></i>{{$t('message.live.Quality')}}
                </span>
                <el-dropdown-menu slot="dropdown" style="background: rgba(31, 31, 31, 0.7);color: #FFFFFF;border:none;" class="optionhuazhi">
                    <template >
                        <el-dropdown-item v-for="(it,index) in qualitylist"   :key="index" :command="it" style="color:#FFFFFF;">{{it.strName}}</el-dropdown-item>
                    </template>
                </el-dropdown-menu>
            </el-dropdown>
        </span>
        <div id="Audio_slider" class="Audio_slider">
            <span :class="(Audioslider==0)?'Audio_slider_jingyin':'Audio_slider_shengyin_fill'">
                <i class="iconfont icon-mn_shengyin_fill"></i>
            </span>
            <el-slider :step='0.1' :show-tooltip="false" :max='1' @input='Audiochange' v-model="Audioslider" style="width:80%;margin-left:10px"></el-slider>
        </div>
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
        <div class="liveplay_butt">
            <div style="display: flex;justify-content: flex-end;">
            <button :id="rtcid" class="iconfont icon-bofang open_button rotate" @click="open_liveplay_butt" style="font-size:12px;line-height: 14px;"></button>
            <button class="open_liveplay_butt" type="text" v-if="$store.state.liveviewrtc == 'RTC'">RTC</button>
            <button class="open_liveplay_butt" type="text" v-else>WS</button>
            <CDropdown
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
            </CDropdown>
            <button class="info_id_show iconfont" :class="(Audioslider==0)?'icon-jingyin':'icon-mn_shengyin_fill'" @click="Audiosetup"></button>
            <button class="info_id_show iconfont icon-liuliang" @click="Information"></button>
            <button class="iconfont" :class="Shoutwheatclass" @click="Shoutwheat"></button>
            <button class="iconfont icon-picfill" @click="DoSnapshotWeb"></button>
            <button class="iconfont icon-biaozhu1" style="font-size:19px;" @click="biaozhu"></button>
            <button class="iconfont icon-camerafill" @click="DoSnapshot"></button>
            <button class="iconfont icon-videofill" @click="DoManualRecordStart"></button>
            <button class="iconfont icon-radioboxfill" @click="DoManualRecordStop"></button>
            <button class="ptz_id_show iconfont icon-yuntai" @click="PtzControlShow"></button>
            <button class="iconfont" :class="dzfa" @click="discanvas" style="font-size: 13.5px;"></button>
            <button class="iconfont" :class="Fullscren" @click="FullScreen"></button>
            <button class="iconfont icon-roundclosefill" @click="CloseVideo('close')"></button>
            </div>
        </div>
        <div class="liveplay_ptz"  style="display:none padding:0px">
            <div class="flex_content">
                <div class="content_zoom">
                    <div class="key_zoom">
                        <div class="key_flex">
                            <div class="key_but" @mousedown ="PtzActionUpleft($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionUpleft($event)" @touchend="PtzActionStop($event)"></div>
                            <div class="key_but" @mousedown ="PtzActionUp($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionUp($event)" @touchend="PtzActionStop($event)"></div>
                            <div class="key_but" @mousedown ="PtzActionUpright($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionUpright($event)" @touchend="PtzActionStop($event)"></div>
                            <div class="key_but" @mousedown ="PtzActionLeft($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionLeft($event)" @touchend="PtzActionStop($event)"></div>
                            <div class="key_but" ></div>
                            <div class="key_but" @mousedown ="PtzActionRight($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionRight($event)" @touchend="PtzActionStop($event)"></div>
                            <div class="key_but" @mousedown ="PtzActionDownleft($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionDownleft($event)" @touchend="PtzActionStop($event)"></div>
                            <div class="key_but" @mousedown ="PtzActionDown($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionDown($event)" @touchend="PtzActionStop($event)"></div>
                            <div class="key_but" @mousedown ="PtzActionDownright($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionDownright($event)" @touchend="PtzActionStop($event)"></div>
                        </div>
                        
                    </div>
                    <div class="zoom">
                            <el-tooltip :enterable="false" :content="content.focusing" placement="top" effect="light">
                                <button class="iconfont icon-add-focus zoom_add" @mousedown ="PtzActionZoomIn($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionZoomIn($event)" @touchend="PtzActionStop($event)"></button>
                            </el-tooltip>
                            <el-tooltip :enterable="false" :content="content.focusing" placement="top" effect="light">
                                <button class="iconfont icon-reduce-focus zoom_add" @mousedown ="PtzActionZoomOut($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionZoomOut($event)" @touchend="PtzActionStop($event)"></button>
                            </el-tooltip>
                        </div>
                </div>
                <!-- 聚光等 -->
                <div class="zoom_g">
                    <el-tooltip :enterable="false" :content="content.Focus" placement="top" effect="light">
                        <button class="iconfont icon-jujiao2 zoom_add" @mousedown ="PtzActionfocusing($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionfocusing($event)" @touchend="PtzActionStop($event)"></button>
                    </el-tooltip>
                    <el-tooltip :enterable="false" :content="content.Focus" placement="top" effect="light">
                        <button class="iconfont icon-jujiao1 zoom_add" @mousedown ="PtzActionfocusings($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionfocusings($event)" @touchend="PtzActionStop($event)"></button>
                    </el-tooltip>

                    <el-tooltip :enterable="false" :content="content.aperture" placement="top" effect="light">
                        <button class="iconfont icon-guangquanjia zoom_add" @mousedown ="PtzActionaperture($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionaperture($event)" @touchend="PtzActionStop($event)"></button>
                    </el-tooltip>
                    <el-tooltip :enterable="false" :content="content.aperture" placement="top" effect="light">
                        <button class="iconfont icon-guangquanjian zoom_add"  @mousedown ="PtzActionaperturej" @mouseup="PtzActionStop" @touchstart ="PtzActionaperturej" @touchend="PtzActionStop"></button>
                    </el-tooltip>
                    <!-- 后四 -->
                    <el-tooltip :enterable="false" :content="content.LightON" placement="top" effect="light">
                        <button class="iconfont icon-light-open zoom_add" @mousedown ="PtzActionlighton($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionfocusing($event)" @touchend="PtzActionStop($event)"></button>
                    </el-tooltip>
                    <el-tooltip :enterable="false" :content="content.LightOFF" placement="top" effect="light">
                        <button class="iconfont icon-light-close zoom_add" @mousedown ="PtzActionlightoff($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionfocusings($event)" @touchend="PtzActionStop($event)"></button>
                    </el-tooltip>

                    <el-tooltip :enterable="false" :content="content.WiperON" placement="top" effect="light">
                        <button class="iconfont icon-kaiyushua zoom_add" @mousedown ="PtzActionwiperon($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionaperture($event)" @touchend="PtzActionStop($event)"></button>
                    </el-tooltip>
                    <el-tooltip :enterable="false" :content="content.WiperOFF" placement="top" effect="light">
                        <button class="iconfont icon-guanyushua zoom_add" @mousedown ="PtzActionwiperoff($event)" @mouseup="PtzActionStop($event)" @touchstart ="PtzActionaperture($event)" @touchend="PtzActionStop($event)"></button>
                    </el-tooltip>
                </div>
                <div class="Preset">
                    <div class="" style="text-align: center;">
                        <el-slider v-model="Preset_value" :show-tooltip="false" :max="1" :min="0.1" :step="0.1"></el-slider>
                        <span style="color:#ffffff;">{{Preset_value}}</span>
                    </div>
                    <div class="block">
                        <el-timeline>
                            <el-timeline-item placement="top" v-for="Pre in Presetdata" :key="Pre.strName" >
                                <el-card>
                                    <div class="preset_bgc">
                                        <input type="text" class="preset_input" :value="Pre.strName"/>
                                        <button type="button" class="iconfont icon-RectangleCopy1" @click="preset_Jump(Pre.strToken)"></button>
                                        <button type="button" class="iconfont icon-icon-test1" @click="preset_set(Pre.strToken,$event)"></button>
                                    </div>
                                </el-card>
                            </el-timeline-item>
                        </el-timeline>
                    </div>
                </div>
            </div>
        </div>
        <el-dialog
            :class="this.$store.state.darkMode == 'c-dark-theme' ? 'BlackClass':(this.$store.state.darkMode == 'blue'?'BlueClass':'DataClass')"
            append-to-body
            title="AI标注"
            :visible.sync="dialogVisible"
            :before-close="handleClose"
            width="60%">
            <div style="display:flex;">
                <div class="box_left" style="width:200px; height:500px; padding-right:10px; display:flex;flex-direction:column;">
                    <div>
                        <div>库</div>
                        <el-select v-model="value" placeholder="请选择" @change="reclass">
                            <el-option
                            v-for="item in options"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                            </el-option>
                        </el-select>
                    </div>
                    <div style="padding-top:20px; display:flex;justify-content:flex-end;">
                        <div>
                            <div style="padding-bottom:20px;"><el-button type="success" icon="el-icon-refresh-right" circle @click="refresh"></el-button></div>
                            <div style="padding-bottom:20px;"><el-button type="primary" icon="iconfont icon-yidong" circle  @click="mover"></el-button></div>
                            <div style="padding-bottom:20px;"><el-button type="danger" icon="el-icon-delete" circle @click="clears"></el-button></div>
                        </div>
                    </div>
                </div>
                <div>
                    <div class="box" style="position:relative;width:900px;height:500px; overflow: hidden;display:block;">
                        <div style="">
                            <canvas :id="canvasbz" width="900" height="500" style="position: absolute; top:0;left: 0;"></canvas>
                        </div>
                        <div :id="bzelect" style="position:absolute;display:none;">
                            <el-select v-model="value1" placeholder="请选择" @change="bzclass">
                                <el-option
                                v-for="item in options1"
                                :key="item.value"
                                :label="item.label"
                                :value="item.value">
                                </el-option>
                            </el-select>
                        </div>
                    </div>
                </div>
            </div>
            <span slot="footer" class="dialog-footer">
                <el-button @click="empty">清 空</el-button>
                <el-button type="primary" @click="sava">确 定</el-button>
            </span>
        </el-dialog>
    </div>
</template>
<script>
import QRCode from 'qrcodejs2';
import '../../assets/js/adapter'
import {H5sPlayerWS,H5sPlayerHls,H5sPlayerRTC,H5sPlayerAudBack} from '../../assets/js/h5splayer.js'
import {H5siOS,H5sPlayerCreate} from '../../assets/js/h5splayerhelper.js'
import uuid from '../../assets/js/uuid';
import images from './imgs/2000.png';
import DataClass from '../public/class';
export default {
    name:"liveplay",
    props:['h5id', 'h5videoid',"cols","rows"],
    data(){
        return{
            warid:'war'+this.h5id,
            Audioslider:0,//音频
            informationshow: false,
            informationAudio:[],
            informationVideo:[],
            proto: this.$store.state.liveviewrtc,
            videoid: this.h5videoid,
            videoid2: this.h5videoid+2,
            canvasid: 'canvas'+this.h5id,
            canvas2id: 'canvas2'+this.h5id,
            canvas3id: 'canvas3'+this.h5id,
            h5handler:undefined,//v1
            h5handler2:undefined,//v1
            audioback:undefined,//语音
            currtoken:undefined,//token
            tokenshou:null,
            ptzshow: false,
            audioshow: false,
            Fullscren:'icon-full',
            dzfa:'icon-chuangkoufangda2',
            Preset_value:0.5,//镜头转换速度
            Presetdata:[],//预置位数组
            Shoutwheatclass:"icon-voicefill1",//语音图标
            content:{
                focusing:this.$t('message.live.focusing'),
                Focus:this.$t('message.live.Focus'),
                aperture:this.$t('message.live.aperture'),

                LightON:this.$t('message.live.LightON'),
                LightOFF:this.$t('message.live.LightOFF'),
                WiperON:this.$t('message.live.WiperON'),
                WiperOFF:this.$t('message.live.WiperOFF')
            },//聚焦

            spanqualityid:"spanqualityid"+this.h5videoid,
            qualityid: 'quality'+this.h5videoid,
            elqualityid:'elqualityid'+this.h5videoid,
            videonameid:"name"+this.h5videoid,
            picturequalityid:"picturequalityid"+this.h5videoid,
            inputid:"input"+this.h5videoid,
            qualitylist:[],
            valuebutton:this.$t("message.live.substream"),
            videoname:"",//视频名称
            picturequality:'',//画质名称
            rtcid:"rtc"+this.h5videoid,
            streamprofileInformation:null,//码流区分
            height:"",
            width:"",
            canvasdisplay:false,
            videowidth:'',
            videoheigh:'',
            resolutionw:'',
            resolutionh:'',
            name:'',
            interval:'',
            RootCloseTokenId:'',
            options: [],
            options1: [],
            value: '',
            value1:'',
            canvasbz:'labels'+this.h5videoid,
            bzelect:'elect'+this.h5videoid,
            dialogVisible:false,
            width:'',
            height:'',
            img:'',
            dataclass:null
        }
    },
    beforeDestroy() {
        this.CloseVideo('close');
    },
    computed: {
        listData() {
            return this.$store.state.liveplay;
        },
        allclosevideo(){
            return this.$store.state.offVideo;
        },
        jiaohuan(){
            return this.$store.state.jiaohuan;
        }
    },
    mounted(){
        this.class();
        let _this = this;
        // console.log(this.$store.state.liveplay)
        // 默认关闭调节声音slider
        $('.Audio_slider').hide();

        $("#"+this.spanqualityid).addClass("spanquality")
        $("#"+this.inputid).addClass("spanpicturequality")
        this.Gettranscod();
        this.$root.bus.$on('dbclick',function(r,c){
            _this.canvasdisplay = false;
            if ('h'+r+c == _this.h5id) {
                _this.dbvideocanvas();
                
            }
        });
        window.addEventListener('resize', () => {
            if(!this.checkFull() &&this.$store.state.FullScreenId!=''){
                // 退出全屏后要执行的动作
                console.log(this.$store.state.FullScreenId);
                this.Fullscren='icon-full';
                var $container =$('#'+this.$store.state.FullScreenId);
                var liveplay_butt = $container.children(".liveplay_butt");
                var suoxiao = liveplay_butt.children(".icon-suoxiao");
                suoxiao.removeClass("icon-suoxiao");
                suoxiao.addClass("icon-full");
                _this.canvasdisplay = false
            }
        });
        this.elqualitytoggle();
        if (localStorage.getItem(this.$store.state.user)) {
            let strLayoutType = JSON.parse(localStorage.getItem(this.$store.state.user))['strLayoutType'];
            if (strLayoutType=="1|13"||strLayoutType=="4|4"||strLayoutType=="5|5") {
                this.$nextTick(()=>{
                    $('.liveplay_butt').css('background-size', '295px 35px');
                })
            }else{
                $('.liveplay_butt').css('background-size', '395px 35px');
            }
        }
        this.$root.bus.$on('strLayoutTypes',function(data){
            $('.liveplay_butt').children().children('.open_button').addClass('rotate');
            $('.liveplay_butt').children().children('.open_liveplay_butt').css("display", "none");
            if (data=="1|13"||data=="4|4"||data=="5|5") {
                this.$nextTick(()=>{
                    $('.liveplay_butt').css('background-size', '295px 35px');
                }) 
            }else if (data=="3|3"){
                this.$nextTick(()=>{
                    setTimeout(()=>{
                        $('.liveplay_butt').css('background-size', '395px 35px');
                    },25)
                })
            }else{
                this.$nextTick(()=>{
                    setTimeout(()=>{
                        $('.liveplay_butt').css('background-size', '395px 35px');
                    },25)
                })
            }
        });
    },
    watch:{
        listData(token){
            if(token.viewparameter=='viewparameter'){
                this.CloseVideo(token.viewparameter);
            }
            setTimeout(()=>{
                if (this.h5id != token.vid)
                {
                    return;
                }
                this.PlayVideo(token.token,token.streamprofile,token.label,token.name);
                this.tokenshou=token.token;
                this.RootCloseTokenId=token.treeid;
                this.streamprofileInformation=token.streamprofile
                var root = this.$store.state.IPPORT;
                var url = root + "/api/v1/GetVidStreamStatus?token="+token.token+"&stream="+token.streamprofile+"&session="+ this.$store.state.token;
                //重组
                let resinterval = setInterval(()=>{
                    this.$http.get(url).then(result=>{
                        if(result.status == 200){
                            if (result.data.nVideoWidth) {
                                clearInterval(resinterval);
                            }
                            this.resolutionw = result.data.nVideoWidth
                            this.resolutionh = result.data.nVideoHeight
                        }
                    })
                },5000)
                setTimeout(()=>{
                     clearInterval(resinterval);
                },20000)
            },200)
        },
        allclosevideo(close){
            this.AllCloseVideo(close);
        },
        jiaohuan(jiao){
            if (jiao==this.h5videoid) {
                this.$store.state.jiaohuan='';
                console.log(this.inputid);
                this.CloseVideo('close',false);
                let datas = this.$store.state.drag;
                this.PlayVideo(datas.two.strToken,datas.two.strStream,null,datas.two.name)
                this.tokenshou=datas.two.strToken;
                this.streamprofileInformation=datas.two.strStream;
                let viewdata1 = {};
                let viewdata2 = {};
                viewdata1['strIndex'] = this.$store.state.drag.two.strId
                viewdata1['strToken'] =this.$store.state.drag.one.strToken
                viewdata1['strStream'] =this.$store.state.drag.one.strStream
                this.$store.state.liveviewadd.push(viewdata1)
                if (this.$store.state.drag.two.strToken!='') {
                    viewdata2['strIndex'] = this.$store.state.drag.one.strId
                    viewdata2['strToken'] =this.$store.state.drag.two.strToken
                    viewdata2['strStream'] =this.$store.state.drag.two.strStream
                    this.$store.state.liveviewadd.push(viewdata2)
                }
            }
        }
    },
    methods:{
        clears(){
            if (this.dataclass) {
                $('#'+this.bzelect).hide();
                this.dataclass.clears();
            }
        },
        mover(){
            if (this.dataclass) {
                this.dataclass.movers();
            }
        },
        refresh(){
            if (this.dataclass) {
                $('#'+this.bzelect).hide();
                this.dataclass.refresh();
            }
        },
        class(){
            var url = this.$store.state.IPPORT + "/api/v2/label/GetRepository?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
            //   return false;
              if(result.status == 200){
                result.data.forEach(item => {
                    this.options.push({
                        value:item,
                        label:item
                    })
                });
             }
            }).catch((err)=>{
                console.log(11111);
            })
        },
        getclass(repository,y,x){
            $('#'+this.bzelect).show();
            $('#'+this.bzelect).css({top:y+'px',left:x+"px"});
            this.options1=[];
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v2/label/GetClass?repository="+repository+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    result.data.forEach(item => {
                        this.options1.push({
                            value:item,
                            label:item
                        })
                    });
                }
            })
        },
        sava(){
            if (this.dataclass) {
                $('#'+this.bzelect).hide();
                let layersclass = this.dataclass.layers;
                let width = this.dataclass.width;
                let height = this.dataclass.height;
                let img = this.dataclass.img;
                let layers = []
                if (layersclass.length>0) {
                    layersclass.forEach((item,i) => {
                        let data = {
                            token:this.dataclass.token,
                            width:item.width*width/900,
                            height:item.height*height/500,
                            type:item.type,
                            x1:item.x1*width/900,
                            y1:item.y1*height/500,
                            x2:item.x2*width/900,
                            y2:item.y2*height/500
                        }
                        layers.push(data)
                    })
                    var canvas = document.createElement('canvas');
                    canvas.width = width;
                    canvas.height = height;
                    let ctx = canvas.getContext('2d');
                    ctx.clearRect(0, 0, width, height);
                    ctx.drawImage(img, 0, 0, width, height);
                    var MIME_TYPE = "image/jpeg";
                    var imgURL = canvas.toDataURL(MIME_TYPE,1.0);
                    let data = {
                        width:width,
                        height:height,
                        type : 'ai',
                        repository:this.value,
                        object:layers,
                        imgurl:imgURL
                    }
                    console.log(data);
                    var url = this.$store.state.IPPORT + "/api/v2/AddLabelImg?session="+ this.$store.state.token;
                    this.$http.post(url,JSON.stringify(data),{headers: {'Content-Type': 'application/json'}}).then(result=>{
                        if (result.status === 200) {
                            this.$message({
                                message: "发送成功",
                                type: 'success'
                            });
                        }
                    })
                }
            }
        },
        Point(x, y, type) {
            this.x = x
            this.y = y
            this.type = type // 左击 1  右击 3
        },
        bzclass(){
            if (this.dataclass) {
                this.dataclass.bzclass(this.value1)
            }
        },
        reclass(){
            if (this.dataclass) {
                this.dataclass.reclass(this.value)
            }
        },
        reshow(item){
            $('#'+this.bzelect).show();
            $('#'+this.bzelect).css({top:item.y2+'px',left:item.x2+"px"});
            this.value1 = item.type;
        },
        handleClose(){
            this.dialogVisible =false;
            this.dataclass = null;
        },
        empty(){
            $('#'+this.bzelect).hide();
            if (this.dataclass) {
                this.dataclass.clear()
            }
        },
        // 标注
        biaozhu(){
            $('#'+this.bzelect).hide();
            this.dialogVisible=true;
            let conf = {
                token : this.currtoken,
                videoid : this.videoid,
                bzelect : this.bzelect,
                canvasid : this.canvasbz,
                value : this.value,
                getclass:this.getclass,
                reshow:this.reshow,
            }
            console.log(conf);
            setTimeout(()=>{
                let Dataclass = new DataClass(conf);
                this.dataclass = Dataclass;
                console.log(Dataclass);
            },100)
        },
        dbvideocanvas(){
            let img = new Image();
            img.src = images;
            let canvas = document.getElementById(this.canvasid);
            let canvas2 = document.getElementById(this.canvas2id);
            let canvas3 = document.getElementById(this.canvas3id);
            let width = $('#'+this.videoid).width()/4;
            let height = $('#'+this.videoid).height()/4;
            let context2 = canvas2.getContext('2d');
            canvas2.width=$('#'+this.videoid).width()
            canvas2.height=$('#'+this.videoid).height()
            $('.h5video2').width($('#'+this.videoid).width()/4);
            $('.h5video2').height($('#'+this.videoid).height()/4);
            canvas.width=width;
            canvas.height=height;
            canvas3.width=width;
            canvas3.height=height;
            context2.clearRect(0, 0, canvas2.width, canvas2.height);
        },
        // 展开或收缩liveplay上面的button按钮
        open_liveplay_butt(){
            if ($("#"+this.rtcid)[0].className=="iconfont icon-bofang open_button rotate") {
                $("#"+this.rtcid).siblings('.open_liveplay_butt').css("display", "block");
                $("#"+this.rtcid).removeClass('rotate');
                $("#"+this.rtcid).parent().parent('.liveplay_butt').css('background-size', '435px 35px');
            }else{
                $("#"+this.rtcid).siblings('.open_liveplay_butt').css("display", "none");
                $("#"+this.rtcid).addClass('rotate');
                $("#"+this.rtcid).parent().parent('.liveplay_butt').css('background-size', '395px 35px');
            }
        },
        //画质选择，是否显示
        elqualitytoggle(){
            if(typeof(localStorage.elqualitytoggle) == 'undefined'){
                if ($("#"+this.spanqualityid).get(0).className=="spanquality elqualitytoggle") {
                    $("#"+this.spanqualityid).removeClass("elqualitytoggle")
                }else{
                    $("#"+this.spanqualityid).addClass("elqualitytoggle")
                }
            }else{
                if(localStorage.elqualitytoggle == 'block') {
                    if ($("#"+this.spanqualityid).get(0).className=="spanquality elqualitytoggle") {
                        $("#"+this.spanqualityid).removeClass("elqualitytoggle")
                    }else{
                        $("#"+this.spanqualityid).addClass("elqualitytoggle")
                    }
                }else if(localStorage.elqualitytoggle == 'none'){
                    $('.elqualitytoggle').css('display', 'none');
                }
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
        //拖动播放
        drag(e){
            this.$store.state.drag.drags = true;
            this.$store.state.dragid = this.h5id;
            console.log(this.$store.state.liveviewadd);
            let data= this.$store.state.liveviewadd;
            for (let i = 0; i < data.length; i++) {
                if (data[i].strIndex ==  this.h5id) {
                    this.$store.state.drag.one.strId = this.h5id
                    this.$store.state.drag.one.strIndex = this.h5videoid
                    this.$store.state.drag.one.strStream = data[i].strStream
                    this.$store.state.drag.one.strToken = data[i].strToken
                    this.$store.state.drag.one.name = this.name
                }
                
            }
            console.log(this.$store.state.drag.one);
        },
        dragover (e) {
            // console.log(e)
        },
        dropTarget () {
            if (this.$store.state.drag.drags&&this.$store.state.dragid!=this.h5id) {
                this.$store.state.drag.drags = false
                // let viewdata = JSON.parse(localStorage.getItem(this.$store.state.user));
                // let data= viewdata.src;
                let data= this.$store.state.liveviewadd;
                console.log(data);
                this.$store.state.jiaohuan = this.$store.state.drag.one.strIndex;
                let token =''
                this.$store.state.drag.two.strId = this.h5id
                this.$store.state.drag.two.strIndex = ''
                this.$store.state.drag.two.strStream = ''
                this.$store.state.drag.two.strToken = ''
                this.$store.state.drag.two.name = ''
                for (let i = 0; i < data.length; i++) {
                    if (data[i].strIndex ==  this.h5id) {
                        this.$store.state.drag.two.strId = this.h5id
                        this.$store.state.drag.two.strIndex = this.h5videoid
                        this.$store.state.drag.two.strStream = data[i].strStream
                        this.$store.state.drag.two.strToken = data[i].strToken
                        this.$store.state.drag.two.name = this.name
                        token = data[i].strToken;
                    }
                }
                
                let datas = this.$store.state.drag;
                this.CloseVideo('close',false);
                var root = this.$store.state.IPPORT;
                var url = root + "/api/v1/GetVidStreamStatus?token="+datas.one.strToken+"&stream="+datas.one.strStream+"&session="+ this.$store.state.token;
                //重组
                this.$http.get(url).then(result=>{
                    if(result.status == 200){
                        this.resolutionw = result.data.nVideoWidth
                        this.resolutionh = result.data.nVideoHeight
                        this.PlayVideo(datas.one.strToken,datas.one.strStream,null,datas.one.name);
                        this.tokenshou=datas.one.strToken;
                        this.streamprofileInformation=datas.one.strStream;
                        this.$root.bus.$emit('dragToken',datas.one.strToken);
                        this.$root.bus.$emit('dragToken',this.$store.state.drag.two.strToken);
                    }
                })
            }
        },
        // 点击el-tree录像小圆点显示
        TreeRecdot(token,vid){
            var $container = $("#"+vid);
            var $controlrec = $container.children(".breath_light");
            $controlrec.css("display", "none");
            if (localStorage.getItem('bRec')) {
                let bRecToken = JSON.parse(localStorage.getItem('bRec'));
                for (let k = 0; k < bRecToken.length; k++) {
                    const rectoken = bRecToken[k];
                    if (token==rectoken) {
                        this.$nextTick(()=>{
                            var $container = $("#"+vid);
                            var $controlrec = $container.children(".breath_light");
                            $controlrec.css("display", "block");
                        })
                    }
                }
            }
        },
        //音频
        Audiosetup(){
            var $container = $("#"+this.h5id);
            var $controlsin = $container.children(".Audio_slider");
            if (this.audioshow == false){
                // $controlsin.css("display", "block");
                $controlsin.show();
                this.audioshow = true;
            }else{
                // $controlsin.css("display", "none");
                $controlsin.hide();
                this.audioshow = false;
            }
        },
        Audiochange(){
            //要打开音频设置
            console.log("要打开音频设置",this.Audioslider)
            document.getElementById(this.h5videoid).volume=this.Audioslider
        },
        //码率
        Information(){
            console.log('124')
            var $container = $("#"+this.h5id);
            var $controlsin = $container.children(".information");
            var cors=this.cols*this.rows;
            console.log(this.tokenshou)
            if(cors>9||this.tokenshou==undefined){
                return false
            }
            if (this.informationshow == false)
            {
                console.log('12455',cors)
                $controlsin.css("display", "block");
                this.informationshow = true;
            }else
            {
                $controlsin.css("display", "none");
                this.informationshow = false;
                clearInterval(this.timerRunInfo);
                return false
            }
            this.Informationdate()
            this.timerRunInfo = setInterval(() => {
                this.Informationdate()
            },8000)

        },
        Informationdate(){
            var root = this.$store.state.IPPORT;
            //url
            var url = root + "/api/v1/GetVidStreamStatus?token="+this.tokenshou+"&stream="+this.streamprofileInformation+"&session="+ this.$store.state.token;
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
        //码流按钮
        Bitstream(event){
            let data= JSON.parse(localStorage.getItem(this.$store.state.user));
            console.log(event,this.$t("message.live.substream"))
            // return false
            if(event===this.$t("message.live.substream")){
                this.streamprofileInformation='sub'
                var streamprofile='sub'
                var token=this.inputtoken
                var label=this.inputlabel
                var arr=label.substring(0,this.inputlabel.lastIndexOf("-"))
                var name=arr+"-"+this.$t("message.live.substream")
                this.valuebutton=this.$t("message.live.mainstream")
                this.picturequality='';
                for (let i = 0; i < data.src.length; i++) {
                    if(data.src[i].strIndex==this.h5id){
                        data.src[i].strStream='sub'
                    }
                    
                }
                localStorage.setItem(this.$store.state.user,JSON.stringify(data));
                this.$store.state.liveviewadd = data.src;
                var root = this.$store.state.IPPORT;
                var url = root + "/api/v1/GetVidStreamStatus?token="+token+"&stream="+streamprofile+"&session="+ this.$store.state.token;
                //重组
                this.$http.get(url).then(result=>{
                    if(result.status == 200){
                        this.resolutionw = result.data.nVideoWidth;
                        this.resolutionh = result.data.nVideoHeight;
                        this.PlayVideo(token,streamprofile,label,name)
                    }
                })
            }else if(event===this.$t("message.live.mainstream")){
                this.streamprofileInformation='main'
                var streamprofile='main'
                var token=this.inputtoken
                var arr=this.inputlabel.substring(0,this.inputlabel.lastIndexOf("-"))
                var name=arr+"-"+this.$t("message.live.mainstream")
                this.picturequality='';
                for (let i = 0; i < data.src.length; i++) {
                    if(data.src[i].strIndex==this.h5id){
                        data.src[i].strStream='main'
                    }
                    
                }
                localStorage.setItem(this.$store.state.user,JSON.stringify(data));
                this.$store.state.liveviewadd = data.src;
                var root = this.$store.state.IPPORT;
                var url = root + "/api/v1/GetVidStreamStatus?token="+token+"&stream="+streamprofile+"&session="+ this.$store.state.token;
                //重组
                this.$http.get(url).then(result=>{
                    if(result.status == 200){
                        this.resolutionw = result.data.nVideoWidth;
                        this.resolutionh = result.data.nVideoHeight;
                        this.PlayVideo(token,streamprofile,label,name)
                    }
                })
            }
        },
        // 转码
        Gettranscod(){
            var url = this.$store.state.IPPORT + "/api/v1/GetTransProfile?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
            //   return false;
              if(result.status == 200){
                this.qualitylist=result.data.profile
                //   var itme=result.data.profile;
                //   for(var i=0;i<itme.length;i++){
                //       var qualitydata={
                //           strName:itme[i].strName,
                //           strToken:itme[i].strToken,
                //           nCodec:itme[i].nCodec,
                //           nEngine:itme[i].nEngine,
                //           nBitrate:itme[i].nBitrate,
                //           nFPSType:itme[i].nFPSType,
                //           nFPS :itme[i].nFPS,
                //           nScaleType:itme[i].nScaleType,
                //           nWidth:itme[i].nWidth,
                //           nHeight:itme[i].nHeight,
                //       };
                //       this.qualityform.push(qualitydata);
                //   }

             }
            }).catch()
        },
        // 画质
        handleCommand(command) {
            this.height =command.nHeight
            this.width =command.nWidth
            this.videoname=''
            var arr=this.inputlabel.substring(0,this.inputlabel.lastIndexOf("-"))
            var name=arr+"-"+command.strName
            var token=this.inputtoken
            this.videoname=name
            var streamprofile= command.strToken
            this.PlayVideo(token,streamprofile,"",name)
           
        },
        discanvas(){
            clearInterval(this.interval);
            let canvas2 =document.getElementById(this.canvas2id);
            let context2 = canvas2.getContext('2d');
            context2.clearRect(0, 0, canvas2.width, canvas2.height);
            let _this = this;
            this.canvasdisplay = !this.canvasdisplay;
            
            this.videowidth = $('#'+this.videoid).width();
            this.videoheight = $('#'+this.videoid).height();
            if (this.canvasdisplay) {
                this.videocanvas()
            }
            this.$root.bus.$emit('ai_click',this.canvasdisplay);
            
        },
        //canvas映射
        videocanvas(){
            let img = new Image();
            img.src = images;
            let _this = this;
            let video = document.getElementById(this.videoid);
            let canvas = document.getElementById(this.canvasid);
            let canvas2 = document.getElementById(this.canvas2id);
            let canvas3 = document.getElementById(this.canvas3id);
            let width = $('#'+this.videoid).width()/4;
            let height = $('#'+this.videoid).height()/4;
            let context = canvas.getContext('2d');
            let context2 = canvas2.getContext('2d');
            let context3 = canvas3.getContext('2d');
            canvas2.width=$('#'+this.videoid).width()
            canvas2.height=$('#'+this.videoid).height()
            $('.h5video2').width($('#'+this.videoid).width()/4);
            $('.h5video2').height($('#'+this.videoid).height()/4);
            canvas.width=width;
            canvas.height=height;
            canvas3.width=width;
            canvas3.height=height;
            clearInterval(this.interval);
            let x=0 ,y=0 ,disX = 0, disY = 0, disW = width, disH = height;
            this.interval = setInterval(function() {
                if(video.paused || video.ended) {
                    return;
                }
                captureFrame();
            },40)
            function captureFrame() {
                if (disW/width==1) {
                    disX = 0;
                    disY = 0;
                }
                if (_this.resolutionw*disW/width>_this.resolutionw-disX) {
                    disX = _this.resolutionw-_this.resolutionw*disW/width;
                }
                if (_this.resolutionh*disH/height>_this.resolutionh-disY) {
                    disY = _this.resolutionh-_this.resolutionh*disH/height;
                }
                context2.clearRect(0, 0, canvas2.width, canvas2.height);
                context2.drawImage(video, disX, disY,_this.resolutionw*disW/width,_this.resolutionh*disH/height,0,0,canvas2.width,canvas2.height);
                context3.clearRect(0, 0, canvas3.width, canvas3.height);
                context3.drawImage(video, 0, 0,width,height);
            }
            canvas.onmousedown = function (e) {
                x = e.clientX - canvas.getBoundingClientRect().left; y = e.clientY - canvas.getBoundingClientRect().top;
                // console.log(x, y);
                //限制移动不能超出画布
                if (x <= disW / 2) x = 0;
                if (x > disW / 2 && x < width - disW / 2) x = x - disW / 2;
                if (x >= width - disW / 2) x = width - disW;
                if (y <= disH / 2) y = 0;
                if (y > disH / 2 && y < height - disH / 2) y = y - disH / 2;
                if (y >= height - disH / 2) y = height - disH;
                //先清除之前的然后重新绘制
                context.clearRect(0, 0, canvas.width, canvas.height);
                context.drawImage(img, x, y, disW, disH);
                disX = _this.resolutionw/disW * x; disY = _this.resolutionh/disH* y;
                canvas.onmousemove = function (e) {
                    x = e.clientX - canvas.getBoundingClientRect().left; y = e.clientY - canvas.getBoundingClientRect().top;
                    // console.log(x,disW,width);
                    //限制移动不能超出画布
                    if (x <= disW / 2) x = 0;
                    if (x > disW / 2 && x < width - disW / 2) x = x - disW / 2;
                    if (x >= width - disW / 2) x = width - disW;
                    if (y <= disH / 2) y = 0;
                    if (y > disH / 2 && y < height - disH / 2) y = y - disH / 2;
                    if (y >= height - disH / 2) y = height - disH;
                    disX = _this.resolutionw/width * x; disY = _this.resolutionh/height* y
                    //先清除之前的然后重新绘制
                    context.clearRect(0, 0, canvas.width, canvas.height);
                    context.drawImage(img, x, y, disW, disH);
                };

                canvas.onmouseup = function () {
                    canvas.onmousemove = null;
                    canvas.onmouseup = null;
                };
            }
            canvas2.onmousewheel = canvas.onmousewheel = function (event) {
                if (event.wheelDeltaY > 0) {
                    if (disW < width) {
                        disW = disW + width / 4;
                        disH = disH + height / 4;
                    } else {
                        disW = width;
                        disH = height;
                    }
                } else {
                    if (disW < width/2) {
                        disW = width / 4; 
                        disH = height / 4;
                    } else {
                        disW = disW - width / 4;
                        disH = disH - height / 4;
                    }
                    console.log(222,x,y);
                }
                if (canvas.width-x<disW) {
                    x = canvas.width-disW;
                }
                if (canvas.height-y<disH) {
                    y = canvas.height-disH;
                }
                context.clearRect(0, 0, canvas.width, canvas.height);
                context.drawImage(img, x, y, disW, disH);
            }
        },
        //播放
        PlayVideo(token,streamprofile,label,name){
            let canvas2 =document.getElementById(this.canvas2id);
            let context2 = canvas2.getContext('2d');
            context2.clearRect(0, 0, canvas2.width, canvas2.height);
            $('#'+this.canvas2id).show()
            if (token=='') return
            this.name =name
            if (this.h5handler != undefined)
            {   this.$root.bus.$emit('closeToken',this.h5handler.T);
                if (localStorage.getItem('treeid')) {
                    var treeid =  JSON.parse(localStorage.getItem('treeid'));
                    for (let k = 0; k < treeid.length; k++) {
                        if (this.h5handler.T==treeid[k].strToken) {
                            this.$root.bus.$emit('RootCloseToken',treeid[k].treeid,treeid[k].firstlevel,treeid[k].strToken);
                        }
                    }
                }
                this.h5handler.disconnect();
                delete this.h5handler;
                this.h5handler = undefined;
                // setTimeout(()=>{
                //     var $container = $("#"+this.h5id);
                    if(document.getElementById(this.h5videoid)){
                        $("#" + this.h5videoid).get(0).poster = '';
                        $("#" + this.h5videoid).get(0).load();
                        // this.$nextTick(()=>{
                        //     $("#" + this.h5videoid).remove();
                        //     $container.prepend(`<video style="width:100%;height:100%;object-fit: fill;" class="h5video" id=${this.h5videoid}  autoplay webkit-playsinline playsinline></video>`);
                        // })
                    }
                // },330)
            }
            // setTimeout(() =>{
                this.currtoken = token;

                var videosize = document.querySelector('#'+this.h5id);
                // return false;
                this.inputtoken=token
                this.inputlabel=name
                this.streamprofile=streamprofile
                // this.streamprofile=streamprofile
                this.videoname=name;//视频名称
            
                $("#"+this.videonameid).addClass("videoname");
                $("#"+this.inputid).addClass("streambutton")
                $("#"+this.qualityid).addClass("quality")
                $("#"+this. picturequalityid).removeClass("picturequality")      
                //console.log("*********************",label,token);
                $("#"+this.spanqualityid).removeClass("spanquality")
                $("#"+this.inputid).removeClass("spanpicturequality") 
                this.currtoken = token;
                // console.log(streamprofile,"111111111111111111*****")
                if(streamprofile==="sub"){
                    this.valuebutton=this.$t("message.live.mainstream")
                } else if(streamprofile==="main"){
                    this.valuebutton=this.$t("message.live.substream")
                }else{
                    this.valuebutton=this.$t("message.live.substream")
                }
                let conf = {
                    videoid: this.h5videoid,
                    protocol: window.location.protocol, //http: or https:
                    host: this.$store.state.WSROOT, //localhost:8080
                    streamprofile: streamprofile, // {string} - stream profile, main/sub or other predefine transcoding profile
                    rootpath: '/', // '/'
                    token: token,
                    hlsver: 'v1', //v1 is for ts, v2 is for fmp4
                    rtcengine: localStorage.getItem('H5sRtcengine')||'v1',
                    session: this.$store.state.token, //session got from login
                    consolelog: 'true'
                };
                console.log(conf);
                if (this.$store.state.liveviewrtc == 'RTC' || (H5siOS() === true)){
                    console.log(this.$store.state.liveviewrtc)
                    this.h5handler = new H5sPlayerRTC(conf);
                    // $("#"+this.rtcid).addClass("rtc_new");
                }else{
                    console.log(this.$store.state.liveviewrtc)
                    this.h5handler = new H5sPlayerWS(conf);
                    // $("#"+this.rtcid).removeClass("rtc_new");
                }
                this.h5handler.connect();
                this.$store.state.liveplay={
                    token:null,
                    streamprofile:null,
                    name:null,
                    label:null,
                    vid:null,
                    viewparameter:null
                }
                if (localStorage.getItem('bRec')) {
                    let bRecToken = JSON.parse(localStorage.getItem('bRec'));
                    for (let k = 0; k < bRecToken.length; k++) {
                        const rectoken = bRecToken[k];
                        if (token==rectoken) {
                            this.$nextTick(()=>{
                                var $container = $("#"+this.h5id);
                                var $controlrec = $container.children(".breath_light");
                                $controlrec.css("display", "block");
                            })
                        }else{
                            this.$nextTick(()=>{
                                var $container = $("#"+this.h5id);
                                var $controlrec = $container.children(".breath_light");
                                $controlrec.css("display", "none");
                            })
                        }
                    }
                }
            // },330)
        },
        // 全部关闭
         AllCloseVideo(){
            $('.h5vcanvas').hide()
            $('.warid').hide()
            $(".h5container").removeClass('h5videoh1');
            this.$root.bus.$emit('close',true)
             // console.log(viewparameter)
            this.$store.state.videoid = this.videoid+uuid();
            // 关闭实时协议显示
            if (localStorage.getItem(this.$store.state.user)) {
                let strLayoutType = JSON.parse(localStorage.getItem(this.$store.state.user))['strLayoutType'];
                this.$nextTick(()=>{
                    $('.liveplay_butt').children().children('.open_button').addClass('rotate');
                    $('.liveplay_butt').children().children('.open_liveplay_butt').css("display", "none");
                    if (strLayoutType=="1|13"||strLayoutType=="4|4"||strLayoutType=="5|5") {
                        this.$nextTick(()=>{
                           setTimeout(()=>{
                                $('.liveplay_butt').css('background-size', '295px 35px');
                           },25)
                        }) 
                    }else if (strLayoutType=="3|3"){
                        this.$nextTick(()=>{
                            setTimeout(()=>{
                                $('.liveplay_butt').css('background-size', '395px 35px');
                            },25)
                        })
                    }else{
                        this.$nextTick(()=>{
                            setTimeout(()=>{
                                $('.liveplay_butt').css('background-size', '395px 35px');
                            },25)
                        })
                    }
                })
            }
            // if(viewparameter==='close'){
                for(var i=0;i<this.$store.state.liveviewadd.length;i++){
                    if(this.$store.state.liveviewadd[i].strToken==this.tokenshou){
                        this.$root.bus.$emit('closeToken',this.$store.state.liveviewadd[i].strToken);
                        if (localStorage.getItem('treeid')) {
                            var treeid =  JSON.parse(localStorage.getItem('treeid'));
                            for (let k = 0; k < treeid.length; k++) {
                                if (this.$store.state.liveviewadd[i].strToken==treeid[k].strToken) {
                                    this.$root.bus.$emit('RootCloseToken',treeid[k].treeid,treeid[k].firstlevel,treeid[k].strToken);
                                    treeid.splice(k, 1);
                                    localStorage.setItem('treeid',JSON.stringify(treeid));
                                }
                            }
                        }else{
                            this.$root.bus.$emit('RootCloseToken',this.$store.state.liveviewadd[i].treeid);
                        }
                        this.$store.state.liveviewadd.splice(i,1)
                        console.log(this.$store.state.liveviewadd)
                    }
                }
                
            // }
            var $container = $("#"+this.h5id);
            // 录像
            var $controlrec = $container.children(".breath_light");
            $controlrec.css("display", "none");
            //音频
            var $controlsinAudio = $container.children(".Audio_slider");
            // $controlsinAudio.css("display", "none");
            $controlsinAudio.hide();
            this.audioshow = false;
            //码流
            var $controlsininformation = $container.children(".information");
            $controlsininformation.css("display", "none");
            this.informationshow = false;
            clearInterval(this.timerRunInfo);
            //ptz
            var $controlsptz = $container.children(".liveplay_ptz");
            $controlsptz.css("display", "none");
            this.ptzshow = false;
            this.Audioslider = 0;
            if (document.getElementById(this.h5videoid)) {
                document.getElementById(this.h5videoid).volume=this.Audioslider    
            }
            //麦克风
            if (this.Shoutwheatclass=="icon-voicefill") {
                this.Shoutwheat();
            }
            //转码
            this.videoname="";
            this.valuebutton='';
            var $ptzcontrols = $container.children(".ptzcontrols");
            
            // var valueId=document.getElementById('inputid ')
            // var divId=document.getElementById('divid')
            // console.log(divId)
            $ptzcontrols.css("display", "none");
            $("#"+this.videonameid).removeClass("videoname");
            $("#"+this.inputid).removeClass("streambutton")
            $("#"+this.qualityid).removeClass("quality")
            // $("#"+this.rtcid).removeClass("rtc_new");
            $("#"+this.spanqualityid).addClass("spanquality")
            $("#"+this.inputid).addClass("spanpicturequality")
            if (this.audioback != undefined)
            {
                this.audioback.disconnect();
                delete this.audioback;
                this.audioback = undefined;
            }
            if (this.h5handler != undefined)
            {
                this.h5handler.disconnect();
                delete this.h5handler;
                this.h5handler = undefined;
                if(document.getElementById(this.h5videoid)){
                    $("#" + this.h5videoid).get(0).poster = '';
                    $("#" + this.h5videoid).get(0).load();
                    this.$nextTick(()=>{
                        $("#" + this.h5videoid).remove();
                        $container.prepend(`<video style="width:100%;height:100%;object-fit: fill;" class="h5video" id=${this.h5videoid}  autoplay webkit-playsinline playsinline></video>`);
                    })
                }
            }
            
        },
        //关闭
        CloseVideo(viewparameter,drag=true){
            $('#'+this.canvas2id).hide()
            $('#'+this.warid).hide()
            $("#"+this.h5id).removeClass('h5videoh1');
            this.$root.bus.$emit('close',true)
            this.canvasdisplay=false
            // console.log(viewparameter)
            this.$store.state.videoid = this.videoid+uuid();
            // 关闭实时协议显示
            if (localStorage.getItem(this.$store.state.user)) {
                setTimeout(()=>{
                    var  strLayoutType = JSON.parse(localStorage.getItem(this.$store.state.user))['strLayoutType'];
                    this.$nextTick(()=>{
                        if (strLayoutType=="1|13"||strLayoutType=="4|4"||strLayoutType=="5|5") {
                            this.$nextTick(()=>{
                                setTimeout(()=>{
                                    // $("#"+this.rtcid).siblings('.open_liveplay_butt').css("display", "none");
                                    // $("#"+this.rtcid).addClass('rotate');
                                    $("#"+this.rtcid).parent().parent('.liveplay_butt').css('background-size', '295px 35px');
                                },50)
                            }) 
                        }else if (strLayoutType=="3|3"){
                            this.$nextTick(()=>{
                                setTimeout(()=>{
                                    $("#"+this.rtcid).siblings('.open_liveplay_butt').css("display", "none");
                                    $("#"+this.rtcid).addClass('rotate');
                                    $("#"+this.rtcid).parent().parent('.liveplay_butt').css('background-size', '395px 35px');
                                },25)
                            })
                        }else{
                            this.$nextTick(()=>{
                                setTimeout(()=>{
                                    $("#"+this.rtcid).siblings('.open_liveplay_butt').css("display", "none");
                                    $("#"+this.rtcid).addClass('rotate');
                                    $("#"+this.rtcid).parent().parent('.liveplay_butt').css('background-size', '395px 35px');
                                },25)
                            })
                        }
                    })
                },200)
            }
            if(viewparameter==='close'){
                for(var i=0;i<this.$store.state.liveviewadd.length;i++){
                    if(this.$store.state.liveviewadd[i].strIndex==this.h5id){
                        this.$root.bus.$emit('closeToken',this.$store.state.liveviewadd[i].strToken);
                        if (localStorage.getItem('treeid')) {
                            var treeid =  JSON.parse(localStorage.getItem('treeid'));
                            for (let k = 0; k < treeid.length; k++) {
                                if (this.$store.state.liveviewadd[i].strToken==treeid[k].strToken) {
                                    this.$root.bus.$emit('RootCloseToken',treeid[k].treeid,treeid[k].firstlevel,treeid[k].strToken);
                                    treeid.splice(k, 1);
                                    localStorage.setItem('treeid',JSON.stringify(treeid));
                                }
                            }
                        }else{
                            this.$root.bus.$emit('RootCloseToken',this.$store.state.liveviewadd[i].treeid);
                        }
                        this.$store.state.liveviewadd.splice(i,1)
                        console.log(this.$store.state.liveviewadd)
                    }
                }
            }
            var $container = $("#"+this.h5id);
            // 录像
            var $controlrec = $container.children(".breath_light");
            $controlrec.css("display", "none");
            //音频
            var $controlsinAudio = $container.children(".Audio_slider");
            // $controlsinAudio.css("display", "none");
            $controlsinAudio.hide();
            this.audioshow = false;
            //码流
            var $controlsininformation = $container.children(".information");
            $controlsininformation.css("display", "none");
            this.informationshow = false;
            clearInterval(this.timerRunInfo);
            clearInterval(this.interval);
            //ptz
            var $controlsptz = $container.children(".liveplay_ptz");
            $controlsptz.css("display", "none");
            this.ptzshow = false;
            this.Audioslider = 0;
            if (document.getElementById(this.h5videoid)) {
                document.getElementById(this.h5videoid).volume=this.Audioslider    
            }
            //麦克风
            if (this.Shoutwheatclass=="icon-voicefill") {
                this.Shoutwheat();
            }
            //转码
            this.videoname="";
            this.valuebutton='';
            var $ptzcontrols = $container.children(".ptzcontrols");
            
            // var valueId=document.getElementById('inputid ')
            // var divId=document.getElementById('divid')
            // console.log(divId)
            $ptzcontrols.css("display", "none");
            $("#"+this.videonameid).removeClass("videoname");
            $("#"+this.inputid).removeClass("streambutton")
            $("#"+this.qualityid).removeClass("quality")
            // $("#"+this.rtcid).removeClass("rtc_new");
            $("#"+this.spanqualityid).addClass("spanquality")
            $("#"+this.inputid).addClass("spanpicturequality")
            if (this.audioback != undefined)
            {
                this.audioback.disconnect();
                delete this.audioback;
                this.audioback = undefined;
            }
            if (this.h5handler != undefined)
            {
                this.h5handler.disconnect();
                delete this.h5handler;
                this.h5handler = undefined;
                if(document.getElementById(this.h5videoid)){
                    $("#" + this.h5videoid).get(0).poster = '';
                    $("#" + this.h5videoid).get(0).load();
                    if (drag) {
                        this.$nextTick(()=>{
                            $("#" + this.h5videoid).remove();
                            $container.prepend(`<video style="width:100%;height:100%;object-fit: fill;" class="h5video" id=${this.h5videoid}  autoplay webkit-playsinline playsinline></video>`);
                        })
                    }
                }
            }
        },
        //预置位跳转
        preset_Jump(token){
		   //url
		    var url = this.$store.state.IPPORT + "/api/v1/Ptz?token="+this.tokenshou+"&action=preset&preset="+token+"&speed="+this.Preset_value+"&session="+ this.$store.state.token;
            console.log(url)
            //重组
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    console.log("跳转");
                }
            })


        },
        //预置位设置
        preset_set(token,event){
            var input_val=event.currentTarget.previousElementSibling.previousElementSibling.value;
		   //url
		    var url = this.$store.state.IPPORT + "/api/v1/SetPreset?token="+this.tokenshou+"&presetname="+input_val+"&presettoken="+token+"&session="+ this.$store.state.token;
            console.log(url)
            //重组
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    console.log(result);
                }
            })
            console.log("设置");
        },
        // 二维码
        qrcode () {
            console.log(this.currtoken)
            if(this.currtoken==undefined){
                return false;
            }else{
                var android=this.$store.state.IPPORT + '/single.html?token=' + this.currtoken+"&h5splayer=rtc"+"&session="+ this.$store.state.token;
                var ios= this.$store.state.IPPORT + '/single.html?token=' + this.currtoken+"&h5splayer=rtc"+"&session="+ this.$store.state.token;
            }
            this.$refs.qrcodead.innerHTML="";
            this.$refs.qrcodeios.innerHTML="";
            console.log('android',android)
            console.log('ios',ios)
            var qrcode = new QRCode(this.$refs.qrcodead, {
                width: 100,
                height: 100// 高度
            })
            var qrcode1 = new QRCode(this.$refs.qrcodeios, {
                width: 100,
                height: 100// 高度
            })
            qrcode.clear();
            qrcode1.clear();
            qrcode.makeCode(android);
            qrcode1.makeCode(ios);
        },
        //麦克风
        Shoutwheat(){
            if (this.h5handler == undefined)
            {
                return;
            }
            var tokenshou=this.currtoken
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
        //本地截图
        DoSnapshotWeb(){
            if (this.h5handler == undefined)
            {
                return;
            }
            var fileName = '1';
            const date = new Date();
            fileName = this.currtoken + '_' + date.getFullYear() + '-' + (date.getMonth() + 1)
                         + '-' + date.getDate() + '-' + date.getHours() + '-' + date.getMinutes() + '-' + date.getSeconds();
            console.log(fileName);
            var video = $("#" + this.h5videoid).get(0);
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
        //截图服务端
        DoSnapshot(){
            if (this.h5handler == undefined)
            {
                return;
            }
            let _this =this;
            var snapshot = "token=" + this.currtoken;
            console.log("snapshot cmd", snapshot);

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
        //开始录像
        DoManualRecordStart(){
            if (this.h5handler == undefined)
            {
                return;
            }
            let _this =this;

            var record = "token=" + this.currtoken + "&duration=300";
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
                        if (this.$store.state.Recviewadd[i]==this.currtoken) {
                            this.$store.state.Recviewadd.splice(i,1);
                            i--;
                        }
                    }
                    this.$root.bus.$emit('RootrecToken',this.currtoken);
                    this.$root.bus.$emit('recToken',this.currtoken);
                    this.$store.state.Recviewadd.push(this.currtoken);
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
            var record = "token=" + this.currtoken;
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
                        if(this.$store.state.Recviewadd[i]==this.currtoken){
                            this.$store.state.Recviewadd.splice(i,1)
                            i--;
                            console.log(this.$store.state.Recviewadd)
                        }
                    }
                    this.$root.bus.$emit('RootcloserecToken',this.currtoken);
                    this.$root.bus.$emit('closerecToken',this.currtoken);
                    localStorage.setItem('bRec',JSON.stringify(this.$store.state.Recviewadd));
                }
            }).catch(error => {
                this.$message({
                    message: 'Record failed !',
                    type: 'warning'
                });
            });
        },
        //全屏
        FullScreen(){
            this.canvasdisplay =false
            this.$store.state.FullScreenId ='';
            var elem = $("#"+this.h5id).get(0);
            this.$store.state.FullScreenId = this.h5id;
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
                    console.log("========  updateUIExitFullScreen");
                    this.updateUIExitFullScreen();
                    this.Fullscren='icon-full';
                    this.canvasdisplay = false
                } else {
                     console.log('panelFullScreen3');
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
                    this.Fullscren='icon-suoxiao';
                }
            } else {
                console.log('Fullscreen is not supported on your browser.');
            }
        },
        updateUIExitFullScreen(){
            if (!document.fullscreenElement && !document.webkitIsFullScreen && !document.mozFullScreen && !document.msFullscreenElement)
            {
                $('div[name="flex"]').height(this.contentHeight / this.rows);
            }
        },
        PtzControlShow(event){
            this.Presetdata=[];
            var cors=this.cols*this.rows;
            if(cors>9){
                return false
            }
		   //url
           var url = this.$store.state.IPPORT + "/api/v1/GetPresets?token="+this.tokenshou+"&session="+ this.$store.state.token;
            //重组
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    if(result.bStatus==false){
                        return false;
                    }else{
                        var data=result.data;

                        console.log(data)
                        for(var i = 0; i < data.preset.length; i++){
                            var newItem ={
                                strName : data.preset[i].strName,
                                strToken : data.preset[i].strToken,};
                            this.Presetdata.push(newItem);
                            if(i>8){
                                break;
                            }
                        }
                    }
                }
            })
            var $container = $("#"+this.h5id);
            var $controls = $container.children(".liveplay_ptz");
            if (this.ptzshow == false)
            {
                $controls.css("display", "block");
                this.ptzshow = true;
            }else
            {
                $controls.css("display", "none");
                this.ptzshow = false;
            }
        },

        PtzActionwiperon(event){
            this.PtzAction('wiperon');
        },
        PtzActionwiperoff(event){
            this.PtzAction('wiperoff');
        },
        PtzActionlighton(event){
            this.PtzAction('lighton');
        },
        PtzActionlightoff(event){
            this.PtzAction('lightoff');
        },
        PtzActionfocusing(event){
            this.PtzAction('focusin');
        },
        PtzActionfocusings(event){
            this.PtzAction('focusout');
        },
        PtzActionaperture(event){
            this.PtzAction('irisin');
        },
        PtzActionaperturej(event){
            this.PtzAction('irisout');
        },

        PtzActionZoomIn(event)
        {
            console.log("PtzActionZoomIn");
            this.PtzAction('zoomin');
        },
        PtzActionZoomOut(event)
        {
            this.PtzAction('zoomout');
        },
        PtzActionUpleft(event){
            this.PtzAction('upleft');
        },
        PtzActionUpright(event){
            this.PtzAction('upright');
        },
        PtzActionDownleft(event){
            this.PtzAction('downleft');
        },
        PtzActionDownright(event){
            this.PtzAction('downright');
        },
        PtzActionLeft(event)
        {
            this.PtzAction('left');
        },
        PtzActionRight(event)
        {
            this.PtzAction('right');
        },
        PtzActionUp(event)
        {
            this.PtzAction('up');
        },
        PtzActionDown(event)
        {
            this.PtzAction('down');
        },
        PtzActionStop(event)
        {
            console.log("PtzActionStop");
            this.PtzAction('stop');
        },

        PtzAction(action)
        {
            if (this.h5handler == undefined)
            {
                return;
            }
            let _this =this;
            var ptzcmd = "token=" + this.currtoken + "&action=" + action + "&speed="+this.Preset_value+"";
            console.log("ptzcmd", ptzcmd);

            var url = this.$store.state.IPPORT + "/api/v1/Ptz?" + ptzcmd  + "&session="+ this.$store.state.token;

            this.$http.get(url).then(result => {
                console.log(result);
                if (result.status == 200)
                {

                }
            }).catch(error => {
                console.log('ptz failed!', error);
            });
        },
    }
}
</script>


<style lang="scss" scoped>
    .liveplay{
        width: 100%;
        height: 100%;
        position: relative;
        .h5video{
            object-fit: fill;
            width: 100%;
            height: 100%
        }
        .h5vcanvas{
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%
        }
        &:hover .liveplay_butt{
            display: block;
        }
        // 音频
        .Audio_slider{
            // display: none;
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
        .liveplay_butt{
            position: absolute;
            display: none;
            top: 0;
            z-index:1012;
            width: 100%;
            height: 40px;
            line-height: 30px;
            background: url('~@/views/liveview/imgs/liveview_buttback.png') no-repeat;
            background-size: 395px 35px;
            background-position-x:right;
            text-align: right;
            padding: 0 15px;
            /deep/.open_liveplay_butt{
                display: none;
            }
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
        .liveplay_ptz {
            position:absolute;
            bottom:0;
            background:rgba(255,255,255,0);
            padding:0px;
            box-sizing:content-box;
            width: 100%;
            height: 100%;
            display:none;
            .flex_content{
                width: 100%;
                height: 100%;
                // background: rgba(0,0,0,0.3);
                padding: 8% 0 8% 0;
                position: relative;
                /* margin-bottom: 6%; */
                .content_zoom{
                    width: 50%;
                    height: 100%;
                    display: flex;
                    /* justify-content: space-between; */
                    align-items: flex-end;
                    .key_zoom{
                        width: 25%;
                        margin: 0 4% 0 8%;
                        /* position: absolute;
                        bottom: 4%; */
                        .key_flex{
                            width: 100px;
                            height: 100px;
                            display: flex;
                            flex-wrap: wrap;
                            justify-content: space-between;
                            margin-left: 5%;
                            background:url("~@/views/liveview/imgs/liveview_ptzbutton.png") no-repeat center;
                            /* margin-bottom: 20px; */
                            .key_but{
                                width: 33.33%;
                                height: 33.33%;
                                text-align: center;
                                /* background-color: #FF0000; */
                            }
                        }
                    }
                    .zoom{
                        width: 20px;
                        height: 50px;
                        // background-color: rgba(0, 0, 0, 0.6);
                        display: flex;
                        flex-wrap: wrap;
                        justify-content: space-around;
                        align-content: space-around;
                        margin-left: 40px;
                        margin-bottom: 50px;
                        .zoom_add{
                            width: 20px;
                            height: 20px;
                            text-align: center;
                            background: none;
                            border: 0;
                            padding: 0;
                            color: #FFFFFF;
                        }
                    }
                    
                }
                .zoom_g{
                    width: 20px;
                    height: 100%;
                    position: absolute;
                    left: 0;
                    bottom: 0;
                    background-color: rgba(0, 0, 0, 0.6);
                    display: flex;
                    flex-wrap: wrap;
                    justify-content: space-around;
                    align-content: space-around;
                    // margin-left: 40px
                    padding: 0 2px;
                    ;
                    .zoom_add{
                        width: 20px;
                        height: 20px;
                        text-align: center;
                        background: none;
                        border: 0;
                        padding: 0;
                        color: #FFFFFF;
                        font-size: 14px;
                        &:nth-child(5){
                            color: #3ABBFE;
                            font-size: 18px;
                        }
                        &:nth-child(7){
                            color: #3ABBFE;
                            font-size: 18px;
                        }
                        &:nth-child(6){
                            font-size: 18px;
                        }
                        &:nth-child(8){
                            font-size: 18px;
                        }
                    }
                }
                
                /*  */
                .Preset{
                    width: 30%;
                    position: absolute;
                    bottom: 10%;
                    right: 4%;
                    .block {
                        width: 100%;
                        height: 140px;
                        margin-right: 4%;
                        overflow: auto;
                        color: #FFFFFF;
                        &::-webkit-scrollbar{
                            display: none;
                        }
                        .preset_bgc{
                            width:100%;
                            height:24px;
                            background:rgba(255,255,255,0.2);
                            .preset_input{
                                width: 52%;
                                background:none;
                                /* opacity:0.2; */
                                border-radius:12px;
                                border: 0;
                                padding: 0 0 0 10px;
                                color:rgba(255,255,255,1)!important;
                            }
                            button{
                                width: 15%;
                                background: none;
                                border: 0;
                                font-size: 15px;
                                color: #ffffff;
                                margin-left: 3px;
                            }
                        }
                    }
                }

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

        //主副码流
        .videoname{ 
            position: absolute;
            bottom: 0px;
            font-size:14px;
            background-color: rgba(0,0,0,0.3);
            color: #FFFFFF;
            padding: 8px 24px;
            text-align: center;
            .streambutton{
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
        .quality{
            position: absolute;
            bottom: 5px;
            right: 10px;
            padding:0 10px;
            /* display:none; */
        }
        .spanquality{
            display:none;
        }
        .elqualitytoggle{
            display:block;
        }
        .spanpicturequality{
            display: none !important;
        }
        .el-dropdown{
            .el-dropdown-link {
                color: white;
                .el-dropdown-menu :hover li{
                    background-color: rgb(248, 28, 28)!important;
                }
            }
        }
        //rtc
        .warid{
            color: #fff100;
            font-size: 18px;
            width: 25px;
            height: 25px;
            margin: 10px;
            position: absolute;
            display:none;
            top: 0px;
            left: 0px;
            /* IE10、Firefox and Opera，IE9以及更早的版本不支持 */
            animation-name: breath;       /* 动画名称 */
            animation-duration: 1s;       /* 动画时长3秒 */
            animation-timing-function: ease-in-out;   /* 动画速度曲线：以低速开始和结束 */
            animation-iteration-count: infinite;   /* 播放次数：无限 */
            /* Safari and Chrome */
            -webkit-animation-name: breath;     /* 动画名称 */
            -webkit-animation-duration: 1s;     /* 动画时长3秒 */
            -webkit-animation-timing-function: ease-in-out; /* 动画速度曲线：以低速开始和结束 */
            -webkit-animation-iteration-count: infinite; /* 播放次数：无限 */
        }
        @keyframes breath {
            from { background: #fff100; }       /* 动画开始时的不透明度 */
            /* 50% { background:#ee9531; }       动画50% 时的不透明度 */
            to  { background: none; }       /* 动画结束时的不透明度 */ 
        }
        @-webkit-keyframes breath {
            from { background: #fff100; }       /* 动画开始时的不透明度 */
            // 50% { opacity:  1; }       /* 动画50% 时的不透明度 */
            to  { background: none; }       /* 动画结束时的不透明度 */
        }
    }
    .optionhuazhi{
        /deep/.el-dropdown-menu__item:focus, .el-dropdown-menu__item:not(.is-disabled):hover {
            background-color: #ecf5ff;
            color: #66b1ff !important;
        }
    }
// 录像呼吸灯
.breath_light {
    display:none;
    position: absolute;
    bottom: 45px;
    left: 19px;
    width: 7px; /* 宽度 */
    height: 7px;         /* 高度 */
    border-radius:50%;
    opacity: 0.1;          /* 不透明度 */
    overflow: hidden;         /* 溢出隐藏 */
    background:red;       /* 背景色 */
    //  margin: 25% auto;         /* 外边距 */

    /* IE10、Firefox and Opera，IE9以及更早的版本不支持 */
    animation-name: breath;       /* 动画名称 */
    animation-duration: 2s;       /* 动画时长3秒 */
    animation-timing-function: ease-in-out;   /* 动画速度曲线：以低速开始和结束 */
    animation-iteration-count: infinite;   /* 播放次数：无限 */
    /* Safari and Chrome */
    -webkit-animation-name: breath;     /* 动画名称 */
    -webkit-animation-duration: 2s;     /* 动画时长3秒 */
    -webkit-animation-timing-function: ease-in-out; /* 动画速度曲线：以低速开始和结束 */
    -webkit-animation-iteration-count: infinite; /* 播放次数：无限 */
}
@keyframes breath {
    from { opacity: 0.1; }       /* 动画开始时的不透明度 */
    50% { opacity:  1; }       /* 动画50% 时的不透明度 */
    to  { opacity: 0.1; }       /* 动画结束时的不透明度 */ 
}
@-webkit-keyframes breath {
    from { opacity: 0.1; }       /* 动画开始时的不透明度 */
    50% { opacity:  1; }       /* 动画50% 时的不透明度 */
    to  { opacity: 0.1; }       /* 动画结束时的不透明度 */
}
</style>