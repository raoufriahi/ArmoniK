<template>
    <div class="Aside RelayMode">
        <div class="upperPart">
            <div class="Aside_title">客户端配置</div>
            <div class="Aside_content">
                <div class="Aside_content_part">
                    <div class="Aside_content_top">
                    <div class="Aside_content_title">{{$t("message.live.protocol")}}: {{proto}}</div>
                    <div class="up_you_but">
                        <el-button @click="changeWS" :class="(proto=='WS')?'button_liveview':'button_liveview1'" type="success"  size="mini">WEBSOCKET</el-button>
                        <el-button @click="changeRTC" :class="(proto=='RTC')?'button_liveview':'button_liveview1'"  type="success"  size="mini">WEBRTC</el-button>
                    </div>
                </div>
                <div class="Aside_content_top">
                    <div class="Aside_content_title">{{$t("message.live.protocol1")}}: {{proto1}}</div>
                    <div class="up_you_but">
                        <el-button @click="changeWS1" :class="(proto1=='WS')?'button_replay':'button_replay1'" type="success"  size="mini">WEBSOCKET</el-button>
                        <el-button @click="changeRTC1" :class="(proto1=='RTC')?'button_replay':'button_replay1'"  type="success"  size="mini">WEBRTC</el-button>
                    </div>
                </div>
                </div>
                <div class="Aside_content_part">
                    <div class="Aside_content_buttom">
                    <div class="Aside_content_title">{{$t("message.live.waterprint")}}：</div>
                    <div class="up_you_but">
                        <div>
                            <el-input
                                class="liveview_left_input"
                                placeholder="请您输入水印内容"
                                v-model="watermarkstring">
                            </el-input>
                        </div>
                        <div>
                            <el-button @click="waterprintoff($event)" :class="(watermarktoggle=='block')?'button_watermark':'button_watermark1'" type="success"  size="mini">打开水印</el-button>
                            <el-button @click="waterprintno($event)" :class="(watermarktoggle=='none')?'button_watermark':'button_watermark1'" type="success"  size="mini">关闭水印</el-button>
                        </div>
                    </div>
                </div>
                <div class="Aside_content_buttom" style="padding-top:21px;">
                    <div class="Aside_content_title">{{$t("message.setting.RTCENGINE")}}</div>
                    <div class="up_you_but changeRTC">
                        <el-select v-model="value" style="width:100px" size='mini' placeholder="请选择" @change='Rtcengine'>
                            <el-option
                            v-for="item in options"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                            </el-option>
                        </el-select>
                    </div>
                </div>
                </div>
                <div class="Aside_content_part">
                    <div class="Aside_content_buttom">
                        <div class="Aside_content_title">显示禁用</div>
                        <div class="up_you_but">
                            <div>
                                <el-button @click="deviceprintoff($event)" :class="(devicemarktoggle=='block')?'button_addpv':'button_addpv1'" type="success"  size="mini">显示</el-button>
                                <el-button @click="deviceprintno($event)" :class="(devicemarktoggle=='none')?'button_addpv':'button_addpv1'" type="success"  size="mini">隐藏</el-button>
                            </div>
                        </div>
                    </div>
                    <div class="Aside_content_buttom">
                        <div class="Aside_content_title">画质选择</div>
                        <div class="up_you_but">
                            <div>
                                <el-button @click="elqualityprintoff($event)" :class="(elqualitytoggle=='block')?'button_addpv':'button_addpv1'" type="success"  size="mini">显示</el-button>
                                <el-button @click="elqualityprintno($event)" :class="(elqualitytoggle=='none')?'button_addpv':'button_addpv1'" type="success"  size="mini">隐藏</el-button>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="Aside_content_part">
                    <div class="Aside_content_buttom">
                        <div class="Aside_content_title">AI显示</div>
                        <div class="up_you_but">
                            <div>
                                <el-button @click="Ai_deviceprintoff($event)" :class="(Ai_devicemarktoggle=='block')?'button_addpv':'button_addpv1'" type="success"  size="mini">显示</el-button>
                                <el-button @click="Ai_deviceprintno($event)" :class="(Ai_devicemarktoggle=='none')?'button_addpv':'button_addpv1'" type="success"  size="mini">隐藏</el-button>
                            </div>
                        </div>
                    </div>
                    <div class="Aside_content_buttom">
                        <div class="Aside_content_title1">
                            <span style="margin-right: 8px;">WS解码器:</span>
                            <span>{{websocketDecoder}}</span>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import {H5sGetClientWSDecoder} from '../../../assets/js/h5splayer.js'
export default {
	name: 'TheAside',
	data () {
		return {
			waterprintcolor:true,
            deviceprintcolor:true,
            Ai_deviceprintcolor:true,
            elqualityprintcolor:true,//画质选择
			proto: this.$store.state.liveviewrtc,
			proto1: this.$store.state.liveviewrtc1,
			watermarkstring:this.$store.state.watermarkstring,//水印、
			watermarktoggle:this.$store.state.watermarktoggle,
			devicemarktoggle:"",
			Ai_devicemarktoggle:"",
            elqualitytoggle:'',//画质选择
            options: [{
                value: 'v1',
                label: 'v1'
                }, {
                value: 'v2',
                label: 'v2'
                }],
            value: "v1",
            websocketDecoder:'',
		}
	},
	mounted(){
        this.websocketDecoder = H5sGetClientWSDecoder();
		// localStorage.removeItem('watermarktoggle')
        if(localStorage.getItem('H5sRtcengine')){
            this.value=localStorage.getItem('H5sRtcengine') 
        }
        if(typeof(localStorage.devicemarktoggle) == 'undefined'){
            this.devicemarktoggle='block'
        }else{
            this.devicemarktoggle = localStorage.devicemarktoggle; 
        }
        if(typeof(localStorage.Ai_devicemarktoggle) == 'undefined'){
            this.Ai_devicemarktoggle='none'
        }else{
            this.Ai_devicemarktoggle = localStorage.Ai_devicemarktoggle; 
        }
        // 画质选择
        if(typeof(localStorage.elqualitytoggle) == 'undefined'){
           this.elqualitytoggle='block'
        }else{
            this.elqualitytoggle = localStorage.elqualitytoggle;
        }
	},
	methods:{
        Rtcengine(){
            // if((/Safari/.test(navigator.userAgent) && !/Chrome/.test(navigator.userAgent))){
                //  console.log((/Safari/.test(navigator.userAgent) && !/Chrome/.test(navigator.userAgent)))
                localStorage.setItem("H5sRtcengine",this.value);
            // }else{
            //     this.value="v1",
            //     this.$message({
            //         message: 'v2只支持Safari浏览器',
            //         type: 'warning'
            //     });
            // }
            
        },
		//水印
        waterprintoff(){
			this.waterprintcolor=true
            this.watermarktoggle = "block";
            var watermarktoggle=this.watermarktoggle;
			localStorage.setItem("watermarktoggle",watermarktoggle);
            this.$store.state.watermarkstring=this.watermarkstring
            localStorage.setItem("watermarkstring",this.watermarkstring);

            this.addWaterMarker();
            this.$store.state.watermarktoggle="block";
            // document.getElementById("watermarktoggle").style.display=this.watermarktoggle;
        }, 
        waterprintno(){
            // console.log(this.watermarktoggle);
			this.waterprintcolor=false
            this.watermarktoggle = "none";
            var watermarktoggle=this.watermarktoggle;
            localStorage.setItem("watermarktoggle",watermarktoggle);
            this.$store.state.watermarktoggle="none";
            // document.getElementById("watermarktoggle").style.display=this.watermarktoggle;
        },
        //显示禁用
        deviceprintoff(){
			this.deviceprintcolor=true
            localStorage.devicemarktoggle = "block";
            this.devicemarktoggle = localStorage.devicemarktoggle;
            this.$store.state.devicemarktoggle=localStorage.devicemarktoggle;
        }, 
        Ai_deviceprintoff(){
			this.Ai_deviceprintcolor=true
            localStorage.Ai_devicemarktoggle = "block";
            this.Ai_devicemarktoggle = localStorage.Ai_devicemarktoggle;
            this.$store.state.Ai_devicemarktoggle=localStorage.Ai_devicemarktoggle;
        }, 
        deviceprintno(){
			this.deviceprintcolor=false
            localStorage.devicemarktoggle = "none";
            this.devicemarktoggle = localStorage.devicemarktoggle;
            this.$store.state.devicemarktoggle=localStorage.devicemarktoggle
        },
        Ai_deviceprintno(){
			this.Ai_deviceprintcolor=false
            localStorage.Ai_devicemarktoggle = "none";
            this.Ai_devicemarktoggle = localStorage.Ai_devicemarktoggle;
            this.$store.state.Ai_devicemarktoggle=localStorage.Ai_devicemarktoggle
        },
        // 画质选择
        elqualityprintoff(){
            this.elqualityprintcolor=true
            localStorage.elqualitytoggle = "block";
            this.elqualitytoggle = localStorage.elqualitytoggle;
            this.$store.state.elqualitytoggle=localStorage.elqualitytoggle;
        },
        elqualityprintno(){
            this.elqualityprintcolor=false;
            localStorage.elqualitytoggle = "none";
            this.elqualitytoggle = localStorage.elqualitytoggle;
            this.$store.state.elqualitytoggle=localStorage.elqualitytoggle;
        },
        addWaterMarker(){
            this.$message({
                    message: '水印已打开',
                    type: 'success'
                });
			if(!document.getElementById("watermarktoggle")){
				return
			}
            var date=new Date();
            var Y = date.getFullYear() + '-';
            var M = (date.getMonth()+1 < 10 ? '0'+(date.getMonth()+1) : date.getMonth()+1) + '-';
            var D = date.getDate() + ' ';
            var dates=Y+M+D;
            var watermarkstring= this.watermarkstring;
            // console.log(watermarkstring);

            var can = document.createElement('canvas');
            var body = document.body;
            body.appendChild(can);
            can.width=300; //画布的宽
            can.height=200;//画布的高度
            can.style.display='none';
            var cans = can.getContext('2d');
            cans.rotate(-20*Math.PI/180); //画布里面文字的旋转角度
            cans.font = "16px Microsoft JhengHei"; //画布里面文字的字体
            cans.fillStyle = "rgba(17, 17, 17, 1)";//画布里面文字的颜色
            cans.textAlign = 'left'; //画布里面文字的水平位置
            cans.textBaseline = 'Middle'; //画布里面文字的垂直位置
            cans.fillText(watermarkstring,can.width/3,can.height/2); //画布里面文字的间距比例
            document.getElementById("watermarktoggle").style.backgroundImage="url("+can.toDataURL("image/png")+")";
        },
		//ws rtc
		changeWS(event) {
            this.proto = "WS";
			var proto=this.proto;
			this.$store.state.liveviewrtc=proto
			localStorage.setItem("liveviewrtc",proto);
        },
        changeRTC(event) {
            this.proto = "RTC";
            var proto=this.proto;
            this.$store.state.liveviewrtc=proto
            localStorage.setItem("liveviewrtc",proto);
        },
		changeWS1(event) {
            this.proto1 = "WS";
			var proto1=this.proto1;
			this.$store.state.liveviewrtc1=proto1
			localStorage.setItem("liveviewrtc1",proto1);
        },
        changeRTC1(event) {
            this.proto1 = "RTC";
            var proto1=this.proto1;
            this.$store.state.liveviewrtc1=proto1
            localStorage.setItem("liveviewrtc1",proto1);
        },
	}
}
</script>

<style lang="scss" scoped>
.Aside{
    // padding: 10px 30px;
    .upperPart{
        height:100%;
        width: 40%;
        padding-left: 2%;
        padding-top: 2%;
        padding-bottom: 6%;
        .Aside_title{
            font-size: 18px;
            font-family: PingFang SC;
            font-weight: bold;
            margin-bottom: 20px;
        }
        .Aside_content{
            width: 85%;
            .Aside_content_part{
                display: flex;
                flex-flow: row wrap;
                justify-content: space-between;
            }
            .Aside_content_top{
                width: 50%;
                margin-bottom: 20px;
            }
            .Aside_content_buttom{
                width: 50%;
                margin-bottom: 20px;
            }
            .Aside_content_title{
                font-size: 14px;
                font-family: PingFang SC;
                font-weight: bold;
                // margin-bottom: 15px;
            }
            .Aside_content_title1{
                font-size: 14px;
                font-family: PingFang SC;
                // margin-bottom: 15px;
            }
            .up_you_but{
                margin-bottom: 10px;
                line-height: 35px;
                .liveview_left_input{
                    width: 185px;
                    /deep/ .el-input__inner{
                        text-align:center;
                        height: 28px;
                        font-size: 12px;
                    }
                }
                .button_addpv{
                    font-size: 12px;
                    padding: 5px 7px;
                    background-color: #3ABCFE;
                    border: none;
                }
                .button_addpv1{
                    font-size: 12px;
                    padding: 5px 7px;
                    background:none;
                    border: 1px solid #3ABCFE;
                }
                .button_liveview{
                    font-size: 12px;
                    padding: 5px 14px;
                    background-color: #3ABCFE;
                    border: none;
                }
                .button_liveview1{
                    font-size: 12px;
                    padding: 5px 14px;
                    background:none;
                    border: 1px solid #3ABCFE;
                }
                .button_replay{
                    font-size: 12px;
                    padding: 5px 14px;
                    background-color: #3ABCFE;
                    border: none;
                }
                .button_replay1{
                    font-size: 12px;
                    padding: 5px 14px;
                    background:none;
                    border: 1px solid #3ABCFE;
                }
                .button_watermark{
                    font-size: 12px;
                    padding: 5px 18px;
                    background-color: #3ABCFE;
                    border: none;
                }
                .button_watermark1{
                    font-size: 12px;
                    padding: 5px 18px;
                    background:none;
                    border: 1px solid #3ABCFE;
                }
            }
            .changeRTC{
                /deep/ .el-input{
                    width: 86%;
                }
                /deep/ .el-input__inner{
                    border: 1px solid #3ABCFE;
                    height: 24px;
                    line-height: 24px;
                }
            }
        }
    }
}
</style>
