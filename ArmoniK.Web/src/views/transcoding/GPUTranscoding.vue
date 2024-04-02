<template>
    <div>
        <el-dialog width="33%" :title="title" :visible.sync="dialogFormVisible" :before-close="handleClose">
            <div id="GPURTS" style="padding: 0px 20px 0px; width:100%;height:250px;">
            </div>
            <div id="GPURTSFPS" style="padding: 0px 20px 0px; width:100%;height:250px;">
            </div>
        </el-dialog>
        <el-table
            :data="tableData1.slice((currentPage1-1)*pageSize,currentPage1*pageSize)"
            style="width: 100%">
            <el-table-column prop="uuid" label="Serial number" width="200"></el-table-column>
            <el-table-column prop="nSrcCodec" label="Encoding before transcoding" ></el-table-column>
            <el-table-column prop="nDstCodec" label="Encoding after transcoding" ></el-table-column>
            <el-table-column prop="nSrcresolution" label="Resolution before transcoding"></el-table-column>
            <el-table-column prop="nDstresolution" label="Resolution after transcoding"></el-table-column>
            <el-table-column prop="nEngine" label="GPU/CPU"></el-table-column>
            <el-table-column prop="nEngineIdx" label="GPU number"></el-table-column>
            <el-table-column >
                <template slot="header">
                    <div class="">
                        <el-button type="text" style="cursor:pointer;" size="small" @click="refresh()">Refresh</el-button>
                    </div>
                </template>
                <template slot-scope="scope">
                    <div class="">
                        <el-button type="text" size="small" @click="details(scope.row)">Details</el-button>
                    </div>
                </template>
            </el-table-column>
        </el-table>
        <el-pagination
            style="text-align: center;"
            @size-change="handleSizeChange1"
            @current-change="handleCurrentChange1"
            :current-page="currentPage1"
            :page-size="pageSize"
            layout=" prev, pager, next,total, jumper"
            :total="total2">
        </el-pagination>
    </div>
</template>
<script>
var echarts = require("echarts");
export default {
    data(){
        return{
            dialogFormVisible:false,
            pageSize: 10,//Number of pages
            currentPage1: 1, // current page number
            total2: 0, // total number
            tableData1: [],//1
            nSrcBitrate:this.$t("message.dashboard.nSrcBitrate"),
            nSrcFPS:this.$t("message.dashboard.nSrcFPS"),
            object:{},
            inter:undefined,
            MyChart:undefined,
            MyChart1:undefined,
            title:'',
            timer:null,
            uuidarr:[]//filter duplicate uuid
        }
    },
    mounted(){
        this.$nextTick(()=>{
            this.GPUTranscoding(true);
        })
    },
    methods:{
        refresh(){
            if(this.timer) {
                clearTimeout(this.timer)
                this.timer = null
            }
            this.tableData1 = [];
            this.uuidarr = []
            this.timer = setTimeout(()=>{
                this.GPUTranscoding(true)
            },500)
        },
        GPURST(myChart,list,type){
            // console.log(lengthnv,nvidiadata,l)
            // return false;
            let base = +new Date();
            let date = [];
            let _this=this;
            let oneSecond = 4000;
            for (let i = 1; i < 62; i++) {
                let now = new Date((base += oneSecond));
                date.push(
                    [('0' + now.getSeconds()).slice(-2) + 's']
                )
            }
            // return false;
            // draw a chart
            let titlecol;
            let formatter;
            let color;
            let data;
            if(this.$store.state.darkMode){
                titlecol="#FFFFFF"
            }else{
                titlecol="#000000"
            }
            if (type == 'FPS') {
                formatter="{b0}<br />frame rate:{c0}"
                color = '#5cf4ff';
                data = this.nSrcFPS;
            }else{
                formatter="{b0}<br />code rate:{c0}Kbps";
                color = '#2b74ff';
                data = this.nSrcBitrate;
            }
            let Option={
                tooltip: {
                    trigger: 'axis',
                    position: function(pt) {
                        return [pt[0]-50, '10%']
                    },
                    formatter:formatter
                },
                legend: {
                        data:[data],
                        icon:'rect',
                        itemWidth: 0,
                        itemHeight: 0,
                        textStyle:{
                            fontSize: 12,//font size
                            color: titlecol//font color
                        },
                    },
                xAxis: {
                    type: 'category',
                    boundaryGap: false,
                    data: date,
                    axisLine: {
                        show: false
                    },
                    axisTick: {
                        show: false
                    },
                    splitLine: { //Gridlines
                        "show": false
                    },
                    axisLabel: {
                        show: true,
                        textStyle: {
                            color: titlecol
                        }
                    }
                },
                yAxis: {
                    type: 'value',
                    axisLine: {
                        show: false
                    },
                    splitLine: { //Gridlines
                        "show": false
                    },
                    axisTick: {
                        show: false
                    },
                    axisLabel: {
                        show: true,
                        textStyle: {
                            color: titlecol,
                            // border:' 1px solid #000',
                        }
                    }
                },
                series: [{
                    name: data,
                    type: 'line',
                    smooth: true,
                    symbol: 'none',
                    sampling: 'average',
                    itemStyle: {
                        color: color
                    },
                    areaStyle: {
                        opacity: 0.8,
                        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [{
                                offset: 0,
                                color: color
                            },
                            {
                                offset: 1,
                                color: 'rgba(255,255,255,0.2)'
                            }
                        ])
                    },
                    data: list
                }, 
                ]
            }
            if (type == 'FPS') {
                this.MyChart1 = myChart;
            }else{
                this.MyChart = myChart;
            }
            myChart.setOption(Option);
            
        },
        GPUTranscoding(state,myChart,myChart1,uuid){
            if (state) this.tableData1=[];
            let root =this.$store.state.IPPORT;
            let url = root + "/api/v2/GetGPUTranscoding?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                let itme = result.data.data;
                if(itme.length>0){
                    for(let i=0;i<itme.length;i++){
                        let tabledata={
                            index:i+1,
                            nSrcCodec:itme[i].nSrcCodec,
                            nDstCodec:itme[i].nDstCodec,
                            nDstBitrate:(itme[i].nSrcBitrate/1000).toFixed(1)+'kpbs',
                            nDstFPS:itme[i].nDscFPS,
                            nSrcresolution:itme[i].nSrcWidth +'x'+itme[i].nSrcHeight,
                            nDstresolution:itme[i].nDstWidth +'x'+itme[i].nDstHeight,
                            nEngine:itme[i].nEngine==1?'GPU':'CPU',
                            nEngineIdx:itme[i].nEngineIdx,
                            uuid : itme[i].strSession
                        };
                        if (this.object[itme[i].strSession+'Bitrate']==undefined) this.object[itme[i].strSession+'Bitrate']=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
                        this.object[itme[i].strSession+'Bitrate'].push((itme[i].nSrcBitrate/1000).toFixed(1));
                        if (this.object[itme[i].strSession+'Bitrate'].length>60) {
                            this.object[itme[i].strSession+'Bitrate'].splice(0,1);
                        }
                        if (this.object[itme[i].strSession+'FPS']==undefined) this.object[itme[i].strSession+'FPS']=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
                        this.object[itme[i].strSession+'FPS'].push(itme[i].nSrcFPS);
                        if (this.object[itme[i].strSession+'FPS'].length>60) {
                            this.object[itme[i].strSession+'FPS'].splice(0,1);
                        }
                        if (this.uuidarr.indexOf(itme[i].strSession)==-1) {
                            this.uuidarr.push(itme[i].strSession);
                            if (state) this.tableData1.push(tabledata);
                        }
                    }
                    if (!state) this.GPURST(myChart , this.object[uuid+'Bitrate'] ,'Bitrate');
                    if (!state) this.GPURST(myChart1 , this.object[uuid+'FPS'],'FPS');
                    if (state) this.total2=this.tableData1.length;
                }
            })
        },
        async details(data){
            let uuid = data.uuid;
            console.log(uuid);
            this.dialogFormVisible=true;
            let _this = this
            let myChart
            let myChart1
            this.object = {}
            let pieId = document.getElementById('GPURTS');
            let pieId1 = document.getElementById('GPURTSFPS');
            if (pieId) {
                pieId = document.getElementById('GPURTS');
                pieId1 = document.getElementById('GPURTSFPS');
                myChart = echarts.init(pieId);
                myChart1 = echarts.init(pieId1);
                this.GPUTranscoding(false,myChart,myChart1,uuid);
            }
            while(!pieId){
                await new Promise(resolve => setTimeout(resolve, 100));//One hundred milliseconds delay
                pieId = document.getElementById('GPURTS');
                pieId1 = document.getElementById('GPURTSFPS');
                myChart = echarts.init(pieId);
                myChart1 = echarts.init(pieId1);
                this.GPUTranscoding(false,myChart,myChart1,uuid);
            }
            this.inter = setInterval(()=>{
                myChart = echarts.init(pieId);
                myChart1 = echarts.init(pieId1);
                _this.GPUTranscoding(false,myChart,myChart1,uuid);
            },2000)
        },
        //分页
        handleSizeChange1(val) {
            console.log(`per page ${val} strip`);
            this.currentPage1 = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            console.log(`current page: ${val}`);
            this.currentPage1 = val;
        },
        handleClose(){
            this.dialogFormVisible=false;
            clearInterval(this.inter);
            this.MyChart.clear();
            this.MyChart1.clear();
        },
    },
    beforeDestroy() {
        if (this.inter) {
            clearInterval(this.inter);
            this.MyChart.clear();
            this.MyChart1.clear();
        }
    },
}
</script>
<style lang="scss" scoped>

</style>