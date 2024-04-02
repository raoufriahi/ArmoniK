<template>
    <div class="database">
        <div class="Cluster_block_content">
            <el-form class=" serveConf el_form" label-position="top" label-width="100px" :model="form">
                <el-form-item label="Type" class="inputSize">
                    <el-select v-model="form.name" placeholder="请选择">
                    <el-option
                        v-for="item in options"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                    </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="Open/Close" class="inputSize">
                    <el-switch v-model="form.bOnline"> </el-switch>
                </el-form-item>
                <el-form-item label="Maximum transcoding" class="inputSize">
                    <el-input v-model="form.nSessionMax"></el-input>
                </el-form-item>
                <!-- lingyige -->
                <el-form-item label="Port"  class="inputSize">
                    <el-input v-model="form.PORT"></el-input>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table" style="padding: 0px 20px 0px; float:right;">
                <el-button class="form_butt1" @click="cancel">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="platformyes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </div>
    </div>
</template>
<script>
import uuid from '../../assets/js/uuid'
  export default {
    name:"GB28181",
    data() {
      return {
        eltitle:this.$t("message.setting.Configuration"),
        form: {
            bOnline:true,
            name:'V1_SW_ONLY',
            nSessionMax:'50',
            PORT:'18000',
        },
        form1: {
            bOnline:true,
            name:'V1_SW_ONLY',
            nSessionMax:'50',
            PORT:'18000',
        },
        options: [{
                value: 'V1_SW_ONLY',
                label: 'V1_SW_ONLY'
            }, {
                value: 'V1_GPU_AUTO',
                label: 'V1_GPU_AUTO'
            }, {
                value: 'V1_GPU_ONLY',
                label: 'V1_GPU_ONLY'
            }
        ],
        edit:false,
      };
    },
    mounted(){
        this.loadDevice();
    },
    methods:{
        cancel(){
            this.loadDevice();
        },
        platformyes(){
            var root =this.$store.state.IPPORT;
            var url = root + "/api/v1/SetAlgoServerConf?enable="+ this.form.bOnline +"&maxsess="+ this.form.nSessionMax +"&engine="+ this.form.name +"&port="+ this.form.PORT +"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status == 200){
                    this.loadDevice();
                    this.$root.bus.$emit('webrtc',true);
                    this.$message('Please restart after modification');
                }
            })
        },
        //Data in the second table
        loadDevice(){
		    var root =this.$store.state.IPPORT;
		   //url
            var url = root + "/api/v1/GetAlgoServerConf?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status == 200){
                    //console.log(result);
                    var itme=result.data;
                    // for(var i=0;i<itme.length;i++){
                        var tabledata={
                            name:itme.nRTSEngine,
                            bOnline:itme.bEnable,
                            PORT:itme.nAlgoServerPort,
                            nSessionMax:itme.nRTSSessionMax
                        };
                        this.form = tabledata;
                        console.log(this.form);
                        this.form1 = tabledata;
                    // }
                }
            })
        },
    }
  };
</script>
<style lang="scss" scoped>
.inputSize{
    padding-right:100px;
    min-width: 300px;
}
</style>