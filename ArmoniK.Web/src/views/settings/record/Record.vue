<template>
    <div class="cont RelayMode">
        <div class="upperPart">
            <div class="record_Duration ">
                <div class="title">Video file duration</div>
                <el-input @blur='Durationsetting' class="portcheck_input" style="width:100px" max="240" min="10" v-model="input" type='number'></el-input> (min)
                <div>The minimum length of the video file is 10 minutes and the longest is 240 minutes. Please restart after setting.</div>
            </div>
            <div>
                <div class="ziti" v-if="titleShow">{{$t("message.setting.manualstart")}}</div>
                <div class="ziti" v-else>{{$t("message.setting.manualstop")}}</div> 
                <el-button class="stop" @click="stop()" size="mini">{{$t("message.tour.stop")}}</el-button>
                <el-button class="start" @click="start()" size="mini">{{$t("message.tour.Start")}}</el-button>
            </div>
        </div>
    </div>
</template>   


<script>
export default {
    name: 'record',
    data(){
        return {
            value:false,
            value1:true,
            recordstart:[],
            input:'',
            titleShow:true,
        }
    },
    mounted(){
        this.getsrc();
        this.Duration();
    },
    methods:{
        
        //Recording time
        Durationsetting(){
            console.log(this.input)
            if(this.input<10||this.input>240){
                this.$message('The minimum length of the video file is 10 minutes and the longest is 240 minutes. Please restart after setting.')
                this.Duration();
                return false
            }
            var url = this.$store.state.IPPORT + "/api/v1/SetRecordDuration?duration="+this.input+"&session="+ this.$store.state.token;
            //return false;
            this.$http.get(url).then(result=>{
                console.log(result)
                if(result.status == 200){
                    // this.input=result.data.nRecordDuration
                    this.$message(result.data.strCode)
                    this.$root.bus.$emit('webrtc',true);
                }//wei
            })
        },
        Duration(){
            var url = this.$store.state.IPPORT + "/api/v1/GetRecordDuration?session="+ this.$store.state.token;
            //return false;
            this.$http.get(url).then(result=>{
                console.log(result)
                if(result.status == 200){
                    this.input=result.data.nRecordDuration
                }//wei
            })
        },
        getsrc(){
            var url = this.$store.state.IPPORT + "/api/v1/GetSrc?session="+ this.$store.state.token;
            console.log(url);
            //return false;
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    var data=result.data;
                    for(var i=0;i<data.src.length;i++){
                        var item=data.src[i];
                        var topData={};
                        topData["token"]=item.strToken;
                        //console.log("token",topData);
                        this.recordstart.push(topData);
                        //console.log("token", this.recordstart);
                    }
                }
            })
        },
        StartbRecdot(token){
            var url = this.$store.state.IPPORT + "/api/v1/ManualRecordStart?token="+token+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    const h = this.$createElement;
                    this.$notify({
                    title: 'Enable recording',
                    message: h('i', { style: 'color: teal'}, 'Open successfully')
                    });
                    // this.bRecdot(Token.token);
                    for (let k = 0; k < this.$store.state.Recviewadd.length; k++) {
                        if (this.$store.state.Recviewadd[k]==token) {
                            this.$store.state.Recviewadd.splice(k,1);
                            k--;
                        }
                    }
                    this.$store.state.Recviewadd.push(token);
                    localStorage.setItem('bRec',JSON.stringify(this.$store.state.Recviewadd));
                }
            })
        },
        start(){
            this.titleShow=false,
            this.value1=false
            console.log(this.recordstart);
            for(var i=0;i<this.recordstart.length;i++){
                var Token=this.recordstart[i];
                this.StartbRecdot(Token.token);
            }
        },
        StopbRecdot(token){
            var url = this.$store.state.IPPORT + "/api/v1/ManualRecordStop?token="+token+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    const h = this.$createElement;
                    this.$notify({
                    title: 'Turn off recording',
                    message: h('i', { style: 'color: teal'}, 'Closed successfully')
                    });
                    for(var k=0;k<this.$store.state.Recviewadd.length;k++){
                        if(this.$store.state.Recviewadd[k]==token){
                            this.$store.state.Recviewadd.splice(k,1)
                            k--;
                            console.log(this.$store.state.Recviewadd)
                        }
                    }
                    localStorage.setItem('bRec',JSON.stringify(this.$store.state.Recviewadd));
                }
            })
        },
        stop(){
            this.titleShow=true,
            this.value=false;
            console.log(this.recordstart);
            for(var i=0;i<this.recordstart.length;i++){
                var Token=this.recordstart[i];
                this.StopbRecdot(Token.token);
            }
        }
    }
}
</script>
<style scoped lang="scss">
.cont{
    // padding: 20px;
    .upperPart{
        height:100%;
        width: 40%;
        padding-left: 2%;
        padding-top: 2%;
        padding-bottom: 2%;
        .title{
            margin-bottom: 10px;
        }
        /deep/ .el-input__inner{
            border: 1px solid rgb(58, 188, 254);
        }
    }
    .stop{
        font-size: 14px;
        padding: 6px 20px;
        background: none;
        border: 1px solid rgb(58, 188, 254);
        border-right:0px none;
        // color: white;
        border-radius: 0px;
    }
    .start{
        font-size: 14px;
        padding: 7px 20px;
        background: rgb(58, 188, 254);
        border: 0px none;
        color: white;
        margin-left: 0px;
        border-radius: 0px;
    }
}
.ziti{
    /* color: #000; */
    font-size: 14px;
    margin: 10px 0;
}
</style>