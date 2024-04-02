<template>
    <div class="RelayMode">
        <div class="Certification upperPart">
            <div>Start Docker</div>
            <div>
                <el-switch
                    @change="Certchange()"
                    v-model="Certification"
                    active-color="#13ce66">
                </el-switch>
            </div>
        </div>
    </div>
</template>

<script>
export default {
	name: 'Docker',
	data(){
        return {
            Certification:''
        }
    },
	mounted(){
        this.Cert()
	},
	methods:{
        Cert(){
            var url = this.$store.state.IPPORT + "/api/v1/GetEnableLinkagent?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    console.log(result)
                    this.Certification=result.data.enable
                }
            })
        },
        Certchange(){
            var url = this.$store.state.IPPORT + "/api/v1/SetEnableLinkagent?enable="+this.Certification+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    console.log(result)
                    this.Cert()
                }
            })
        },
	}
}
</script>

<style lang="scss" scoped>
.Certification{
    // padding: 10px 20px;
    height:100%;
    width: 20%;
    padding-left: 2%;
    padding-top: 2%;
    padding-bottom: 4%;
}
</style>