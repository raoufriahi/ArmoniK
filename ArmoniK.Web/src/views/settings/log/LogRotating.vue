<template>
    <div class="RelayMode">
        <div class="log_log upperPart">
            <div>{{label.Loop}}</div>
            <el-switch
                @change="systemchange"
                v-model="system"
                active-color="#53BCA1"
                inactive-color="#DCDFE6">
            </el-switch>
        </div>
    </div>
</template>

<script>
export default {
    name:'log',
    data(){
        return {
            activeName:'cloudmode',
            label:{
                log:this.$t("message.setting.LogLevel"),
                Loop:this.$t("message.setting.LogRotating")
            },
            value: '',
            system:null,
        }
    },
    mounted(){
        this.systemdata()
    },
    methods:{
        systemchange(){
            var url = this.$store.state.IPPORT + "/api/v1/SetRotatingLog?enable="+this.system+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result)
                if(result.status == 200){
                    this.$message('Successfully modified')
                    this.$root.bus.$emit('webrtc',true);
                    this.$root.bus.$emit('LogData',true);
                }
            })
        },
        systemdata(){
            var url = this.$store.state.IPPORT + "/api/v1/GetRotatingLog?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result)
                if(result.status == 200){
                    this.system=result.data.enable
                }
            })
        },
    }
}
</script>

<style scoped>
.log_log{
    /* display: flex;
    align-items: center; */
    height:100%;
    width: 20%;
    padding-left: 2%;
    padding-top: 2%;
    padding-bottom: 8%;
}
.log_log div:nth-child(1){
    /* margin-right: 10px; */
    margin-bottom: 10px;
}
</style>