<template  class="intel">
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
            myChart:{},
            Interval:undefined
        }
    },
    methods:{
        Nvidia(){
			var urls = this.$store.state.IPPORT + "/api/v2/GetLinuxIntelGPULoading?session="+ this.$store.state.token;
            this.$http.get(urls).then(result=>{
                console.log(result);
                if (result.status === 200) {
					if (result.data.length!=0) {
                        console.log(result.data); 
                        if (result.data) {
                            let i = 0;
                            let pieId = document.getElementById('ech'+i);
                            this['data1'].push(parseInt(result.data[1].engines['Render/3D/0'].busy)>100?100:parseInt(result.data[1].engines['Render/3D/0'].busy));
                            this['data2'].push(parseInt(result.data[1].engines['Video/0'].busy)>100?100:parseInt(result.data[1].engines['Video/0'].busy));
                            this['data1'].splice(0, 1);
                            this['data2'].splice(0, 1);
                            let myChart=echarts.init(pieId);
                            if (result.data[1].engines['Video/1']) {
                                this['data3'].push(parseInt(result.data[1].engines['Video/1'].busy)>100?100:parseInt(result.data[1].engines['Video/1'].busy));
                                this['data3'].splice(0, 1);
                                this.GPURST(myChart,this.data1,this.data2,this.data3);
                            }else{
                                this.GPURST(myChart,this.data1,this.data2);
                            }
                        }
					}
                }
            })
		},
        Nvidialists(){
			var url = this.$store.state.IPPORT + "/api/v2/GetLinuxIntelGPUList?session="+ this.$store.state.token;
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
            if (data3) {
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
                            data:['Render/3D/0','video/0','video/1'],
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
                        name: 'Render/3D/0',
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
                        name: 'video/0',
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
                        name: 'video/1',
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
            }else{
               var Option={
                    tooltip: {
                        trigger: 'axis',
                        position: function(pt) {
                            return [pt[0], '10%']
                        },
                        formatter:"{b0}<br />{a0}:{c0}%<br />{a1}:{c1}%"
                    },
                    title: {
                        left: 'center',
                        text: "nvidiadata[l].strName",
                        show:false,
                    },
                    legend: {
                            data:['Render/3D/0','video/0'],
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
                        name: 'Render/3D/0',
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
                        name: 'video/0',
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
                    }]
                } 
            }
            // myChart.clear();
            myChart.setOption(Option)
        },
    },
    mounted(){
        let _this = this
        this.Nvidialists();
        this.Interval = setInterval(()=>{
            _this.Nvidia()
        },2000)
        this.Nvidia()
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