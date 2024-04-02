<template>
    <div class="nvidia">
        <div v-for="i in NVIDIAlist" :key="i.id" class="nvidialist">
            <div :id="'ech'+i.index" class="echarts"></div>
            <div :id="'right'+i.index" class="right">
                <ul class="list">
                    <li>序号
                        <br>
                        <br>
                        <div>{{i.index+1}}</div>
                    </li>
                    <li>名字
                        <br>
                        <br>
                        <div>{{i.name}}</div>
                    </li>
                    <li>GPU编号
                        <br>
                        <br>
                        <div>{{i.id}}</div>
                    </li>
                </ul>
            </div>
        </div>
    </div>
</template>
<script>
var echarts = require("echarts");
export default {
    data(){
        return{
            NVIDIAlist:[],
            data1:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data2:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data3:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data4:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data5:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data6:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data7:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data8:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data9:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data10:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data11:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            data12:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
            myChart:{},
            Interval:undefined
        }
    },
    methods:{
        Nvidia(){
			var urls = this.$store.state.IPPORT + "/api/v2/GetNvidiaGPULoading?session="+ this.$store.state.token;
            this.$http.get(urls).then(result=>{
                if (result.status === 200) {
					if (result.data.length!=0) {
                        console.log(result.data);
                        for (let i = 0; i < result.data.length; i++) {
                            let pieId = document.getElementById('ech'+i);
                            // this.myChart['ech'+i]=echarts.init(pieId);
                            this['data'+(3*i+1)].push(Number(result.data[i].mem)>100?100:Number(result.data[i].mem));
                            this['data'+(3*i+2)].push(Number(result.data[i].enc)>100?100:(isNaN(Number(result.data[i].enc))?0:Number(result.data[i].enc)));
                            this['data'+(3*i+3)].push(Number(result.data[i].dec)>100?100:(isNaN(Number(result.data[i].dec))?0:Number(result.data[i].dec)));
                            // this.data.push(uuid(2));
                            // this.data1.push(uuid(2));
                            // this.data2.push(uuid(2));

                            this['data'+(3*i+1)].splice(0, 1);
                            this['data'+(3*i+2)].splice(0, 1);
                            this['data'+(3*i+3)].splice(0, 1);
                            let myChart=echarts.init(pieId);
                            this.GPURST(myChart,this['data'+(3*i+1)],this['data'+(3*i+2)],this['data'+(3*i+3)]);
                            // this.GPURST(myChart['ech'+i]);
                        }
					}
                }
            })
		},
        Nvidialists(){
			var url = this.$store.state.IPPORT + "/api/v2/GetNvidiaGPUList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.status === 200) {
                    console.log(result);
					if (result.data.length!=0) {  
						this.NVIDIAlist = result.data;
					}
                }
            })
        },
        GPURST(myChart,data1,data2,data3){
            console.log(1111);
            var base = +new Date();
            var date = [];
            var _this=this;
            var oneSecond = 1000;
            for (var i = 1; i < 37; i++) {
                var now = new Date((base += oneSecond));
                date.push(
                    [('0' + now.getSeconds()).slice(-2) + 's']
                )
            }
            // return false;
            // 绘制图表
            var titlecol
            if(this.$store.state.darkMode){
                titlecol="#FFFFFF"
            }else{
                titlecol="#000000"
            }
            var Option={
                tooltip: {
                    trigger: 'axis',
                    position: function(pt) {
                        return [pt[0], '10%']
                    },
                    formatter:"{b0}<br />{a0}:{c0}%<br />{a1}:{c1}%<br />{a2}:{c2}%"
                },
                title: {
                    left: 'center',
                    text: "nvidiadata[l].strName",
                    show:false,
                },
                legend: {
                        data:['MEM','ENC','DEC'],
                        icon:'rect',
                        itemWidth: 50,
                        itemHeight: 2,
                        textStyle:{
                            fontSize: 12,//字体大小
                            color: titlecol//字体颜色
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
                    splitLine: { //网格线
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
                    max:'100',
                    axisLine: {
                        show: false
                    },
                    splitLine: { //网格线
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
                    name: 'MEM',
                    type: 'line',
                    smooth: true,
                    symbol: 'none',
                    sampling: 'average',
                    itemStyle: {
                        color: '#52aff9'
                    },
                    areaStyle: {
                        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [{
                                offset: 0.5,
                                color: '#52aff9',
                            },
                            {
                                offset: 1,
                                color: 'rgba(255,255,255,0.2)'
                            }
                        ])
                    },
                    data: data1
                }, {
                    name: 'ENC',
                    type: 'line',
                    smooth: true,
                    symbol: 'none',
                    sampling: 'average',
                    itemStyle: {
                        color: '#7ff5de'
                    },
                    areaStyle: {
                        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [{
                                offset: 0.5,
                                color: '#7ff5de',
                            },
                            {
                                offset: 1,
                                color: 'rgba(255,255,255,0.2)'
                            }
                        ])
                    },
                    data: data2
                }, {
                    name: 'DEC',
                    type: 'line',
                    smooth: true,
                    symbol: 'none',
                    sampling: 'average',
                    itemStyle: {
                        color: '#bb43f6'
                    },
                    areaStyle: {
                        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [{
                                offset: 0.5,
                                color: '#bb43f6',
                            },
                            {
                                offset: 1,
                                color: 'rgba(255,255,255,0.2)'
                            }
                        ])
                    },
                    data: data3
                }]
            }
            // myChart.clear();
            myChart.setOption(Option)
        },
    },
    mounted(){
        let _this = this
        this.Nvidialists();
        this.Nvidia()
        this.Interval = setInterval(()=>{
            _this.Nvidia()
        },2000)
        // this.Nvidia()
    },
    beforeDestroy(){
        clearInterval(this.Interval);
    }
    
}
</script>
<style lang="scss" scoped>
.nvidia{
    height: 100%;
    padding-left: 9px;
    overflow: auto;
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
    .nvidialist{
        width: 85%;
        height: 49.2%;
        margin-bottom: 10px;
        display: flex;
        &:nth-last-child(1){
            margin-bottom:0px !important;
        }
        .echarts{
                padding: 20px 20px 0px;
                width:85%;
                height:90%;
        }
        .right{
            width: 14%;
            height: 90%;
            margin-top: 20px;
        }
        .list{
            li{
                padding-top: 30px;
            }
        }
    }
}
</style>