<template>
    <div class="Cluster RelayMode">
        <div class="Cluster_block upperPart">
            <div class="Cluster_block_title">HTTP Configuration</div>
            <div class="Cluster_block_content">
                <el-form class="el_form" label-position="top" label-width="100px" size="small" :model="form">
                    <el-form-item label="HTTP Port">
                        <el-input v-model="form.nHTTPPort"></el-input>
                    </el-form-item>
                    <el-form-item label="HTTPS Port">
                        <el-input v-model="form.nHTTPSPort"></el-input>
                    </el-form-item>
                    <el-form-item label="SSL certificate file">
                        <el-input v-model="form.strSSLCertificateFile"></el-input>
                    </el-form-item>
                    <el-form-item :label="label.ForceHTTPS" style="width:174px">
                        <el-switch active-color="#13ce66" v-model="form.bForceHTTPS"></el-switch>
                    </el-form-item>
                    <el-form-item label="Enable multi-port services"  style="width:174px">
                        <el-switch active-color="#13ce66" v-model="form.bEnableMultiServer"></el-switch>
                    </el-form-item>
                    <el-form-item :label="label.DocumentRoot">
                        <el-input v-model="form.strDocumentRoot" @change='Document(form.strDocumentRoot)'></el-input>
                    </el-form-item>
                    <el-form-item label="Service address">
                        <el-input v-model="form.strIPAddress"></el-input>
                    </el-form-item>
                </el-form>
            </div>
        </div>
        <div class="Cluster_but LowerPart">
            <el-button  class="saveBtn" @click="stop">Cancel</el-button>
            <el-button class="save" @click="SetHTTPConf">{{$t("message.camera.save")}}</el-button>
        </div>
    </div>
</template>
<script>
export default {
    name:"Cluster",
    data(){
        return {
            label:{
                open_Close:this.$t("message.camera.open_Close"),
                ForceHTTPS:this.$t("message.setting.ForceHTTPS"),
                EnableDomain:this.$t("message.setting.EnableDomain"),
                DocumentRoot:this.$t("message.setting.DocumentRoot"),
            },
            form: {
                nHTTPPort:"8080",
                nHTTPSPort:"8443",
                strSSLCertificateFile:"server.pem",
                bForceHTTPS:false,
                // bEnableDomain:false,
                bEnableMultiServer:false,
                strDocumentRoot:"www/",
                strIPAddress:"0.0.0.0"
            }
        }
    },
    mounted(){
        this.GetHTTPConf()

    },
    methods:{
        //Judgment cannot be empty
        Document(data){
            console.log(data,this.form.strDocumentRoot)
            if(data==''){
                this.$message({
                    message: 'The file directory cannot be empty',
                    type: 'warning'
                });
            }
        },
        stop(){
            this.GetHTTPConf();
        },
        SetHTTPConf(){
            console.log(this.ruleForm,this.form)
            // return
            if(this.form.strDocumentRoot==''){
                this.$message({
                    message: 'The file directory cannot be empty',
                    type: 'warning'
                });
                return
            }
            var _this=this
            var form=_this.form
            var url = this.$store.state.IPPORT + "/api/v1/SetHTTPConf?http="+form.nHTTPPort+
            "&https="+form.nHTTPSPort+
            "&sslcertificatefile="+form.strSSLCertificateFile+
            "&forcehttps="+form.bForceHTTPS+
            "&enablemultiserver="+form.bEnableMultiServer+
            "&documentroot="+form.strDocumentRoot+"&ip="+form.strIPAddress+"&session="+ this.$store.state.token;

            this.$http.get(url).then(result=>{
                console.log("SetHTTPConf",result);
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
        },
        GetHTTPConf(){
            var _this=this
            var url = this.$store.state.IPPORT + "/api/v1/GetHTTPConf?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log("GetHTTPConf",result);
                if(result.status == 200){
                    this.form=result.data
                }
            })
        },
    }
}
</script>

<style lang="scss" scoped>
.Cluster{
    // padding: 40px;
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
            margin-bottom: 45px;
            font-size: 20px;
            font-family: PingFang SC;
            font-weight: 600;
        }
        .el_form{
            width: 85%; 
            display: flex;
            flex-flow: row wrap;
            justify-content: space-between;
            .el-form-item{
                margin-bottom: 25px !important;
                width: 174px;
            }
        }
    }
    .Cluster_but{
        // text-align: center
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
}
</style>