<template>
    <div class="transGPU">
        <div class="transGPU1"> Transcoding Type : 
            <el-radio v-model="radio" label="H5_GPUTYPE_SYSTEM">SYSTEM</el-radio>
            <el-radio v-model="radio" label="H5_GPUTYPE_NVIDIA">NVIDIA</el-radio>
        </div>
        <div class="transGPU2"> Maximum transcoding : 
            <el-input class="transGPU2input" v-model="input" placeholder="Please enter content"></el-input>
        </div>
        <div>
            <br>
            <el-button size="small" @click="save()" type="primary" style="position: absolute;right: 30px;">Save</el-button>
        </div>
    </div>
</template>
<script>
export default {
    data(){
        return{
            input:0,
            radio:'',
            options: [{
                value: 'H5_GPUTYPE_SYSTEM',
                label: 'H5_GPUTYPE_SYSTEM'
            }, {
                value: 'H5_GPUTYPE_NVIDIA',
                label: 'H5_GPUTYPE_NVIDIA'
            }
        ],
        }
    },
    mounted(){
        this.TransGPU();
    },
    methods:{
        TransGPU(){
            var root =this.$store.state.IPPORT;
            var url = root + "/api/v1/GetTransGPUEngineType?session="+ this.$store.state.token;
            var urls = root + "/api/v1/GetTransMaxSession?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status == 200){
                    this.radio = result.data.nType;
                }
            })
            this.$http.get(urls).then(result=>{
                console.log(result);
                if(result.status == 200){
                    this.input = result.data.nSessionMax;
                }
            })

        },
        save(){
            var root =this.$store.state.IPPORT;
            var url = root + "/api/v1/SetTransMaxSession?maxsess="+this.input+"&session="+ this.$store.state.token;
            var urls = root + "/api/v1/SetTransGPUEngineType?type="+this.radio+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status == 200){
                    this.$message({
                        message: 'Please restart after modification',
                        type: 'success'
                    });
                    this.$root.bus.$emit('webrtc',true);
                }
            })
            this.$http.get(urls).then(result=>{
                console.log(result);
                if(result.status == 200){
                }
            })
        },
    }
}
</script>
<style lang='scss' scoped>
    .transGPU{
        margin-left:10px;
        padding: 20px;
        background: #fff;
        width: 550px;
        height: 250px;
        position: relative;
        // display: flex;
        .transGPU1{
            padding: 20px;
        }
        .transGPU2{
            padding: 20px;
            .transGPU2input{
                width: 100px;
            }
        }
    }
</style>