
<template>
    <div>
      <!-- Edit popup -->
        <el-dialog width="25%" :title="eltitle" :visible.sync="editPopup">
          <el-form class="el_form" ref="form" label-position='left' label-width="150px" size="small " :model="editform">
              <el-form-item label="Name">
                    <el-input v-model="editform.name"></el-input>
                </el-form-item>
                <el-form-item label="Serial number">
                    <el-input v-model="editform.token"></el-input>
                </el-form-item>
                <el-form-item label="HTTP Port">
                    <el-input v-model="editform.http"></el-input>
                </el-form-item>
                <el-form-item label="HTTPS Port">
                    <el-input v-model="editform.https"></el-input>
                </el-form-item>
                <el-form-item label="SSL certificate file">
                    <el-input v-model="editform.strSSLCertificateFile"></el-input>
                </el-form-item>
                <el-form-item label="File Directory">
                    <el-input v-model="editform.strDocumentRoot"></el-input>
                </el-form-item>
                <el-form-item label="Service address">
                    <el-input v-model="editform.ip"></el-input>
                </el-form-item>
                <el-form-item label="Force HTTPS">
                    <el-switch active-color="#13ce66" v-model="editform.forcehttps"></el-switch>
                </el-form-item>
          </el-form>
          <div slot="footer" class="dialog-footer button_table">
              <el-button class="form_butt1" @click="editPopup = false">{{$t("message.setting.Cancel")}}</el-button>
              <el-button class="form_butt" type="primary" @click="edityes">{{$t("message.setting.OK")}}</el-button>
          </div>
        </el-dialog>
        <!-- Added popup -->
        <el-dialog width="25%" :title="eltitle" :visible.sync="dialogFormVisible">
            <el-form class="el_form" ref="form" label-position='left' label-width="150px" size="small " :model="form">
                <el-form-item label="Name">
                    <el-input v-model="form.name"></el-input>
                </el-form-item>
                <el-form-item label="Serial number">
                    <el-input v-model="form.token"></el-input>
                </el-form-item>
                <el-form-item label="HTTP Port">
                    <el-input v-model="form.http"></el-input>
                </el-form-item>
                <el-form-item label="HTTPS port">
                    <el-input v-model="form.https"></el-input>
                </el-form-item>
                <el-form-item label="SSL certificate file">
                    <el-input v-model="form.strSSLCertificateFile"></el-input>
                </el-form-item>
                <el-form-item label="File Directory">
                    <el-input v-model="form.strDocumentRoot"></el-input>
                </el-form-item>
                <el-form-item label="Service address">
                    <el-input v-model="form.ip"></el-input>
                </el-form-item>
                <el-form-item label="Force HTTPS">
                    <el-switch active-color="#13ce66" v-model="form.forcehttps"></el-switch>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table">
                <el-button class="form_butt1" @click="dialogFormVisible = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="platformyes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <!-- Two tables -->
        <!-- Add -->
        <div class="button_edi">
            <CButton class="form_butt" @click="addto" type="submit">Add</CButton>
            <CButton class="form_butt1" @click="deleteselect" type="submit">Delete</CButton>
        </div>
            <!-- Sheet -->
        <el-table
            :data="tableData.slice((currentPage2-1)*pageSize,currentPage2*pageSize).filter(data => !search || data.strName.toLowerCase().includes(search.toLowerCase()) || data.strToken.toLowerCase().includes(search.toLowerCase()))"
            @select='selectCall'
            @select-all='select_Call'
            style="width: 100%">
            <el-table-column type="selection" width="55"></el-table-column>
            <el-table-column prop="strName" label="Name"></el-table-column>
             <el-table-column prop="strToken" label="Serial number"></el-table-column>
            <el-table-column prop="nHTTPPort" label="HTTP Port"></el-table-column>
            <el-table-column prop="nHTTPSPort" label="HTTPS Port"></el-table-column>
            <el-table-column prop="strSSLCertificateFile" label="SSL certificate file"></el-table-column>
            <el-table-column prop="strDocumentRoot" label="File Directory"> </el-table-column>
            <el-table-column label="Force HTTPS">
                <template slot-scope="scope">
                    <el-switch active-color="#13ce66" v-model="scope.row.bForceHTTPS" disabled></el-switch>
                </template>
            </el-table-column>
            <el-table-column prop="strIPAddress" label="Service address"></el-table-column>
            <el-table-column fixed="right">
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
            @size-change="handleSizeChange3" 
            @current-change="handleCurrentChange3"
            :current-page="currentPage2"
            :total="total">
        </el-pagination>
        
    </div>
</template>
<script>
import uuid from '../../../assets/js/uuid'

  export default {
    data() {
      return {
        eltitle:this.$t("message.setting.Configuration"),
        search: '',//Search
        inputedit:[],
        label:{
        },
        //Pagination
        pageSize: 10,//Number of pages
        dialogFormVisible: false,//Add popup
        editPopup:false,//Edit popup
        form: {
            name:"server1",
            token:"server1",
            http:"8280",
            https:"8643",
            forcehttps:false,
            ip:"0.0.0.0",
            // strAuthenticationDomain: "localhost",
            strDocumentRoot: "www/domain/cluster/",
            strSSLCertificateFile: "server.pem"
        },
        editform: {
            name:"server1",
            token:"server1",
            http:"8280",
            https:"8643",
            forcehttps:false,
            ip:"0.0.0.0",
            // strAuthenticationDomain: "localhost",
            strDocumentRoot: "www/domain/cluster/",
            strSSLCertificateFile: "server.pem"
        },
        edittoken:"",//The token to delete when editing
        editindex:"",//index when editing
        
        tableData: [],//3
        total: 0, // Total number 3
        currentPage2: 1, // Current page 3
        selectop:[]//Choose those
      };
    },
    mounted(){
        this.loadfile();
    },
    methods:{
        loadfile(){
            //url
            var url = this.$store.state.IPPORT + "/api/v1/GetHTTPServerList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
              console.log("a",result);
              if(result.status == 200){
                  var itme=result.data.server;
                  this.tableData=[]
                  for(var i=0;i<itme.length;i++){
                      var tabledata={
                          index:i+1,
                          nHTTPPort:itme[i].nHTTPPort,
                          nHTTPSPort:itme[i].nHTTPSPort,
                          strSSLCertificateFile:itme[i].strSSLCertificateFile,
                          bForceHTTPS:itme[i].bForceHTTPS,
                          strName:itme[i].strName,
                          strToken:itme[i].strToken,
                          strDocumentRoot:itme[i].strDocumentRoot,
                          strIPAddress:itme[i].strIPAddress,
                      };
                      this.tableData.push(tabledata);
                      //console.log(tabledata);
                  }
                  this.total=this.tableData.length;
              }
            })
        },
        // Edit
        handleEdit(index,row){
            console.log(index);
            console.log(this.tableData[index]);
            console.log("Serial number",((this.currentPage2-1)*10)+index);
            var index_xlh=((this.currentPage2-1)*10)+index;
            //return false;
            this.editPopup = true;
            this.edittoken=row.strToken;
            this.editindex=index_xlh;
            
            // this.editform["strAuthenticationDomain"]=row.strAuthenticationDomain;
            this.editform["name"]=row.strName;
            this.editform["token"]=row.strToken;
            this.editform["http"]=row.nHTTPPort;
            this.editform["https"]=row.nHTTPSPort;
            this.editform["forcehttps"]=row.bForceHTTPS;
            this.editform["ip"]=row.strIPAddress;
            this.editform["strDocumentRoot"]=row.strDocumentRoot;
            this.editform["strSSLCertificateFile"]=row.strSSLCertificateFile;
            console.log(this.editform);
        },
        
        //  Edit Add OK button
        edityes(){
            console.log(this.editindex);
            //return false;
            this.editPopup = false;
            var root = this.$store.state.IPPORT;
            //url
            var form=this.editform;
            console.log("form", form);
            // return false;
            var url1 = root + "/api/v1/DelHTTPServer?token="+this.edittoken+"&session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                //console.log("1",result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        var list = {
                            // strAuthenticationDomain:form.strAuthenticationDomain,
                            nHTTPPort:form.http,
                            nHTTPSPort:form.https,
                            bForceHTTPS:form.forcehttps,
                            strName:form.name,
                            strToken:form.token,
                            strIPAddress:form.ip,
                            strDocumentRoot:form.strDocumentRoot,
                            strSSLCertificateFile:form.strSSLCertificateFile,
                        }
                        this.tableData.splice(this.editindex, 1,list)
                        var url = root + "/api/v1/AddHTTPServer?&http="+form.http+"&https="+form.https+"&forcehttps="+form.forcehttps+"&ip="+form.ip+"&name="+form.name+"&token="+form.token+"&sslcertificatefile="+form.strSSLCertificateFile+"&documentroot="+form.strDocumentRoot+"&session="+ this.$store.state.token;
                        console.log(url);
                        this.$http.get(url).then(result=>{
                            console.log(result);
                            if(result.status==200){
                                this.$root.bus.$emit('webrtc',true);
                            }
                        })
                    }else{
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
            
            var root = this.$store.state.IPPORT;
            console.log(form.Type)
            var url = root + "/api/v1/AddHTTPServer?&http="+form.http+"&https="+form.https+"&forcehttps="+form.forcehttps+"&ip="+form.ip+"&name="+form.name+"&token="+form.token+"&sslcertificatefile="+form.strSSLCertificateFile+"&documentroot="+form.strDocumentRoot+"&session="+ this.$store.state.token;
            console.log(url);
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus){
                        this.tableData=[];
                        this.loadfile();
                        // this.reload();
                        this.$root.bus.$emit('webrtc',true);
                    }else{
                        this.$message({
                            message: 'Add failed',
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
            
        },
        //view edit remove
        //Randomly get token data when you click Add
        addto(){
            this.dialogFormVisible=true;
            this.form["Token"] = uuid(4, 16).toLowerCase();
        },
        //Click to delete
        deleteRow(index, row,rows) {
          //var form=this.form;
            // console.log(rows,row,this.form.Type,index);
            var index_xlh="";
            // return false;
            console.log("Serial number V1_FILE",this.currentPage2,((this.currentPage2-1)*10)+index);
            index_xlh=((this.currentPage2-1)*10)+index;
            //url
            var url = this.$store.state.IPPORT + "/api/v1/DelHTTPServer?token="+encodeURIComponent(row.strToken)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                console.log(this.tableData);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        rows.splice(index_xlh, 1);
                        this.total=this.tableData.length
                    }else{
                        this.$message({
                            message: 'Failed to delete',
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        //Select all delete
        async deleteselect(){
            var selectop=this.selectop;
            var root = this.$store.state.IPPORT;
            for(var i=selectop.length-1;i>=0;i--){
                var url = root + "/api/v1/DelHTTPServer?token="+selectop[i].token+"&session="+ this.$store.state.token;
                await this.$http.get(url).then(result=>{
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            if(selectop[0].type=='dan'){
                                this.tableData=[];
                                this.loadfile();
                            }
                            if(selectop[i].type=='duo'){
                                console.log(selectop[i].index)
                                this.tableData.splice(selectop[i].index, 1);
                            }
                        }else{
                            this.$message({
                                message: 'Failed to delete',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
                
                

            }
            
        },
        selectCall(row){
            console.log(row);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                console.log(row[i].strToken)
                var selectop={
                    token:row[i].strToken,
                    type:"dan",
                };
                this.selectop.push(selectop);
            }
            
        },
        select_Call(row){
            console.log(row);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                console.log(row[i].strToken)
                var selectop={
                    token:row[i].strToken,
                    index:((this.currentPage2-1)*10)+i,
                    type:'duo',
                };
                this.selectop.push(selectop);
            }
        },
        //Search
        handlechange(){},
        //Pagination
        //3
        handleSizeChange3(val) {
            console.log(`Per page ${val} strip`);
            this.currentPage2 = 1;
            this.pageSize = val;
        },
        handleCurrentChange3(val) {
            console.log(`Current page: ${val}`);
            this.currentPage2 = val;
        }
    },
  };
</script>
