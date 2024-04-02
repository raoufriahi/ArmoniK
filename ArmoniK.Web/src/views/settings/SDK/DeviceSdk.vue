<template>
    <div>
        <!-- Edit popup -->
        
        <el-dialog width="18%" :title="eltitle" :visible.sync="editPopup">
            <el-form class="el_form" ref="form" label-position='left' label-width="200px" size="small " :model="editform">
                    
                <el-form-item :label="label.Type">
                    <el-select v-model="editform.Type" placeholder="Please choose">
                        <el-option
                            v-for="item in options"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.Name" class="inputSize">
                    <el-input v-model="editform.Name"></el-input>
                </el-form-item>
                <el-form-item :label="label.Token" class="inputSize">
                    <el-input v-model="editform.Token"></el-input>
                </el-form-item>
                <el-form-item :label="label.User" v-if="editform.Type!='H5_DEV_HIKISC'&&editform.Type!='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="editform.User"></el-input>
                </el-form-item>
                <el-form-item :label="label.password" v-if="editform.Type!='H5_DEV_HIKISC'&&editform.Type!='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="editform.Password"></el-input>
                </el-form-item>
                <!-- lingyige -->
                <el-form-item label="AppKey "  v-if="editform.Type=='H5_DEV_HIKISC'" class="inputSize">
                    <el-input v-model="editform.User" class="inputSize"></el-input>
                </el-form-item>
                <el-form-item label="AppSecret"  v-if="editform.Type=='H5_DEV_HIKISC'" class="inputSize">
                    <el-input v-model="editform.Password"></el-input>
                </el-form-item>

                <el-form-item label="AppKey "  v-if="editform.Type=='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="editform.User"></el-input>
                </el-form-item>
                <el-form-item label="AppSecret"  v-if="editform.Type=='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="editform.Password"></el-input>
                </el-form-item>

                <el-form-item :label="label.IP" class="inputSize">
                    <el-input v-model="editform.IP"></el-input>
                </el-form-item>
                <el-form-item :label="label.Port" class="inputSize">
                    <el-input v-model="editform.Port"></el-input>
                </el-form-item>
                <el-form style="display: flex; flex-direction: row;">
                    <el-form-item :label="label.Audio" style="display: flex; flex-direction: row;" class="audio">
                        <el-switch
                        v-model="editform.Audio">
                        </el-switch>
                    </el-form-item>
                    <el-form-item :label="label.Sandbox" style="display: flex; flex-direction: row;margin-left: 42px;"  class="audio">
                        <el-switch
                        v-model="editform.bSandbox">
                        </el-switch>
                    </el-form-item>
                </el-form>
                <el-form-item v-if="editform.Type=='H5_DEV_HIK'" label="ISAPI">
                    <el-switch
                    v-model="editform.ISAPI">
                    </el-switch>
                </el-form-item>
                <el-form-item v-if="editform.Type=='H5_DEV_UNV'" label="VMS">
                    <el-switch
                    v-model="editform.VMS">
                    </el-switch>
                </el-form-item>
                <!-- Edit the number of channels -->
                <el-form-item :label="label.Maxichannels" class="inputSize">
                    <el-input v-model="editform.Maxichannels" ></el-input>
                    <p class="maxnum">({{$t('message.setting.Maxinit')}})</p>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table" style="padding: 0px 20px 0px;">
                <el-button class="form_butt1" @click="editPopup = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="edityes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <el-dialog width="30%" :title="eltitle" :visible.sync="dialogFormVisible">
            <el-form class="el_form" ref="form" label-position='left' label-width="200px" size="small " :model="form">
            
                <el-form-item :label="label.Type">
                    <el-select v-model="form.Type" placeholder="Please choose">
                    <el-option
                        v-for="item in options"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                    </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.Name" class="inputSize">
                    <el-input v-model="form.Name"></el-input>
                </el-form-item>
                <el-form-item :label="label.Token" class="inputSize">
                    <el-input v-model="form.Token"></el-input>
                </el-form-item>
                <el-form-item :label="label.username" v-if="form.Type!='H5_DEV_HIKISC'&&form.Type!='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="form.Username"></el-input>
                </el-form-item>
                <el-form-item :label="label.password" v-if="form.Type!='H5_DEV_HIKISC'&&form.Type!='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="form.Password"></el-input>
                </el-form-item>
                <!-- lingyige -->
                <el-form-item label="AppKey "  v-if="form.Type=='H5_DEV_HIKISC'" class="inputSize">
                    <el-input v-model="form.Username_isc"></el-input>
                </el-form-item>
                <el-form-item label="AppSecret"  v-if="form.Type=='H5_DEV_HIKISC'" class="inputSize">
                    <el-input v-model="form.Password_isc"></el-input>
                </el-form-item>

                <el-form-item label="AppKey "  v-if="form.Type=='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="form.Username_INFOV"></el-input>
                </el-form-item>
                <el-form-item label="AppSecret"  v-if="form.Type=='H5_DEV_HIKINFOV'" class="inputSize">
                    <el-input v-model="form.Password_INFOV"></el-input>
                </el-form-item>

                <el-form-item :label="label.IP" class="inputSize">
                    <el-input v-model="form.IP"></el-input>
                </el-form-item>
                <el-form-item :label="label.Port" class="inputSize">
                    <el-input v-if="form.Type=='V1_DEV_HIK'" v-model="form.Port"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_DH'" v-model="form.Port_dh"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_HIKISC'" v-model="form.Port_isc"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_TD'" v-model="form.Port_td"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_UNV'" v-model="form.Port_unv"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_DHDSS'" v-model="form.Port_DSS"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_IVS'" v-model="form.Port_1800"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_HIKINFOV'" v-model="form.Port_INFOV"></el-input>
                    <el-input v-if="form.Type=='V1_DEV_VIKOR'" v-model="form.Port_vikor"></el-input>
                </el-form-item>
                <el-form style="display: flex; flex-direction: row;">
                    <el-form-item :label="label.Audio" style="display: flex; flex-direction: row;" class="audio">
                        <el-switch
                        v-model="form.Audio">
                        </el-switch>
                    </el-form-item>
                    <el-form-item :label="label.Sandbox" style="display: flex; flex-direction: row;margin-left: 42px;" class="audio">
                        <el-switch
                        v-model="form.bSandbox">
                        </el-switch>
                    </el-form-item>
                </el-form>
                <el-form-item v-if="form.Type=='H5_DEV_HIK'" label="ISAPI">
                    <el-switch
                    v-model="form.ISAPI">
                    </el-switch>
                </el-form-item>
                <el-form-item v-if="form.Type=='H5_DEV_UNV'" label="VMS">
                    <el-switch
                    v-model="form.VMS">
                    </el-switch>
                </el-form-item>
                 <!-- Add the number of channels -->
                 <el-form-item :label="label.Maxichannels" class="inputSize">
                    <el-input v-model="form.Maxichannels" ></el-input>
                    <p class="maxnum">({{$t('message.setting.Maxinit')}})</p>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table" style="padding: 0px 20px 0px;">
                <el-button class="form_butt1" @click="dialogFormVisible = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="platformyes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <el-dialog width="25%" :title="eltitle" :visible.sync="settingSandbox">
            <el-form class="el_form" ref="form" label-position='left' size="small " :model="settingform">
            
                <el-form-item :label="label.Sandbox">
                    <el-select v-model="settingform.nSandboxMode" placeholder="Please choose">
                    <el-option
                        v-for="item in opnSandboxMode"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                    </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.LoadAllChannel">
                    <el-switch
                        v-model="settingform.bEnableLoadAllChannel">
                    </el-switch>
                </el-form-item>
                
            </el-form>
            <div slot="footer" class="dialog-footer button_table">
                <el-button class="form_butt1" @click="settingSandbox = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="SetDeviceConf">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <!-- Button -->
        <div class="sdk_button">
            <div class="button_edi">
                <CButton class="form_butt" @click="addto" type="submit">Add</CButton>
                <CButton class="form_butt1" @click="deleteselect" type="submit">Delete</CButton>
            </div>
            <div class="sdk_setting">
                <CButton type="submit"  @click="settingto"><i class="iconfont icon-shezhi" ></i> {{$t("message.setting.setting")}}</CButton>
            </div>
        </div>
       
        <el-table
            :data="tableData.filter(data => !search || data.Name.toLowerCase().includes(search.toLowerCase())).slice((currentPage1-1)*pageSize,currentPage1*pageSize)"
            @select='selectCall'
            @select-all='select_Call'
            style="width: 200%" 
            v-loading="loading"
            element-loading-text="Editing, please do not switch pages"
            element-loading-background="rgba(0, 0, 0, 0)">
            <el-table-column
                type="selection"
                width="55">
            </el-table-column>
            <el-table-column
                prop="index"
                :label="label.Index"
                width="100">
            </el-table-column>
            <el-table-column
            prop="Name"
            :label="label.Name">
            </el-table-column>
            <el-table-column
            prop="IP"
            label="IP"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="Port"
            :label="label.Port">
            </el-table-column>
            <el-table-column
            prop="User"
            :label="label.User">
            </el-table-column>
            <el-table-column
            prop="Online"
            :label="label.Online">
            </el-table-column>
            <el-table-column
            prop="Type"
            :label="label.Type"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="Token"
            :label="label.Token">
            </el-table-column>
            <el-table-column
                fixed="right"
                min-width="140">
                <template slot="header" slot-scope="scope">
                    <el-input
                    v-model="search"
                    size="mini"
                    @change="handlechange(scope.$index,scope.row)"
                    placeholder="Enter keywords"/>
                </template>
                <template slot-scope="scope">
                    <el-button @click="handleEdit(scope.$index,scope.row)" type="text" size="small">{{$t("message.setting.edit")}}</el-button>
                    <el-button @click.native.prevent="deleteRow(scope.$index,scope.row, tableData)" type="text" size="small">{{$t("message.setting.DeleteAll")}}</el-button>
                </template>
            </el-table-column>
        </el-table>
        <!-- Pagination -->
        <el-pagination
            style="text-align: center;"
            layout=" prev, pager, next,total, jumper"
            @size-change="handleSizeChange1" 
            @current-change="handleCurrentChange1"
            :current-page="currentPage1"
            :total="total2">
        </el-pagination>
        
    </div>
</template>
<script>
import uuid from '../../../assets/js/uuid'
  export default {
    name:"GB",
    data() {
      return {
        eltitle:this.$t("message.setting.Configuration"),
        search: '',//Search
        activeName: 'first',
        tabPosition: 'button',//Tab position
        inputedit:[],
        label:{
            label:"H5_DEV",//Pick 1

            LoadAllChannel:this.$t("message.setting.LoadAllChannel"),
            Sandbox:this.$t("message.setting.Sandbox"),
            Maxichannels:this.$t("message.setting.Maxichannels"),
            
            Index:this.$t("message.table.Index"),
            Name:this.$t("message.table.Name"),
            IP:this.$t("message.table.IP"),
            Port:this.$t("message.table.Port"),
            User:this.$t("message.table.User"),
            Online:this.$t("message.table.Online"),
            Type:this.$t("message.table.Type"),
            Audio:this.$t("message.setting.Audio"),
            username:this.$t("message.setting.username"),
            password:this.$t("message.setting.password"),
            IP:this.$t("message.table.IP"),
            Token:this.$t("message.table.Token")
        },
        options: [{
                value: 'V1_DEV_HIK',
                label: 'V1_DEV_HIK'
            }, {
                value: 'V1_DEV_IVS',
                label: 'V1_DEV_IVS'
            }, {
                value: 'V1_DEV_DH',
                label: 'V1_DEV_DH'
            }
            , {
                value: 'V1_DEV_HIKISC',
                label: 'V1_DEV_HIKISC'
            }, {
                value: 'V1_DEV_HIKINFOV',
                label: 'V1_DEV_HIKINFOV'
            },{
                value: 'V1_DEV_TD',
                label: 'V1_DEV_TD'
            }, {
                value: 'V1_DEV_UNV',
                label: 'V1_DEV_UNV'
            }, {
                value: 'V1_DEV_DHDSS',
                label: 'V1_DEV_DHDSS'
            }
            // ,{
            //     value: 'H5_DEV_VIKOR',
            //     label: 'H5_DEV_VIKOR'
            // }
        ],
        //pagination
        pageSize: 10,//Number of pages
        currentPage1: 1, // Current page number
        total2: 0, // Total number 1
        dialogFormVisible: false,//Add popup
        editPopup:false,//Edit popup
        settingSandbox:false,//Set popup
        settingform:{
            bEnableLoadAllChannel: true,
            nSandboxMode: "V1_SANDBOX_AUTO"
        },
        opnSandboxMode: [{
                value: 'V1_SANDBOX_AUTO',
                label: 'V1_SANDBOX_AUTO'
            },{
                value: 'V1_SANDBOX_ALL',
                label: 'V1_SANDBOX_ALL'
            }, {
                value: 'V1_SANDBOX_DISABLE',
                label: 'V1_SANDBOX_DISABLE'
            }
        ],
        form: {
            Type: 'V1_DEV_HIK',
            Name:"Device1",
            Token:"platform1",
            Username:"admin",
            Username_isc:"22936233",
            Username_INFOV:"22936233",
            Password:"admin12345",
            Password_isc:"px50TzrNNUiU1uxloJLG",
            Password_INFOV:"px50TzrNNUiU1uxloJLG",
            IP:"192.168.1.1",
            Port:"8000",
            Port_dh:"37777",
            Port_isc:"443",
            Port_td:"3000",
            Port_unv:"80",
            Port_DSS:"9000",
            Port_1800:"18531",
            Port_INFOV:'443',
            Port_vikor:'20000',
            Audio:false,
            bSandbox: false,
            Maxichannels:'0',
            ISAPI:false,
            VMS:false,
        },
        editform: {
            Type: 'V1_DEV_HIK',
            Name:"Device1",
            Token:"platform1",
            User:"admin",
            Username_isc:"22936233",
            Username_INFOV:"22936233",
            Password:"admin12345",
            Password_isc:"px50TzrNNUiU1uxloJLG",
            Password_INFOV:"px50TzrNNUiU1uxloJLG",
            IP:"192.168.1.1",
            Port:"8000",
            Port_dh:"37777",
            Port_isc:"443",
            Port_td:"3000",
            Port_unv:"80",
            Port_DSS:"9000",
            Port_1800:"18531",
            Port_INFOV:'443',
            Port_vikor:'20000',
            Audio:false,
            bSandbox:false,
            Maxichannels:'0',
            ISAPI:false,
            VMS:false,
        },
        loading: false,
        edittoken:"",//The token to delete when editing
        editindex:"",//Index when editing
        tableData: [],//1
        selectop:[]//choose those
      };
    },
    mounted(){
        this.loadHIK();
        this.GetDeviceConf();
    },
    methods:{
        settingto(){
            this.settingSandbox=true
            this.GetDeviceConf();
        },
        //Get sandbox data
        GetDeviceConf(){
            var url = this.$store.state.IPPORT + "/api/v1/GetDeviceConf?session="+ this.$store.state.token;
            //   console.log("***********************",url)
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    this.settingform.bEnableLoadAllChannel=result.data.bEnableLoadAllChannel
                    this.settingform.nSandboxMode=result.data.nSandboxMode
                }
            })
        },
        //set sandbox data
        SetDeviceConf(){
            var form=this.settingform;
            var url = this.$store.state.IPPORT + "/api/v1/SetDeviceConf?loadallchannel="+form.bEnableLoadAllChannel+"&sandboxmode="+form.nSandboxMode+"&session="+ this.$store.state.token;
            //   console.log("***********************",url)
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    if(result.data.bStatus){
                        this.$message({
                            message: 'Set successfully',
                            type: 'success'
                        });
                    }
                }
            })
        },
        //The data of the first table
        loadHIK(){
          //url
          var url = this.$store.state.IPPORT + "/api/v1/GetDevice?getonline=true&session="+ this.$store.state.token;
          //   console.log("***********************",url)
            this.$http.get(url).then(result=>{
            console.log(result)
              if(result.status == 200){
                  var itme=result.data.dev;
                  this.tableData=[];
                  console.log(result);
                  for(var i=0;i<itme.length;i++){
                      var tabledata={
                          index:i+1,
                          Type:itme[i].nType,
                          Name:itme[i].strName,
                          Token:itme[i].strToken,
                          User:itme[i].strUser,
                          Password:itme[i].strPasswd,
                          IP:itme[i].strDevIpAddress,
                          Port:itme[i].strDevPort,
                          Audio :itme[i].bEnableAudio,
                          Online:itme[i].bOnline+"",
                          bPasswdEncrypt:itme[i].bPasswdEncrypt,
                          bSandbox: itme[i].bSandbox,
                          Maxchannel:itme[i].nMaxChannel,
                          bISAPI:itme[i].bISAPI,
                          bVMS:itme[i].bVMS
                      };
                      this.tableData.push(tabledata);
                      //console.log(tabledata);
                  }
                  this.total2=this.tableData.length;
              }
            })
        },
        
        //Randomly get token data when you click Add
        addto(){
            this.dialogFormVisible=true;
            this.form["Token"] = uuid(4, 16).toLowerCase();
        },
        //Edited successfully
        Success(){
            
            var form=this.editform;
            // console.log(this.editindex,form.Type);
            // return false;
            //url
            if(form.Type=="V1_DEV_HIK"){
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceHik?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&isapi="+form.ISAPI+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                      if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_DH"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceDh?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                   }
                })
            }else if(form.Type=="H5_DEV_HIKISC"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceHikISC?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log("isc****************************",url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                       if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_TD"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceTd?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log("td****************************",url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_UNV"){
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceUnv?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&vms="+form.VMS+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log("yushi****************************",url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_DHDSS"){
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceDss?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log("yushi****************************",url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_IVS"){
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceIvs?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log("yushi****************************",url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_HIKINFOV"){
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceHikInfov?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log("yushi****************************",url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.loading = false;
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_VIKOR"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceVik?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.User)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.loading = false;
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.loading = false;
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }

        },
        //  Edit Add OK button
        edityes(){
            this.loading = true;
            console.log(this.editindex);
            //return false;
            this.editPopup = false;
            //url
            var form=this.editform;
            console.log("45111111******",form)
            //return false;
            var url1 = this.$store.state.IPPORT + "/api/v1/DelDevice?token="+encodeURIComponent(this.edittoken)+"&edit=true&session="+ this.$store.state.token;
            console.log("isc------------------------",url1)
            this.$http.get(url1).then(result=>{
                console.log("1",result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        var list = {
                            index:form.index,
                            Type:form.Type,
                            Name:form.Name,
                            Token:form.Token,
                            User:form.User,
                            Password:form.Password,
                            IP:form.IP,
                            Port:form.Port,
                            Audio :form.Audio,
                            Online:form.Online+"",
                            bPasswdEncrypt:form.bPasswdEncrypt,
                            bSandbox: form.bSandbox,
                            Maxichannels:form.Maxichannels,
                            ISAPI:form.ISAPI,
                            VMS:form.VMS
                            }
                        this.tableData.splice(this.editindex, 1,list)
                        console.log('123')
                        this.Success();
            
                        
                    }else{
                        this.loading = false;
                        console.log("Edit failed")
                        this.$message({
                            message: this.$t("message.setting.Editorfailure"),
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
            
        },
        platformyes(){
            this.dialogFormVisible=false;
            console.log(this.form)
            //return false;
            var form=this.form;
            
            console.log(form.Type)
            // return false
            if(form.Type=="H5_DEV_HIK"){
              var url = this.$store.state.IPPORT + "/api/v1/AddDeviceHik?&name="+encodeURIComponent(form.Name)+
              "&token="+encodeURIComponent(form.Token)+
              "&user="+encodeURIComponent(form.Username)+
              "&password="+encodeURIComponent(form.Password)+
              "&ip="+encodeURIComponent(form.IP)+
              "&port="+encodeURIComponent(form.Port)+
              "&audio="+form.Audio+
              "&sandbox="+form.bSandbox+
              "&isapi="+form.ISAPI+
              "&maxchannel="+form.Maxichannels+
              "&session="+ this.$store.state.token;
              console.log(url);
              this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.tableData=[];
                        this.loadHIK();
                    }else{
                        this.$message({
                            message: 'Add failed',
                            type: 'warning'
                        });
                        return false;
                    }
                  
                }
              })
            }else if(form.Type=="H5_DEV_DH"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceDh?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_dh)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }else if(form.Type=="H5_DEV_HIKISC"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceHikISC?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username_isc)+
                "&password="+encodeURIComponent(form.Password_isc)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_isc)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }else if(form.Type=="H5_DEV_TD"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceTd?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_td)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }else if(form.Type=="H5_DEV_UNV"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceUnv?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_unv)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&vms="+form.VMS+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }else if(form.Type=="H5_DEV_DHDSS"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceDss?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_DSS)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }else if(form.Type=="H5_DEV_IVS"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceIvs?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_1800)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }else if(form.Type=="H5_DEV_HIKINFOV"){
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceHikInfov?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username_INFOV)+
                "&password="+encodeURIComponent(form.Password_INFOV)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_INFOV)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+encodeURIComponent(form.Maxichannels)+
                "&session="+ this.$store.state.token;
                console.log("yushi****************************",url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                       }else{
                         this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                      }
                    }
                })
            }else if(form.Type=="H5_DEV_VIKOR"){
                console.log(form.Type)
                var url = this.$store.state.IPPORT + "/api/v1/AddDeviceVik?&name="+encodeURIComponent(form.Name)+
                "&token="+encodeURIComponent(form.Token)+
                "&user="+encodeURIComponent(form.Username)+
                "&password="+encodeURIComponent(form.Password)+
                "&ip="+encodeURIComponent(form.IP)+
                "&port="+encodeURIComponent(form.Port_vikor)+
                "&audio="+form.Audio+
                "&sandbox="+form.bSandbox+
                "&maxchannel="+form.Maxichannels+
                "&session="+ this.$store.state.token;
                console.log(url);
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.loadHIK();
                        }else{
                            this.$message({
                                message: 'Edit failed',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }
            
        },
        // Edit remove
        handleEdit(index,row){
            console.log("****************",row);
            var index_xlh=((this.currentPage1-1)*10)+index;
            // return false;
            this.editPopup = true;
            this.edittoken=row.Token;
            this.editindex=index_xlh;
            
            this.editform["Type"]=row.Type;
            this.editform["index"]=row.index;
            this.editform["Name"]=row.Name;
            this.editform["Token"]=row.Token;
            this.editform["User"]=row.User;
            this.editform["Password"]=row.Password;
            this.editform["IP"]=row.IP;
            this.editform["Port"]=row.Port;
            this.editform["Audio"]=row.Audio;
            this.editform["Online"]=row.Online;
            this.editform["bPasswdEncrypt"]=row.bPasswdEncrypt;
            this.editform["bSandbox"]=row.bSandbox;
            this.editform["Maxichannels"]=row.Maxchannel;
            this.editform["ISAPI"]=row.bISAPI;
            this.editform["VMS"]=row.bVMS;
            // console.log(this.editform)
            // console.log(this.tableData[index])
        },
        //Delete
        deleteRow(index, row,rows) {
            //var form=this.form;
            // console.log(index)
            console.log("Serial number",((this.currentPage1-1)*10)+index);
            var index_xlh=((this.currentPage1-1)*10)+index;
            //return false;
            //url
            var url = this.$store.state.IPPORT + "/api/v1/DelDevice?token="+encodeURIComponent(row.Token)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                console.log(this.tableData);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        rows.splice(index_xlh, 1);
                    }else{
                        this.$message({
                            message: 'failed to delete',
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        //Select all delete
        deleteselect(){
            var selectop=this.selectop;
            console.log(selectop)
            
            // return false
            //url
            for(var i=selectop.length-1;i>=0;i--){
                if(selectop[i].type=='duo'){
                    this.tableData.splice(selectop[i].index, 1);
                    console.log(selectop[i],i,selectop[i].index,this.currentPage)
                }
                console.log(selectop[i],i,selectop[i].index)
                var url = this.$store.state.IPPORT + "/api/v1/DelDevice?token="+encodeURIComponent(selectop[i].token)+"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    console.log(result);
                    console.log(this.tableData);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            if(selectop[0].type=='dan'){
                                this.tableData=[];
                                this.loadHIK();
                            }
                            this.$message({
                                message: this.$t("message.camera.Delete_successful"),
                                type: 'warning'
                            });
                        }else{
                            this.$message({
                                message:this.$t("message.camera.Delete_failed"),
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }
            
        },
        selectCall(row,index){
            console.log("INDEX",row,index);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    token:row[i].Token,
                    index:row[i].index-1,
                    type:"dan",
                };
                
                this.selectop.push(selectop);
            }
            
        },
        select_Call(row){
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    token:row[i].Token,
                    index:((this.currentPage-1)*10)+i,
                    type:'duo',
                    Name:row[i].Name
                };
                this.selectop.push(selectop);
            }
        },
        //search
        handlechange(){},
        //pagination

        handleSizeChange1(val) {
            console.log(`Per page ${val} strip`);
            this.currentPage1 = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            console.log(`current page: ${val}`);
            this.currentPage1 = val;
        },
    },
  };
</script>

<style lang="scss" scoped>
.el-dialog__body{
    padding-bottom:0 !important;
}
.sdk_button{
    display: flex;
    justify-content: space-between;
    .sdk_setting{
        button{
            color: rgba(58, 187, 254, 1);
        }
    }
}
.maxnum{
    font-size: 12px;
    line-height:2;
    margin: 0;
    padding:0;
}
.audio /deep/ .el-form-item__label{
    width:70px;
}
.inputSize /deep/ .el-input{
    width:83%;
}
</style>