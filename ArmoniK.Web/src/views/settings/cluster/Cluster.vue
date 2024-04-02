<template>
    <div class="Cluster RelayMode">
        <div class="Cluster_block upperPart">
            <div class="Cluster_block_title">Cluster management</div>
            <div class="Cluster_block_content">
                <el-form class="Cluster_form el_form" label-position="top" label-width="100px" :model="form">
                    <el-form-item :label="label.NodeName">
                        <el-input v-model="form.strNodeName"></el-input>
                    </el-form-item>
                    <el-form-item :label="label.NodeID">
                        <el-input v-model="form.strNodeId"></el-input>
                    </el-form-item>
                    <el-form-item :label="label.RedisServerIP">
                        <el-input v-model="form.strServerIp"></el-input>
                    </el-form-item>
                    <el-form-item :label="label.LocalIP">
                        <el-input v-model="form.strNodeIp"></el-input>
                    </el-form-item>
                    <el-form-item :label="label.RedisServerPort">
                        <el-input v-model="form.nServerPort"></el-input>
                    </el-form-item>
                    <el-form-item label="SSL Protocol" style="width: 174px;">
                        <el-switch active-color="#13ce66" v-model="ruleForm.ssl"></el-switch>
                    </el-form-item>
                </el-form>
            </div>
            <div class="upperPart_bottom">
                <div class="upperPart_bottom_content">
                    <div class="Cluster_block_content_bottom">
                        <div class="Cluster_block_title">Connection Status</div>
                        <div class="Cluster_block_content">
                            <div class="lianjie">Connection Status: <span>{{status}}</span></div>
                        </div>
                    </div>
                    <div class="Cluster_block_content_bottom">
                        <div class="Cluster_block_title">Cluster switch</div>
                        <div class="Cluster_block_content">
                            <el-form class="Cluster_form" label-position="left" label-width="100px" :model="ruleForm">
                                <el-form-item :label="label.open_Close">
                                    <el-switch active-color="#13ce66" v-model="ruleForm.switch"></el-switch>
                                </el-form-item>
                            </el-form>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="Cluster_but LowerPart">
            <el-button class="saveBtn" @click="stop">Cancel</el-button>
            <el-button class="save" @click="SetRedis">{{$t("message.camera.save")}}</el-button>
        </div>
    </div>
</template>
<script>
export default {
    name:"Cluster",
    data(){
        return {
            status:'',
            // editPopup:false,
            label:{
                NodeName:this.$t("message.cluster.NodeName"),
                NodeID:this.$t("message.cluster.NodeID"),
                RedisServerIP:this.$t("message.cluster.RedisServerIP"),
                RedisServerPort:this.$t("message.cluster.RedisServerPort"),
                LocalIP:this.$t("message.cluster.LocalIP"),
                open_Close:this.$t("message.camera.open_Close"),
            },
            form: {
                strServerIp:"192.168.100.162",
                strNodeId:"node1",
                strNodeIp:"192.168.100.145",
                strNodeName:"node1",
                nServerPort:"6379",
            },
            ruleForm:{
                switch:null,
                ssl:null
            },

        }
    },
    mounted(){
        this.GetRedisConf()
        this.GetEnableRedis()
        this.Redis()
    },
    methods:{
        Redis(){
            var url = this.$store.state.IPPORT+"/api/v1/ConnectRedisStatus?session="+this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.data.bOnline){
                    this.status='Online'
                }else{
                    this.status='Offline'
                }
            })
        },
        // Cancel
        stop(){
            this.GetRedisConf();
            this.GetEnableRedis();
            this.Redis();
        },
        SetRedis(){
            console.log(this.ruleForm,this.form)
            // return
            var _this=this
            var form=_this.form
            var url = this.$store.state.IPPORT + "/api/v1/SetRedisConf?serverport="
            +form.nServerPort+"&nodeid="
            +form.strNodeId+"&nodename="
            +form.strNodeName+"&nodeip="
            +form.strNodeIp+"&serverip="
            +form.strServerIp+"&session="+ this.$store.state.token
            +'&ssl='+this.ruleForm.ssl;
            console.log(url ,111111111111);
            var url1 = this.$store.state.IPPORT + "/api/v1/SetEnableRedis?enable="+_this.ruleForm.switch+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log("SetRedisConf",result);
                if(result.status == 200){
                    if(result.data.bStatus){
                        this.$root.bus.$emit('webrtc',true);
                        this.$message({
                            message: result.data.strCode,
                            type: 'success'
                        });
                    }else{
                        this.$message({
                            message: result.data.strCode,
                            type: 'warning'
                        });
                    }
                }
            })
            this.$http.get(url1).then(result=>{
                console.log("SetEnableRedis",result);
                if(result.status == 200){
                    if(result.data.bStatus){
                        this.$root.bus.$emit('webrtc',true);
                        // this.editPopup=true
                        this.$message({
                            message: result.data.strCode,
                            type: 'success'
                        });
                    }else{
                        this.$message({
                            message: result.data.strCode,
                            type: 'warning'
                        });
                    }
                }
            })
            this.Redis();
        },
        GetRedisConf(){
            var _this=this
            var url = this.$store.state.IPPORT + "/api/v1/GetRedisConf?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log("GetRedisConf",result);
                if(result.status == 200){
                    this.form=result.data
					this.ruleForm["ssl"]=result.data.bSSL
                }
            })
        },
        GetEnableRedis(){
            var url = this.$store.state.IPPORT + "/api/v1/GetEnableRedis?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log("GetEnableRedis",result);
                if(result.status == 200){
                    this.ruleForm["switch"]=result.data.enable
                }
            })
        },
    }
}
</script>

<style lang="scss" scoped>
.Cluster{
    // padding: 40px;
    .Cluster_dialog{
        text-align: center;
        margin-bottom: 20px;
    }
    .Cluster_block{
        // margin-bottom: 40px;
        height:100%;
        width: 40%;
        padding-left: 2%;
        padding-top: 2%;
        padding-bottom: 4%;
        display:flex;
        flex-direction:column;
        justify-content: flex-start;
        .Cluster_block_title{
            margin-bottom: 20px;
            font-size: 18px;
            font-family: PingFang SC;
            font-weight: 600;
        }
        .Cluster_block_content{
            // padding: 20px 40px;
            .el_form{
                width: 85%; 
                display: flex;
                flex-flow: row wrap;
                justify-content: space-between;
                .el-form-item{
                    margin-bottom: 25px !important;
                }
            }
        }
    }
    .upperPart_bottom{
        height:100%;
        // padding-left: 2%;
        padding-top: 2%;
        padding-bottom: 4%;
        .upperPart_bottom_content{
            width: 85%; 
            display: flex;
            flex-flow: row wrap;
            justify-content: space-between !important;
            .Cluster_block_title{
                margin-bottom: 20px;
                font-size: 18px;
                font-family: PingFang SC;
                font-weight: 600;
            }
            .Cluster_block_content_bottom{
                width: 174px;
            }
        }
    }
    .Cluster_but{
        // text-align: center;
        display: flex;
        justify-content: flex-end;
        width: 40%;
        padding: 15px 0px;
        .save{
            font-size: 14px;
            padding: 7px 20px;
            background: rgb(58, 188, 254);
            border: 0px none;
            color: white;
        }
    }
    .lianjie{
        // color:#fff;
        font-size:14px;
        span{
            padding-left:35px;
            color: #b7b7b7;
        }
    }
}
</style>