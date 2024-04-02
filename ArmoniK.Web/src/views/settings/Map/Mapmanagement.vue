<template>
    <div class="Map_manag">
        <div class="Map_title">GIS map data source</div>
        <div class="Map_edi">
            <!-- <CButton class="form_butt" type="submit">百度地图</CButton> -->
            <CButton :class="[Mapback=='Gothe'?'Map_butts':'']" class="Map_but" @click="gaode" type="submit">Gaode Map</CButton>
            <CButton :class="[Mapback=='Google'?'Map_butts':'']" class="Map_but1" @click="guge" type="submit">Google Map</CButton>
            <!-- <CButton class="form_butt1" @click="tiandi" type="submit">天地地图</CButton> -->
        </div>
    </div>
</template>
<script>
export default {
    name:'Mapmanagement',
    data(){
        return {
            Mapback:null
        }
    },
    mounted(){
        var Mapurl=JSON.parse(this.$store.state.Mapurl);
            console.log(Mapurl)
        if(Mapurl){
            this.Mapback=Mapurl[0].name
            console.log(Mapurl[0].name);
        }else(
            $('.Map_but').addClass('Map_butts')
        )
    },
    methods:{
        gaode(){
            var viewdata=[{
                name:"Gothe",
                url:"http://wprd0{1-4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&style=7&x={x}&y={y}&z={z}"
            }]
            var viewjson=JSON.stringify(viewdata)
            console.log(viewjson)
            this.$store.state.Mapurl=viewjson
            localStorage.Mapurl=viewjson
            console.log(localStorage.getItem('Mapurl'))
            $('.Map_but').addClass('Map_butts')
            $('.Map_but1').removeClass('Map_butts')
        },
        guge(){
            var viewdata=[{
                name:"Google",
                url:"http://mt2.google.cn/vt/lyrs=m@167000000&hl=zh-CN&gl=cn&x={x}&y={y}&z={z}"
            }]
            var viewjson=JSON.stringify(viewdata)
            console.log(viewjson)
            this.$store.state.Mapurl=viewjson
            localStorage.Mapurl=viewjson
            console.log(localStorage.getItem('Mapurl'))
            $('.Map_but1').addClass('Map_butts')
            $('.Map_but').removeClass('Map_butts')
        },
        tiandi(){
            var viewdata=[{
                name:"World",
                url:"http://t0.tianditu.com/DataServer?T=vec_w&x={x}&y={y}&l={z}&tk=093859ee97e903db5b5636801ce54f9e"
            },{
                 name:"World",
                url:"http://t0.tianditu.com/DataServer?T=cva_w&x={x}&y={y}&l={z}&tk=093859ee97e903db5b5636801ce54f9e"
            }]
            var viewjson=JSON.stringify(viewdata)
            console.log(viewjson)
            this.$store.state.Mapurl=viewjson
            localStorage.Mapurl=viewjson
            console.log(localStorage.getItem('Mapurl'))
            
        },
    }
}
</script>
<style lang="scss" scoped>
.Map_manag{
    padding: 0 10px;
    .Map_title{
        font-size: 18px;
        font-family: PingFang SC;
        font-weight: 600;
        margin-bottom: 10px;
    }
    .Map_edi{
        button{
            height: 40px;
            box-sizing: border-box;
            font-size: 16px;
            line-height: 25px;
            border-radius: 4px;
            border:1px solid rgba(58, 187, 254, 1) !important;
            margin-right: 10px;
            &:hover{
                background-color: rgba(58, 187, 254, 1);
            }
            
        }
        .Map_butts{
            background-color: rgba(58, 187, 254, 1);
        }
    }
}
</style>