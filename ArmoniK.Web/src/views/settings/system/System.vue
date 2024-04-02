<template>
    <div class="RelayMode">
        <div class="System upperPart">
            <el-button class="system_but"  @click="systemsave">{{label.Snopshot}}</el-button>
            <div class="system_content">
                <div class="system_content_top">
                    <div>{{label.SnapshotList}}</div>
                    <div @click="GetSnapshotList">
                        <i class="iconfont icon-zhongqi"></i>
                        {{label.Refresh}}
                    </div>
                </div>
                <div class="system_content_botm">
                    <div>{{label.Name}}</div>
                    <div class="content" v-for="(a,index) in dataarr" :key="index">
                        {{a}}
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
export default {
    name:'system',
    data(){
        return {
            activeName:'cloudmode',
            dataarr:[],
            label:{
                system:this.$t("message.setting.System"),
                Snopshot:this.$t("message.setting.Snopshot"),
                SnapshotList:this.$t("message.setting.SnapshotList"),
                Name:this.$t("message.setting.Name"),
		        Refresh:this.$t("message.setting.Refresh"),
            },
        }
    },
    mounted(){
        this.GetSnapshotList();
    },
    methods:{
        GetSnapshotList(){
            var url = this.$store.state.IPPORT + "/api/v1/GetSnapshotList?session="+ this.$store.state.token;
            // console.log(url)
            this.$http.get(url).then(result=>{
                console.log(result)
                if(result.status == 200){
                    var itme=result.data;
                    this.dataarr=itme.strName
                    console.log(this.dataarr);
                    if(result.data.strName.length<=0){
                        this.$root.bus.$emit('System',true);
					}else{
                        this.$root.bus.$emit('System',false);
					}
                }
            })
        },
        systemsave(){
            var url = this.$store.state.IPPORT + "/api/v1/CreateSnapshot?session="+ this.$store.state.token;
            // console.log(url)
            this.$http.get(url).then(result=>{
                console.log(result)
                if(result.status == 200){
                    if(result.data.bStatus){
                        this.$message('Successfully saved')
                        this.$root.bus.$emit('System',true);
                    }
                }
            })
        }
    }
}
</script>

<style scoped lang="scss">
.System{
    // padding: 0 10px;
    height:100%;
    width: 35%;
    padding-left: 2%;
    padding-top: 2%;
    padding-bottom: 2%;
}
.system_but{
    // font-size: 14px;
    // background-color: #74C7B2;
    // padding: 5px 18px;
    font-size: 14px;
    padding: 7px 20px;
    background: rgb(58, 188, 254);
    border: 0px none;
    color: white;
    border-radius: 0px;
}
.system_content_top{
    display: flex;
    justify-content: space-between;
    padding-bottom: 5px;
}
// .system_content_top div:nth-child(1){
//     opacity: 0.7;
// }
.system_content_top div:nth-child(2){
    color: #53BCA1;
    cursor:pointer;
}
.system_content_botm{
    width: 100%;
    height: 500px;
    border: 1px solid #BFBFBF;
    overflow: auto;
    padding: 10px 10px;
    font-size: 14px;
    font-family: PingFang SC;
    font-weight: 400;
    // opacity: 0.8;
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
}
.system_content_botm div{
    margin-bottom: 10px;
}
.system_content_botm div:nth-child(1){
    font-size: 15px;
    font-family: PingFang SC;
    font-weight: 600;
}
.system_content_botm .content{
    opacity: 0.8;
}
.system_content{
    width: 400px;
    font-size: 14px;
    font-family: PingFang SC;
    font-weight: 400;
    margin: 20px 0;
}
</style>