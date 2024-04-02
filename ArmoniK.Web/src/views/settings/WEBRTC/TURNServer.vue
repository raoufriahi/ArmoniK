<template>
    <div class="RelayMode">
        <div class="upperPart">
            <el-form class="el_form" ref="form" label-position='top' label-width="100px" size="small " :model="form" style="width: 40%;">
                <el-form-item :label="label.Enable">
                    <el-select v-model="form.bTurnServer" :placeholder="label.pleasechoose">
                        <el-option
                            v-for="item in scaletype"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.IP">
                    <el-input v-model.trim="form.strTurnServerIp"></el-input>
                </el-form-item>
                <el-form-item :label="label.Port">
                    <el-input v-model="form.nTurnServerPort"></el-input>
                </el-form-item>
            </el-form>
        </div>
        <div class="LowerPart">
            <el-button class="saveBtn" @click="CancelTurn">Cancel</el-button>
            <el-button class="saveBtn" @click="addTurn">Save</el-button>
        </div>
    </div>
</template>
<script>
import uuid from '../../../assets/js/uuid'
import '../../../assets/js/jQuery.md5.js'
  export default {
    data() {
      return {
        eltitle:this.$t("message.setting.Configuration"),
        search: '',//search
        activeName: 'cloudmode',
        tabPosition: 'button',//Tab position
        inputedit:[],
        label:{
            Enterkeywords:this.$t("message.table.Enterkeywords"),
            pleasechoose:this.$t("message.table.pleasechoose"),
            CloudMode:this.$t("message.setting.CloudMode"),
            RelayMode:this.$t("message.setting.RelayMode"),
            TURNServer:this.$t("message.setting.TURNServer"),

            PublicIP:this.$t("message.setting.PublicIP"),
            MinPort:this.$t("message.setting.MinPort"),
            MaxPort:this.$t("message.setting.MaxPort"),
            Password:this.$t("message.setting.password"),
            PasswordEncrypt:this.$t("message.setting.PasswordEncrypt"),
            Enable:this.$t("message.setting.Enable"),
            user:this.$t("message.setting.user"),
            Port:this.$t("message.table.Port"),
            IP:this.$t("message.table.IP"),
            Edit:this.$t("message.table.Edit")
        },
        form: {
            bTurnServer:'',
            strTurnServerIp:'',
            nTurnServerPort:'',
        },
        //Pagination
        editform: {
            Type:"cloudmode",
            bCloudMode:"",
            strCloudPublicIp:"",
            iPortRangeMin:"",
            iPortRangeMax:"",

            bRelayMode:"",
            strRelayPublicIp:"",
            nRelayPublicPort:"",
            strRelayUser:"",
            strRelayPasswd:"",
            strCloudPort:"",
            bRelayPasswdEncrypt:"",

            bTurnServer:"",
            strTurnServerIp:"",
            nTurnServerPort:""
        },
        scaletype:[
            {
                value: "true",
                label: "true"
            },{
                value: "false",
                label: "false"
            }
        ],
        pageSize: 10,//Number of pages
        editPopup:false,//Edit popup
        edittoken:"",//The token to delete when editing
        editindex:"",//Index when editing

        tableData3: [],//2
        total3: 0, // total number 2
        currentPage3: 1, // current page 2
        editpasswd:""//Edited password
      };
    },
    mounted(){
        this.loadTurn();
    },
    methods:{
        loadTurn(){
            this.editPopup = false;
            var url = this.$store.state.IPPORT + "/api/v1/GetRTCTurnServerInfo?session="+ this.$store.state.token;
            // console.log(url)
            this.$http.get(url).then(result=>{
                // console.log(result)
                // return false;
                if(result.status == 200){
                    var itme=result.data;
                    var tabledata={
                        bTurnServer: itme.bTurnServer+"",
                        strTurnServerIp:itme.strTurnServerIp,
                        nTurnServerPort:itme.nTurnServerPort
                    };
                    this.form = tabledata;
                    // console.log(tabledata)
                    this.tableData3.push(tabledata);
                    this.total3=this.tableData3.length;
                }
            })
        },
        // Cancel
        CancelTurn(){
            this.loadTurn();
        },
        // Save
        addTurn(){
            let form = this.form;
            var url = this.$store.state.IPPORT + "/api/v1/SetRTCTurnServerInfo?turnserver="+encodeURIComponent(form.bTurnServer)+
            "&turnserverip="+encodeURIComponent(form.strTurnServerIp)+
            "&turnserverport="+encodeURIComponent(form.nTurnServerPort)+
            "&session="+ this.$store.state.token;
            // console.log("Forwardingmode",url);
            // return false
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        // this.editPopup = false;
                        // this.tableData2=[];
                        this.loadTurn();
                        this.$root.bus.$emit('webrtc',true);
                        console.log(result);
                    }else{
                        this.$message({
                            message:"Failed to save" ,
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        // Edit
        handleEdit(index,row){
            
            // console.log(row.bCloudMode);
            // return false
            this.editPopup = true;
            this.editform["bCloudMode"] = row.bCloudMode;
            this.editform["iPortRangeMax"] = row.iPortRangeMax;
            this.editform["iPortRangeMin"] = row.iPortRangeMin;
            this.editform["strCloudPublicIp"] = row.strCloudPublicIp;

            this.editform["bRelayMode"] = row.bRelayMode;
            this.editform["strRelayPublicIp"] = row.strRelayPublicIp;
            this.editform["nRelayPublicPort"] = row.nRelayPublicPort;
            this.editform["strRelayUser"] = row.strRelayUser;
            this.editform["strRelayPasswd"] = row.strRelayPasswd;
            this.editform["bRelayPasswdEncrypt"] = row.bRelayPasswdEncrypt;

            this.editform["bTurnServer"] = row.bTurnServer;
            this.editform["strTurnServerIp"] = row.strTurnServerIp;
            this.editform["nTurnServerPort"] = row.nTurnServerPort;
            // this.editpasswd = row.strPasswd;
            // console.log(this.editform);
            // console.log(this.tableData[index])
        },
        //  Edit OK button
        proedityes(){
            var form=this.editform;
            var url = this.$store.state.IPPORT + "/api/v1/SetRTCTurnServerInfo?turnserver="+encodeURIComponent(form.bTurnServer)+
            "&turnserverip="+encodeURIComponent(form.strTurnServerIp)+
            "&turnserverport="+encodeURIComponent(form.nTurnServerPort)+
            "&session="+ this.$store.state.token;
 
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.editPopup = false;
                        this.tableData3=[];
                        this.loadTurn();
                        this.$root.bus.$emit('webrtc',true);
                    }else{
                        this.$message({
                            message:this.$t("message.setting.Editorfailure") ,
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
            
        },

        selectCall(row){
            console.log("INDEX",row);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    token:row[i].strToken,
                    index:row[i].index,
                    type:row[i].nFPSType
                };
                this.selectop.push(selectop);
            }
            
        },
        select_Call(row){
            console.log(row);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    token:row[i].strToken
                };
                this.selectop.push(selectop);
            }
        },
        //Tab
        removeTab(targetName) {
            this.editform.Type=targetName.name;
            console.log(this.editform.Type);
            if(targetName.name=='cloudmode'){
                this.tableData=[];
                this.loadCloud();
            }else if(targetName.name=='Forwardingmode'){
                this.tableData2=[];
                this.loadRelay();
            }else if(targetName.name=='turnmode'){
                this.tableData3=[];
                this.loadTurn();
            }
        },
        //Pagination
        change (e) {
            this.$forceUpdate()
        },
        handleSizeChange1(val) {
            console.log(`per page ${val} strip`);
            this.currentPage1 = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            console.log(`Current page: ${val}`);
            this.currentPage1 = val;
        },
        handleSizeChange2(val) {
            console.log(`Per page ${val} strip`);
            this.currentPage2 = 1;
            this.pageSize = val;
        },
        handleCurrentChange2(val) {
            console.log(`Current page: ${val}`);
            this.currentPage2 = val;
        },
        handleSizeChange3(val) {
            console.log(`per page ${val} strip`);
            this.currentPage3 = 1;
            this.pageSize = val;
        },
        handleCurrentChange3(val) {
            console.log(`Current page: ${val}`);
            this.currentPage3 = val;
        }
    },
  };
</script>
<style scoped lang="scss">
.RelayMode{
    .upperPart{
        height:100%;
        width: 40%;
        padding-left: 2%;
        padding-top: 2%;
        display:flex;
        flex-direction:column;
        justify-content: flex-start;
        .el_form{
            width: 85%; 
            display: flex;
            flex-flow: row wrap;
            justify-content: space-between;
            .el-form-item{
                /deep/ .el-input{
                    width: 177px;
                }
            }
        }
    }
    .LowerPart{
        display: flex;
        justify-content: flex-end;
        width: 40%;
        padding: 15px 0px;
    }
}
</style>