<template>
    <div class="CloudConnect">
        <!-- Edit popup -->
        
        <el-dialog :title="label.Edit" :visible.sync="editPopup">
            <el-form label-position="left" label-width="80px" :model="editform">
                <el-form-item :label="label.user">
                    <el-input v-model="editform.strUser"></el-input>
                </el-form-item>
                <el-form-item :label="label.password">
                    <el-input type="password" v-model="editform.strPasswd"></el-input>
                </el-form-item>
                <el-form-item :label="label.name">
                    <el-input v-model="editform.strServerName"></el-input>
                </el-form-item>
                <el-form-item :label="label.Token">
                    <el-input v-model="editform.strServerToken"></el-input>
                </el-form-item>
                <el-form-item :label="label.IP">
                    <el-input v-model="editform.strCloudIp"></el-input>
                </el-form-item>
                <el-form-item :label="label.Port">
                    <el-input v-model="editform.strCloudPort"></el-input>
                </el-form-item>
                <el-form-item :label="label.KeepaliveTime">
                    <el-input v-model="editform.nKeepaliveTime"></el-input>
                </el-form-item>
                <el-form-item :label="label.Enable">
                    <el-select v-model="editform.bEnable" placeholder="Please choose">
                        <el-option
                            v-for="item in scaletype"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.SSL">
                    <el-select v-model="editform.bSSL" placeholder="Please choose">
                        <el-option
                            v-for="item in scaletype"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.EdgeTranscoding">
                    <el-select v-model="editform.bEdgeTranscoding" placeholder="Please choose">
                        <el-option
                            v-for="item in scaletype"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button class="form_butt1" @click="editPopup = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="proedityes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <!-- two tables -->
        <el-table
            :data="tableData.filter(data => !search || data.Name.toLowerCase().includes(search.toLowerCase())).slice((currentPage1-1)*pageSize,currentPage1*pageSize)"
            style="width: 100%">
            <el-table-column
                prop="strUser"
                :label="label.user"
                width="100">
            </el-table-column>
            <el-table-column
            prop="strServerName"
            :label="label.name">
            </el-table-column>
            <el-table-column
            prop="strServerToken"
            :label="label.Token"
            min-width="80">
            </el-table-column>
            <el-table-column
            prop="strCloudIp"
            :label="label.IP"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="strCloudPort"
            :label="label.Port"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="bOnline"
            :label="label.Online"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="bSSL"
            :label="label.SSL"
            min-width="140">
            </el-table-column>
            <el-table-column
                fixed="right"
                min-width="140">
                <template slot="header" slot-scope="scope">
                    <el-input
                    v-model="search"
                    @change="handlechange(scope.$index,scope.row)"
                    size="mini"
                    placeholder="Enter keywords"/>
                </template>
                <template slot-scope="scope">
                    <el-button @click="handleClick(scope.$index,scope.row)" type="text" size="small">{{$t("message.setting.Detail")}}</el-button>
                    <el-button @click="handleEdit(scope.$index,scope.row)" type="text" size="small">{{$t("message.setting.edit")}}</el-button>
                </template>
            </el-table-column>
        </el-table>
        <!-- Pagination -->
        <el-pagination
            style="text-align: center;"
            layout="prev, pager, next"
            @size-change="handleSizeChange1" 
            @current-change="handleCurrentChange1"
            :current-page="currentPage1"
            :total="total1">
        </el-pagination>
        
    </div>
</template>
<script>
import uuid from '../../../assets/js/uuid'
import '../../../assets/js/jQuery.md5'
  export default {
    data() {
      return {
        eltitle:this.$t("message.setting.Configuration"),
        search: '',//Search
        activeName: 'Default',
        tabPosition: 'button',//Tab position
        inputedit:[],
        label:{
            label:this.$t("message.setting.CloudConnect"),//Pick 1
            Edit:this.$t("message.table.Edit"),
            user:this.$t("message.setting.user"),
            name:this.$t("message.setting.name"),
            Token:this.$t("message.table.Token"),
            IP:this.$t("message.table.IP"),
            Port:this.$t("message.table.Port"),
            Online:this.$t("message.table.Online"),
            SSL:this.$t("message.setting.SSL"),
            password:this.$t("message.setting.password"),
            KeepaliveTime:this.$t("message.setting.KeepaliveTime"),
            Enable:this.$t("message.setting.Enable"),
            EdgeTranscoding:this.$t("message.setting.EdgeTranscoding")
        },
        //Pagination
        editform: {
            strUser:"",
            strServerName:"",
            strServerToken:"",
            bEdgeTranscoding:"",
            bEnable:"",
            bOnline:"",
            bSSL:"",
            nKeepaliveTime:"",
            strCloudIp:"",
            strCloudPort:"",
            strPasswd:"12345"
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
        tableData: [],//1
        currentPage1: 1, // Current page number
        total1: 0, // total number 1

        tableData2: [],//2
        total2: 0, // total number 2
        currentPage2: 1, // current page 2
        editpasswd:""//edited password
      };
    },
    mounted(){
        this.loadstream();
    },
    methods:{
        loadstream(){
            this.editPopup = false;
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v1/GetCloudClientInfo?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
            //   return false;
                if(result.status == 200){
                    var itme=result.data;
                    // console.log(result)
                    var tabledata={
                        strUser: itme.strUser,
                        strServerName:itme.strServerName,
                        strServerToken:itme.strServerToken,
                        bEdgeTranscoding:itme.bEdgeTranscoding+"",
                        bEnable:itme.bEnable,
                        bOnline:itme.bOnline+"",
                        bSSL:itme.bSSL+"",
                        nKeepaliveTime:itme.nKeepaliveTime,
                        strCloudIp:itme.strCloudIp,
                        strCloudPort:itme.strCloudPort,
                        strPasswd:itme.strPasswd
                    };
                    console.log(tabledata)
                    this.tableData.push(tabledata);
                    this.total1=this.tableData.length;
                }
            })
        },
        //view edit
        handleClick(index,row){
           console.log(row,index,this.tableData);
            this.$alert(
                '<div> User: '
                +row.strUser+'</div><div>ServerName: '
                +row.strServerName+'</div><div>ServerToken: '
                +row.strServerToken+'</div><div>EdgeTranscoding: '
                +row.bEdgeTranscoding+'</div><div>Enable:' 
                +row.bEnable+'</div><div>Online: '
                +row.bOnline+'</div><div>SSL: '
                +row.bSSL+'</div><div>KeepaliveTime: '
                +row.nKeepaliveTime+'</div><div>CloudIp: '
                +row.strCloudIp+'</div><div>CloudPort: '
                +row.strCloudPort+'</div>', '标题名称',{dangerouslyUseHTMLString: true});
        },
        
        handleEdit(index,row){
            this.editPopup = true;
            this.editform["strUser"]=row.strUser;
            this.editform["strServerName"]=row.strServerName;
            this.editform["strServerToken"]=row.strServerToken;
            this.editform["bEdgeTranscoding"]=row.bEdgeTranscoding+"";
            this.editform["bEnable"]=row.bEnable+"";
            this.editform["bOnline"]=row.bOnline;
            this.editform["bSSL"]=row.bSSL+"";
            this.editform["nKeepaliveTime"]=row.nKeepaliveTime;
            this.editform["strCloudIp"]=row.strCloudIp;
            this.editform["strCloudPort"]=row.strCloudPort;
            this.editform["strPasswd"]=row.strPasswd;
            this.editpasswd = row.strPasswd;
        },
        //  Edit Add OK button
        proedityes(){
            var form=this.editform;
    
            console.log("Default",this.editpasswd,form.strPasswd);
            var root = this.$store.state.IPPORT;
            //url
            var passwordol=""
            if(this.editpasswd==form.strPasswd){
                passwordol=this.editpasswd
            }else if(this.editpasswd!=form.strPasswd){
                passwordol=$.md5(form.strPasswd)
            }
            console.log("Default",passwordol);
            // return false
            var url = root + "/api/v1/SetCloudInfo?enable="+encodeURIComponent(form.bEnable)+
            "&servername="+encodeURIComponent(form.strServerName)+
            "&servertoken="+encodeURIComponent(form.strServerToken)+
            "&cloudip="+encodeURIComponent(form.strCloudIp)+
            "&cloudport="+encodeURIComponent(form.strCloudPort)+
            "&ssl="+encodeURIComponent(form.bSSL)+
            "&user="+encodeURIComponent(form.strUser)+
            "&password="+encodeURIComponent(passwordol)+
            "&keepalivetime="+encodeURIComponent(form.nKeepaliveTime)+
            "&edgetranscoding="+encodeURIComponent(form.bEdgeTranscoding)+
            "&session="+ this.$store.state.token;
            console.log(url);
            // return false
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.editPopup = false;
                        this.tableData=[];
                        this.loadstream();
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
        //Pagination
        change (e) {
            this.$forceUpdate()
        },
        handleSizeChange1(val) {
            console.log(`Per page ${val} strip`);
            this.currentPage1 = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            console.log(`current page: ${val}`);
            this.currentPage1 = val;
        }
    },
  };
</script>