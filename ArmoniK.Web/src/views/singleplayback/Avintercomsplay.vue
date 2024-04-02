<template>
    <div class="Avintercomsplay">
        <video class="videoo1" :id="videoid" autoplay webkit-playsinline playsinline muted="true"></video>
        <!-- <div :id="rtcid" class=""></div> -->
        <div class="singleReplay">
            <div class="singleReplay_left">
                <span style="line-height:60px;margin-left:36%">
                    <i class="iconfont icon-shijian1"></i>
                </span>
            </div>
            <div class="singleReplay_center">
                <span style="line-height:60px;margin-left:38%">
                    <i class="iconfont icon-shangxiajiantou"></i>
                </span>
            </div>
            <div class="singleReplay_right">
                <el-date-picker
                    class="changtime"
                    style="line-height:0px;color:black"
                    v-model="xzvalue"
                    size="mini"
                    @change="startinput_ch"
                    type="datetime"
                    popper-class="date-style"
                    placeholder="选择日期时间"
                    default-time="00:00:00">
                </el-date-picker>
                <el-date-picker
                    class="changtime endTime"
                    style="line-height:0px"
                    v-model="endxzvalue"
                    size="mini"
                    @blur="input_ch"
                    type="datetime"
                    popper-class="date-style"
                    placeholder="选择日期时间"
                    default-time="23:59:59">
                </el-date-picker>
            </div>
        </div>
        <div class="camerafill">
            <button class="iconfont icon-camerafill" @click="DoSnapshotWeb"></button>
        </div>
        <!-- <div class="liveplay_butt">
            <button class="iconfont icon-full" @click="FullScreen"></button>
            <button class="iconfont icon-roundclosefill" @click="CloseVideo"></button>
        </div> -->
    </div>
</template>

<script>

export default {
    name:"Avintercomsplay",
    props:['h5id', 'h5videoid',"cols","rows"],
    data(){
        return{
            videoid: this.h5videoid,
            rtcid:"rtc"+this.h5videoid,
            xzvalue: new Date().getTime() - 3600 * 1000 * 1,//双向日历
            endxzvalue:new Date().getTime(),//双向日历
            timevalues:'',
            timevalues1:'',
            timevaluee:'',
            oldstarttime:'',
            oldendtime:''
        }
    },
    mounted(){
        var _this=this
        this.$root.bus.$on("playback",function(vid){
            // console.log(vid,_this.h5id)
            if(_this.h5id!=vid){
                return
            }
            console.log("vid",vid)
        })
        this.$nextTick(()=>{
            document.getElementsByClassName('el-input__suffix')[0].className += ' iconfont icon-rili1';
            document.getElementsByClassName('el-input__suffix')[1].className += ' iconfont icon-rili1';
        })
        document.body.addEventListener('mousedown', function(){
			_this.$nextTick(()=>{
                var video = $("video")[0];
			    video.muted = false;
            })
        }, false);
    },
    methods:{
        //本地截图
        DoSnapshotWeb(){
            // if (this.h5handler == undefined)
            // {
            //     return;
            // }
            var fileName = '1';
            const date = new Date();
            fileName = this.$route.query.token + '_' + date.getFullYear() + '-' + (date.getMonth() + 1)
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
        startinput_ch(data){
            // var timevalue=this.xzvalue;
            var timevalue='';
            if (data) {
               var timevalue=data; 
            }else{
                var timevalue=this.xzvalue;
            }
            console.log("timevalue11111",timevalue);
            var year = timevalue.getFullYear();
            var month = timevalue.getMonth() + 1;
            var strDate = timevalue.getDate();
            var strDate1 = timevalue.getDate()-1;
            var localOffset = Math.abs(timevalue.getTimezoneOffset() /60);
            var hour = timevalue.getHours();
            var minute = timevalue.getMinutes();
            var second = timevalue.getSeconds();
            if(sessionStorage.Adswitch=="false"){
                if (month<10) {
                    month = '0'+month;
                }
                if (strDate<10) {
                    strDate = '0'+strDate;
                }
                if (strDate1<10) {
                    strDate1 = '0'+strDate1;
                }
                if (hour<10) {
                    hour = '0'+hour;
                }
                if (minute<10) {
                    minute = '0'+minute;
                }
                if (second<10) {
                    second = '0'+second;
                }
            }
            this.timevalues=year+"-"+month+"-"+strDate+"T"+hour+':'+minute+':'+second+"+0"+localOffset+":00";
            this.timevalues1=year+"-"+month+"-"+strDate1+"T"+hour+':'+minute+':'+second+localOffset+":00";
        },
        //点击确定键
		input_ch(){
			// var data=this.Gtoken;
			// console.log("日历",data,this.Gtoken,this.Adswitch);
			// return false;
			// this.Play(data);
            // if (this.$route.query.session) {
				if (this.$route.query.token) {
                    if (!this.timevalues) {
                        this.startinput_ch(new Date(this.xzvalue));
                    }
                    if (this.oldendtime==this.endxzvalue&&this.oldstarttime==this.xzvalue) {
                        return false;
                    }else{
                        this.oldendtime=this.endxzvalue;
                        this.oldstarttime=this.xzvalue;
                    }
                    var timevalue1=this.endxzvalue;
                    var timevalue = new Date(timevalue1);
                    console.log("timevalue11111",timevalue);
                    var year = timevalue.getFullYear();
                    var month = timevalue.getMonth() + 1;
                    var strDate = timevalue.getDate();
                    var strDate1 = timevalue.getDate()-1;
                    var localOffset = Math.abs(timevalue.getTimezoneOffset() /60);
                    var hour = timevalue.getHours();
                    var minute = timevalue.getMinutes();
                    var second = timevalue.getSeconds();
                    if(sessionStorage.Adswitch=="false"){
                        if (month<10) {
                            month = '0'+month;
                        }
                        if (strDate<10) {
                            strDate = '0'+strDate;
                        }
                        if (strDate1<10) {
                            strDate1 = '0'+strDate1;
                        }
                        if (hour<10) {
                            hour = '0'+hour;
                        }
                        if (minute<10) {
                            minute = '0'+minute;
                        }
                        if (second<10) {
                            second = '0'+second;
                        }
                    }
                    this.timevaluee=year+"-"+month+"-"+strDate+"T"+hour+':'+minute+':'+second+"+0"+localOffset+":00";
                    var data = {
                        xzvalue:this.timevalues,
                        token:this.$route.query.token,
                        endxzvalue:this.timevaluee,
                    }
                    this.$root.bus.$emit('SinglePlayBack',data)
					// this.Play(this.$route.query.token)
				}
			// }
		},
        CloseVideo(){

        },
        //全屏
        FullScreen(){
            var elem = $("#"+this.h5id).get(0);
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
    }
}
</script>
<style lang="scss" scoped>
.rtc_new{
    width: 25px;
    height: 25px;
    margin: 10px;
    background:url("~@/views/liveview/imgs/liveview_rtc.svg") no-repeat center;
    background-size: contain;
    background-position:center center;
    position: absolute;
    top: 0px;
}
.Avintercomsplay{
    width: 100%;
    height: 100%;
    position: relative;
    .videoo1{
        width: 100%;
        height: 100%;
        object-fit: fill;
    }
    &:hover .liveplay_butt{
        display: block;
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
            background-size: 320px 35px;
            background-position-x:right;
            text-align: right;
            padding: 0 15px;
            button{
                background: none;
                border: none;
                padding: 0;
                margin: 0;
                color: #FFFFFF;
                margin-left: 15px;
            }
            .fw_butt{
                padding:0;display: inline;
                border: none;
                top: -2px;
                right: -15px;
            }
        }
    .singleReplay{
        display: flex;
        flex-direction: row;
        // width:270px;
        height:60px; 
        position: absolute; 
        top: 80px;  
        right: 20px;
        .singleReplay_left{
            background-color:#2C7BF4;
            height:100%;
            line-height:60px;
            width:67px;
        }
        .singleReplay_center{
            background-color:#FFFFFF;
            height:100%;
            line-height:60px;
            width:50px;
            color:black;
        }
        .singleReplay_right{
            background-color:#FFFFFF;
            height:100%;
            line-height:60px;
            width:205px;
            color:black;
            display: flex; 
            flex-direction:column;
            .changtime{
                width: 100%;
                /deep/ .el-input__prefix{
                    i{
                        display: none !important
                    }
                }
            }
            /deep/.el-input {
                .el-input__inner{
                    color: black !important;
                    border: none !important;
                    border-bottom: 1px solid black;
                    padding-left: 8px !important;
                    font-size: 14px !important;
                }
            }
            .el-input--prefix .el-input__inner{
                padding-left: 0px !important;
            }
            /deep/ .el-input__suffix{
                line-height: 30px;
                display: block !important;
                color: black;
                padding-right: 10px !important;
                i{
                    display: none !important
                }
            }
        }
    }
    .camerafill{
        display: flex;
        flex-direction: row;
        width: 60px;
        height: 50px;
        position: absolute;
        top: 150px;
        right: 20px;
        button{
            width: 60px;
            height: 50px;
            background-color:#2C7BF4;
            color:white;
            border: none !important;
        }
    }
}
</style>