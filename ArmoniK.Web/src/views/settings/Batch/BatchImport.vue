<template>
	<div class="BatchImport RelayMode">
        <div class="BatchImport_flex upperPart">
            <div class="BatchImport_title">
                {{lable.Batchimportdevice}}
            </div>
            <el-input placeholder="Please select file" v-model="input">
                <template slot="append">
                    <!-- Button -->
                    <el-upload
                    class="upload"
                    action=""
                    :multiple="false"
                    :show-file-list="false"
                    accept="csv, application/vnd.ms-excel, application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
                    :http-request="httpRequest">
                    <el-button size="small" class="iconfont icon-wenjianjia" type="primary"></el-button>
                    </el-upload>
                </template>
            </el-input>
            <div class="BatchImport_up">
                <a href="/doc/devtemplate.xlsx" download="devtemplate.xlsx" class="waves-effect" > <div>Download file template</div></a>
            </div>
        </div>
        <!-- Button end -->
        <div class="BatchImport_butt LowerPart">
            <el-button class="saveBtn" size="small" type="primary" @click="cancel">{{lable.Cancel}}</el-button>
            <el-button class="saveBtn" size="small" type="primary" @click="Upload">{{lable.Import}}</el-button>
        </div>
     </div>
</template>

<script>
import XLSX from 'xlsx'

export default {
	data () {
		return {
            tableData: [],
            input:'',
            lable:{
                DeviceImport:this.$t("message.setting.DeviceImport"), 
                Import:this.$t("message.setting.Import"),
                Batchimportdevice:this.$t("message.setting.Batchimportdevice"),
                Cancel: this.$t("message.setting.Cancel"),
            },
		}
	},
	methods: {
        //Cancel
        cancel(){
            this.input=''
            this.tableData=[]
        },
        //Upload
        Upload(){
            var root = this.$store.state.IPPORT;
            if(this.tableData.length==0){
                return false
            }else{
                console.log(this.tableData)
                var data=this.tableData
                for(var i=0;i<data.length;i++){
                    // console.log(data[i].Type)
                    var from=data[i]
                    if(data[i].Type=='H5_STREAM'){
                        var url = root + "/api/v1/AddSrcRTSP?name="+encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username)+
                        "&password="+encodeURIComponent(from.Password)+
                        "&audio="+from.Audio+
                        "&url="+encodeURIComponent(from.URL)+
                        "&session="+ this.$store.state.token;
                        console.log('H5_STREAM',from.Type,url)
                        this.platfromyes(url,from.Type);
                    }else if(data[i].Type=='H5_ONVIF'){
                        var url = root + "/api/v1/AddSrcONVIF?name="
                        +encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username)+
                        "&password="+encodeURIComponent(from.Password)+
                        "&audio="+from.Audio+
                        "&ip="+encodeURIComponent(from.IP)+
                        "&port="+encodeURIComponent(from.Port)+
                        "&session="+ this.$store.state.token;
                        this.platfromyes(url,from.Type);
                        console.log('H5_ONVIF',from.Type,url)
                    }else if(data[i].Type=='H5_FILE'){
                        var url = root + "/api/v1/AddSrcFile?name="
                        +encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&url="+encodeURIComponent(from.URL)+
                        "&session="+ this.$store.state.token;
                        console.log('H5_FILE',from.Type,url)

                    }else if(data[i].Type=='H5_DEV_HIK'){
                        var url = root + "/api/v1/AddDeviceHik?name="+encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username)+
                        "&password="+encodeURIComponent(from.Password)+
                        "&ip="+encodeURIComponent(from.IP)+
                        "&port="+encodeURIComponent(from.Port)+
                        "&audio="+from.Audio+
                        "&sandbox=false"+
                        "&session="+ this.$store.state.token;
                        console.log('H5_DEV_HIK',from.Type,url)
                        this.platfromyes(url,from.Type);
                    }else if(data[i].Type=='H5_DEV_DH'){
                        var url = root + "/api/v1/AddDeviceDh?name="+encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username)+
                        "&password="+encodeURIComponent(from.Password)+
                        "&ip="+encodeURIComponent(from.IP)+
                        "&port="+encodeURIComponent(from.Port)+
                        "&audio="+from.Audio+
                        "&sandbox=false"+
                        "&session="+ this.$store.state.token;
                        this.platfromyes(url,from.Type);
                        console.log('H5_DEV_DH',from.Type,url)
                    }else if(data[i].Type=='H5_DEV_HIKISC'){
                        var url = root + "/api/v1/AddDeviceHikISC?name="+encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username_isc)+
                        "&password="+encodeURIComponent(from.Password_isc)+
                        "&ip="+encodeURIComponent(from.IP)+
                        "&port="+encodeURIComponent(from.Port)+
                        "&audio="+from.Audio+
                        "&sandbox=false"+
                        "&session="+ this.$store.state.token;
                        this.platfromyes(url,from.Type);
                        console.log('H5_DEV_HIKISC',from.Type,url)
                    }else if(data[i].Type=='H5_DEV_TD'){
                        var url = root + "/api/v1/AddDeviceTd?name="+encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username)+
                        "&password="+encodeURIComponent(from.Password)+
                        "&ip="+encodeURIComponent(from.IP)+
                        "&port="+encodeURIComponent(from.Port)+
                        "&audio="+from.Audio+
                        "&sandbox=false"+
                        "&session="+ this.$store.state.token;
                        this.platfromyes(url,from.Type);
                        console.log('H5_DEV_TD',from.Type,url)
                    }else if(data[i].Type=='H5_DEV_UNV'){
                        var url = root + "/api/v1/AddDeviceUnv?name="+encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username)+
                        "&password="+encodeURIComponent(from.Password)+
                        "&ip="+encodeURIComponent(from.IP)+
                        "&port="+encodeURIComponent(from.Port)+
                        "&audio="+from.Audio+
                        "&sandbox=false"+
                        "&session="+ this.$store.state.token;
                        this.platfromyes(url,from.Type);
                        console.log('H5_DEV_UNV',from.Type,url)
                    }else if(data[i].Type=='H5_DEV_DHDSS'){
                        var url = root + "/api/v1/AddDeviceDss?name="+encodeURIComponent(from.Name)+
                        "&token="+encodeURIComponent(from.Token)+
                        "&user="+encodeURIComponent(from.Username)+
                        "&password="+encodeURIComponent(from.Password)+
                        "&ip="+encodeURIComponent(from.IP)+
                        "&port="+encodeURIComponent(from.Port)+
                        "&audio="+from.Audio+
                        "&sandbox=false"+
                        "&session="+ this.$store.state.token;
                        this.platfromyes(url,from.Type);
                        console.log('H5_DEV_DHDSS',from.Type,url)
                    }else{
                        console.log(data[i].Type)
                    }
                }
            }
        },
        platfromyes(url,Type){
            var _this=this
            $.ajax({
                type: 'get',
                url: url,  
                async: false,  
                success: function(data){ 
                    console.log("Add people",data)
                    if(data.bStatus==true){
                        console.log('Added successfully',Type)
                    }else{
                        _this.$message({
                            message: 'Add failed'+Type,
                            type: 'warning'
                        });
                        return false;
                    }
                }  
            });
        },
        //Import
	    httpRequest (e) {
            let file = e.file // File information
            console.log('e: ', e)
            console.log('file: ', e.file)
            if (!file) {
                // No file
                return false
            } else if (!/\.(xls|xlsx)$/.test(file.name.toLowerCase())) {
                // Format is defined according to your needs
                this.$message.error('The upload format is incorrect, please upload xls or xlsx format')
                return false
            }
            
            this.input=e.file.name
            const fileReader = new FileReader()
            fileReader.onload = (ev) => {
                try {
                    const data = ev.target.result
                    const workbook = XLSX.read(data, {
                    type: 'binary' // Parse by character encoding
                    })
                    const exlname = workbook.SheetNames[0] // Take the first table
                    const exl = XLSX.utils.sheet_to_json(workbook.Sheets[exlname]) // Generate json table content
                    console.log(exl)
                    // Hang JSON data into data
                    this.tableData = exl
                    console.log(this.tableData)
                    // document.getElementsByName('file')[0].value = '' // According to your own needs, you can reset the upload value to empty, allowing the same file to be uploaded repeatedly
                } catch (e) {
                    console.log('error::')
                    return false
                }
	        }
            fileReader.readAsBinaryString(file)
	    }
    }
}
</script>
<style lang="scss" scoped>
.BatchImport{
    .BatchImport_flex{
        height:100%;
        width: 40%;
        padding-left: 2%;
        padding-top: 2%;
        display:flex;
        flex-direction:column;
        justify-content: flex-start;
        .BatchImport_title{
            font-size: 16px;
            font-family: PingFang SC;
            font-weight: 600;
            margin-bottom: 20px;
        }
        /deep/ .el-input{
            width: 60% !important;
            text-align: right !important;
            margin-bottom: 10px !important;
            .el-input-group__append{
                    .el-button{
                        font-size: 16px;
                    }
                }
        }
        .BatchImport_up{
            margin-bottom: 40px;
            div{
                font-family: PingFang SC;
                font-weight: 600;
                color: #2CA3FB;
                cursor:pointer;
            }
        }
    }
    .BatchImport_butt{
        display: flex;
        justify-content: flex-end;
        width: 40%;
        padding: 15px 0px;
    }
}
</style>