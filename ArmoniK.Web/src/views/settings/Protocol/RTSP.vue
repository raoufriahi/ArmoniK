<template>
    <div class="RelayMode">
        <div class="Protocol upperPart">
            <div>{{this.$t("message.setting.ServiceDetection")}}</div>
            <div>
                <el-switch
                    @change="Certchange()"
                    v-model="Certification"
                    active-color="#13ce66">
                </el-switch>
            </div>
            <div class="portcheck">
                {{this.$t("message.GB.KeepaliveTime")}} <el-input class="portcheck_input" style="width:80px" v-model="input" type='number'></el-input>(S)
            </div>
        </div>
    </div>
</template>


<script>
export default {
	name: 'Protocol',
	data(){
        return {
            input:'',
            Certification:''
        }
    },
	mounted(){
        this.Cert()
	},
	methods:{
        Cert(){
            var url = this.$store.state.IPPORT + "/api/v1/GetIpPortCheck?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    console.log(result)
                    if(result.status == 200){
                        this.input=result.data.nDetentionPeriod
                        this.Certification=result.data.enable
                    }
                }
            })
        },
        Certchange(){
            var url = this.$store.state.IPPORT + "/api/v1/SetIpPortCheck?enable="+this.Certification+"&period="+this.input+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    console.log(result)
                    this.$message('The modification is successful, please restart。')
                    this.$root.bus.$emit('webrtc',true);
                }
            },response => {
                    this.$message('Fail to edit')
                    this.Cert()
            })
        },
	}
}
</script>

<style lang="scss" scoped>
.Protocol{
    // padding: 10px 20px;
    height:100%;
    width: 20%;
    padding-left: 2%;
    padding-top: 2%;
    padding-bottom: 8%;
     div:nth-child(1){
        margin-bottom: 10px;
    }
    .portcheck{
        // margin-top: 10px;
        .portcheck_input{
            /deep/ .el-input__inner{
                border: 1px solid #DCDFE6 !important;
                border-radius: 0px !important;
                height: 25px !important;
                line-height: 25px !important;
                padding: 0px !important;
            }
        }
    }
}
</style>