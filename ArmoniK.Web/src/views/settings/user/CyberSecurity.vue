<template>
    <div class="RelayMode">
        <div class="Certification upperPart">
            <div>{{$t("message.setting.Watchvideo")}}</div>
            <el-switch
                @change="anonymoued"
                v-model="anonymous"
                active-color="#13ce66"
                inactive-color="#ff4949">
            </el-switch>
            <div>{{$t("message.setting.Illegal")}}</div>
            <el-switch
                @change="Illegalfun"
                v-model="Illegal"
                active-color="#13ce66"
                inactive-color="#ff4949">
            </el-switch>
        </div>
    </div>
</template>


<script>
export default {
	name: 'Safetyinof',
	data(){
        return {
            Certification:'',
            anonymous:"",
            Illegal:"",
        }
    },
	mounted(){
        this.anonymou();
        this.Illegallook();
	},
	methods:{
         // Whether to browse anonymously
        anonymou(){
            var url1 = this.$store.state.IPPORT + "/api/v1/GetAnonymousView?session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                if(result.status==200){
                    this.anonymous=result.data.bAnonymousView
                }
            })
        },
        anonymoued(){
            var url1 = this.$store.state.IPPORT + "/api/v1/SetAnonymousView?enable="+this.anonymous+"&session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                if(result.status==200){
                    this.anonymou();
                    this.$root.bus.$emit('webrtc',true);
                    this.$root.bus.$emit('Anonymous',true);
                }
            })
        },
        // Whether to browse anonymously
        Illegallook(){
            var url1 = this.$store.state.IPPORT + "/api/v1/GetLoginLockStatus?session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                if(result.status==200){
                    // console.log(result);
                    this.Illegal=result.data.bLoginLockStatus
                }
            })
        },
        Illegalfun(){
            var url1 = this.$store.state.IPPORT + "/api/v1/SetLoginLockStatus?enable="+this.Illegal+"&session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                if(result.status==200){
                    this.Illegallook();
                }
            })
        }
	}
}
</script>

<style lang="scss" scoped>
.Certification{
    height:100%;
    width: 20%;
    padding-left: 2%;
    padding-top: 2%;
    padding-bottom: 2%;
}
</style>