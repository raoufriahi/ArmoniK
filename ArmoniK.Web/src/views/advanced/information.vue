
<template>
    <div class="Cluster RelayMode">
        <div class="information upperPart" >
            <h3 class='title'>System message</h3>
            <div class="port" style="line-height:32px;margin-bottom: 10px;"><span>{{$t("message.left.WebConsolePort")}}</span><el-input style="width:35%;" v-model="nWebConsolePort"></el-input></div>
            <!-- <div style="line-height:50px; display:flex; justify-content:space-between;"><span>{{$t("message.left.WebConsolePort")}} : &nbsp; </span><el-input style="width:80px;" v-model="nWebConsolePort"></el-input></div> -->
            <div style="line-height:70px; display:flex; width: 35%;justify-content: space-between;"><span>{{$t("message.left.OS")}}：</span><span>{{data.strOSName}}</span></div>
            <div style=" display:flex; width: 35%;justify-content: space-between;">
            <div style=" display:flex;flex-direction:column;flex-start ">
                <span>{{$t("message.left.H5SExtention")}}</span>
                <el-switch
                    v-model="data.bExtSupport"
                    disabled
                    style="padding-top:26px;"
                    active-color="#13ce66"
                    inactive-color="#ff4949">
                </el-switch>
                </div>
            <div style="  display:flex;flex-direction:column;flex-start ">
                <span>{{$t("message.left.WebConsole")}}</span>
                <el-switch
                    v-model="data.bWebConsoleSupport"
                    disabled
                    style="padding-top:26px;"
                    active-color="#13ce66"
                    inactive-color="#ff4949">
                </el-switch>
            </div>
            </div>
            
        </div>
        <div class="LowerPart">
            <el-button  class="saveBtn" @click="stop">Cancel</el-button>
            <el-button class="save" @click="save"> Save</el-button>
        </div>
    </div>
</template>
<script>
//system message
export default {
    data(){
        return{
            data:{},
            nWebConsolePort:'',
        }
    },
    methods:{
        datalist(){
            console.log(this.$store.state.IPPORT);
            let url = this.$store.state.IPPORT + "/api/v1/GetAdvSystemInfo?session="+ this.$store.state.token;
            console.log(url);
            this.$http.get(url).then(result=>{
                if (result.status == 200){
                    this.data =  result.data;
                    this.nWebConsolePort=this.data.nWebConsolePort;
                    this.$store.state.webconsole = this.data.bWebConsoleSupport;
                    console.log(this.data);
                } 
            }).catch((err) =>{
                console.log('GetSrc failed', err);
            })
        },
        stop(){
            this.datalist();
        },
        save(){
            console.log(this.nWebConsolePort);
            let url = this.$store.state.IPPORT + "/api/v1/SetAdvWebConsolePort?session="+ this.$store.state.token+"&port="+this.nWebConsolePort;
            console.log(url);
            this.$http.get(url).then(result=>{
                if (result.status == 200){
                    alert('Successfully saved')
                } 
            }).catch((err) =>{
                console.log('GetSrc failed', err);
            })
        }

	},
	mounted(){
        this.datalist();
	},
}
</script>
<style lang="scss" scoped>
.Cluster{
    padding: 10px 10px;
}
.information{
    height:100%;
    width: 40%;
    padding-left: 2%;
    padding-top: 2%;
    padding-bottom: 4%;
    display:flex;
    flex-direction:column;
    justify-content: flex-start;
    .port{
        display: flex;
        flex-direction:column;
        justify-content: flex-start;
        /deep/ .el-input__inner{
            border-top:0px none;
            border-left: 0px none;
            border-right: 0px none;
            border-bottom: 1px solid #787878;
            border-radius: 0px !important;
        }
    }
    div{
        color: azure;
        font-size: 16px;
    }
    .title{
        padding: 10px 0;
    }
}
.LowerPart{
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
</style>