<template>
    <div class=liveview>
        <div id="watermarktoggle"></div>
		<!-- 右边视频栏 -->
      	<div class="liveview_right" id="videoPanel">
            <div id="video_hed">
                <div name='flex' style="position: relative;" class="videoColor" v-for="r in rows" :key="r">
                    <div
                        class="palace" 
                        name="flex" 
                        v-for="c in cols" 
                        @contextmenu.prevent="stopVideo($event)" 
                        @click="videoClick(r,c,$event)" :key="c">
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
		</div>
    </div>
</template>
<script>
import Vue from 'vue'
import liveplay from './layout/Liveplay'
import uuid from '../assets/js/uuid'
export default {
    name: 'Liveview',
    components: {
        'v-liveplay': liveplay
    },
	data(){
		return{
            viewlists:{},
            cur_query:this.$route.query || {},
            viewdata:[],//视图数据
            proto: this.$store.state.liveviewrtc,
			rows: 0,
			cols: 0,
			selectCol: 1,
			selectRow: 1,
			contentHeight: '',
            contentWidth: '',
            data:[],
            objlist:{},
            tokens:{},
            data22:['h11','h12','h21','h22'],
            data33:['h11','h12','h13','h21','h22','h23','h31','h32','h33'],
            data44:['h11','h12','h13','h14','h21','h22','h23','h24','h31','h32','h33','h34','h41','h42','h43','h44'],
            data55:['h11','h12','h13','h14','h15','h21','h22','h23','h24','h25','h31','h32','h33','h34','h35','h41','h42','h43','h44','h45','h51','h52','h53','h54','h55'],
            data6:['h11','h14','h15','h12','h13','h16','h17'],
            data8:['h11','h12','h13','h14','h15','h16','h17','h19','h18','h110','h111','h112','h113'],
            vdata:[],
		}
    },
	mounted(){
        this.viewlist();
        $('.c-header').hide();
        setTimeout(() => {
            this.viewplay();
            this.updateUI(1);
            this.addWaterMarker();
            this.licenseWaterMark(); 
        }, 1000);
        this.$nextTick(()=>{
            this.srcview();
        })
	},
	methods:{
        stopVideo(event){
            return;
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
        //水印
        addWaterMarker(){
            console.log(1111111111111111111111111111111111111111);
            console.log(document.getElementById("watermarktoggle"));
			if(!document.getElementById("watermarktoggle")){
				return
			}
            console.log(111111111111111);
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
            can.width=300; //画布的宽
            can.height=200;//画布的高度
            can.style.display='none';
            var cans = can.getContext('2d');
            cans.rotate(-20*Math.PI/140); //画布里面文字的旋转角度
            cans.font = "16px Microsoft JhengHei"; //画布里面文字的字体
            cans.fillStyle = "rgba(17, 17, 17, 1)";//画布里面文字的颜色
            cans.textAlign = 'left'; //画布里面文字的水平位置
            cans.textBaseline = 'Middle'; //画布里面文字的垂直位置
            cans.fillText(watermarkstring,can.width/3,can.height/2); //画布里面文字的间距比例
            console.log("url("+can.toDataURL("image/png")+")");
            document.getElementById("watermarktoggle").style.backgroundImage="url("+can.toDataURL("image/png")+")";
        },
        viewplay(){
            let data = this.cur_query.view;
            if(this.viewlists[data]){
                console.log(this.viewlists[data]);
                this.viewClick(this.viewlists[data]);
            }
        },
        viewlist(){
            this.vdata=[];
            var strview='';
            let query=this.$route.query;
            console.log(query.type)
            if (!query.type||query.type=='undefined') {
                return;
            }else{
                if (query.type==1) {
                    strview='1|1'
                }else if (query.type==2) {
                    strview='1|3'
                }else if (query.type==3) {
                    strview='2|2'
                }else if (query.type==4) {
                    strview='1|4'
                }else if (query.type==5) {
                    strview='1|6'
                }else if (query.type==6) {
                    strview='1|7'
                }else if (query.type==7) {
                    strview='3|3'
                }else if (query.type==8) {
                    strview='1|13'
                }else if (query.type==9) {
                    strview='4|4'
                }else{
                    return;
                }
            }
            
            this.changePanel(strview,'viewClick')
            var url = this.$store.state.IPPORT + "/api/v1/GetSrc"+"?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.status === 200) {
                    console.log(result.data.src);
                    for (let i = 0; i < result.data.src.length; i++) {
                        this.objlist[result.data.src[i].strToken]=result.data.src[i].strName
                    }
                }
                for (let i = 1; i < 26; i++) {
                    this.tokens[i]=query[i];
                }
                for (const k in this.tokens) {
                    if (this.tokens[k]!=undefined) {
                        if (query.type==3) {
                            var data={
                                strIndex:this.data22[k-1]||'h11',
                                strToken: this.tokens[k],
                                strStream: query.stream||"main",
                                strName: this.objlist[this.tokens[k]]
                            }
                            this.vdata.push(data);
                        }else if (query.type==7) {
                            var data={
                                strIndex:this.data33[k-1]||'h11',
                                strToken: this.tokens[k],
                                strStream: query.stream||"main",
                                strName: this.objlist[this.tokens[k]]
                            }
                            this.vdata.push(data);
                        }else if (query.type==9) {
                            var data={
                                strIndex:this.data44[k-1]||'h11',
                                strToken: this.tokens[k],
                                strStream: query.stream||"main",
                                strName: this.objlist[this.tokens[k]]
                            }
                            this.vdata.push(data);
                        }else if (query.type==6) {
                            var data={
                                strIndex:this.data6[k-1]||'h11',
                                strToken: this.tokens[k],
                                strStream: query.stream||"main",
                                strName: this.objlist[this.tokens[k]]
                            }
                            this.vdata.push(data);
                        }else if (query.type==8) {
                            var data={
                                strIndex:this.data8[k-1]||'h11',
                                strToken: this.tokens[k],
                                strStream: query.stream||"main",
                                strName: this.objlist[this.tokens[k]]
                            }
                            this.vdata.push(data);
                        }else{
                            var vid = 'h1'+k;
                            var data={
                                strIndex:vid||'h11',
                                strToken: this.tokens[k],
                                strStream: query.stream||"main",
                                strName: this.objlist[this.tokens[k]]
                            }
                            this.vdata.push(data);
                            
                        }
                    }
                }
                var listdata={
                    strLayoutType: strview,
                    src:this.vdata,
                    icon: "iconfont icon-tubiao_huaban1fuben51",
                }
                this.viewClick(listdata);
            })
        },
        //播放视图
        async viewClick(data){
            console.log(data)
            this.$store.state.liveviewadd=[]
            var _this=this
            await this.changePanel(data,'viewClick')
            if(data.src.length==0){
                var vdata={
                    viewparameter:'viewparameter'
                }
                _this.$store.state.layoutplay=vdata
                return
            }
            for(var i=0;i<data.src.length;i++){
                console.log(this.objlist,this.objlist[data.src[i].strToken],data.src[i].strToken)
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
                _this.$store.state.layoutplay=vdata
                await _this.$store.state.liveviewadd.push(viewdata)
            }
        },
        // 'http://localhost:8080/api/v1/AddView?session=59733292-eda0-42dd-b676-ed71c7a664cf'
        srcview(){
            console.log(this.$store.state.token);
            var url = this.$store.state.IPPORT + "/api/v1/GetView?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if (result.status === 200) {
                    // console.log(result)
                    var oldarr=result.data.conf;
                    var oldarr1=result.data.src;
                    
                    for(var l in oldarr){
                        var dataroot=this.getview(oldarr[l],oldarr1);
                        this.viewdata.push(dataroot);
                        this.viewlists[dataroot.strName]=dataroot
                    }
                    console.log(this.viewlists);
                    // console.log(this.viewlists.indexOf(cur_query['view']));
                    console.log(this.viewdata)
                }
            })
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
		//点击宫格
		videoClick(r, c, $event) {
            this.selectCol = c;
            this.selectRow = r;
            console.log(r, c);
            $(".h5container").removeClass('h5videoh');
            $("#h"+r+c).addClass('h5videoh');
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
            $(document.body).css('overflow-y','hidden');
        },
		//点击宫格
        changePanel(event,viewClick) {
            console.log(viewClick,event)
            var data=''
            if(viewClick=='viewClick'){
                data=event.strLayoutType||event
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
    },
    computed: {
        watchvideo(){
            return this.$route.query
        },
    },
    //监听器
    watch: {
        watchvideo(data){
            this.viewlist();
        },
    }
    
}
</script>

<style lang="scss" scoped>
.liveview{
    width: 100vw;
    height: 90vh;
    // padding-top: 10px;
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
    .liveview_right{
        width: 100%;
        height: 100%;
        /* 播放片段 */
        .h5videoh{
            /* border: 2px solid #f44336 !important;
            box-sizing: border-box; */
            box-shadow: 0 0 0 2px #f44336;
            // box-sizing: border-box;
            z-index: 10;
        }
        #video_hed{
            height: 100%;
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
            display: none;
            justify-content: space-between;
            button{
                border: none;
                background: none;
                font-size: 30px;
            }
        }
    }
}
</style>