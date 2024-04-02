<template>
  <div class="RelayMode">
    <div class="stora-config upperPart">
      <div class="config_title">存储配置</div>
      <div class="config_input">
        <el-form class="Cluster_form el_form" label-position="left" label-width="100px" :model="form">
          <el-form-item :label="label.configuiport">
            <el-input v-model="form.strNodeName"></el-input>
          </el-form-item>
          <el-form-item :label="label.strNodeName">
            <el-switch active-color="#13ce66" v-model="form.switch"></el-switch>
          </el-form-item>
        </el-form>
      </div>
    </div>
    <div class="submit_title LowerPart">
      <el-button class="saveBtn" @click="configsubmit">保存</el-button>
    </div>
  </div>
</template>
<script>
export default {
  name:"Storagepage",
  data(){
    return{
      label:{
        strNodeName:this.$t("message.setting.bStorageConfigUI"),
        configuiport:this.$t("message.setting.nStorageConfigPort")
      },
      form:{
        strNodeName:'',
        switch:''
      },
    } 
  },
  mounted(){
    this.configdata();
  },
  methods:{
    configdata(){
      var confurl=this.$store.state.IPPORT+"/api/v1/GetStorageConf?session="+this.$store.state.token
      console.log(confurl)
      this.$http.get(confurl).then(res=>{
        console.log(res)
        var storageres=res.data
        if(res.status==200){
          this.form.strNodeName= storageres.nStorageConfigPort
          this.form.switch= storageres.bStorageConfigUI
          localStorage.setItem('strNodeName',JSON.stringify(storageres.nStorageConfigPort)) 
          localStorage.setItem('switchconfig',JSON.stringify(storageres.bStorageConfigUI)) 
        }
      })
    },
    configsubmit(){
      var submiturl=this.$store.state.IPPORT+"/api/v1/SetStorageConf?session="+this.$store.state.token+"&storageui="+this.form.switch+'&storageport='+this.form.strNodeName
      console.log(submiturl)
      this.$http.get(submiturl).then(res=>{
        console.log(res)
        if(res.status==200){
          if(res.data.bStatus){
            this.configdata()
            this.$message({
              message:res.data.strCode,
              type: 'success'
            })
          }
        }
      })
    } 
  }
}
</script>

<style scoped lang="scss">
  .stora-config{
     /* width:33.33%;
     margin:20px auto; */
    height:100%;
    width: 40%;
    padding-left: 2%;
    padding-top: 2%;
    padding-bottom: 4%;
  }
  .config_title{
    font-size: 16px;
    font-family: PingFang SC;
    font-weight: 600;
    margin-bottom: 10px;
  }
  .el-form-item{
    margin-bottom: 0 !important;
    /deep/ .el-input__inner{
      border: 1px solid #DCDFE6 !important;
      border-radius: 0px !important;
      height: 30px !important;
      line-height: 30px !important;
    }
  }
  .Cluster_form{
    /deep/ .el-input{
      width: 50%;
    }
  }
  .submit_title{
    /* text-align: right; */
    width: 40%;
    display: flex;
    justify-content: flex-end;
    padding: 15px 0px;
  }
  .el-button--primary{
    height: 32px;
    line-height: 0;
  }
</style>