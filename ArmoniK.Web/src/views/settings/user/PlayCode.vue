<template>
    <div>
        <!-- Edit popup -->
        <el-dialog :title="eltitle" :visible.sync="editPopup" width="25%">
            <el-form class="el_form" ref="form" label-position='left' label-width="100px" size="small " :model="editform">
                <el-form-item label="Type">
                    <el-select v-model="editform.Type" placeholder="Please choose" @change="EditchangeType($event)" style="width:300px;">
                        <el-option
                            v-for="item in options"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="Play code" style="width:400px;">
                    <el-input v-model="editform.playcode"></el-input>
                </el-form-item>
                <el-form-item label="Get a list of regions">
                    <el-select v-model="editform.getregion" placeholder="Please choose" style="width:300px;">
                        <el-option
                            v-for="item in bGetRegionOptions"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table">
                <el-button class="form_butt1" @click="editPopup = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="edityes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <!-- Added popup -->
        <el-dialog :title="eltitle" :visible.sync="dialogFormVisible" width="60%">
            <el-form class="el_form" ref="form" label-position='left' label-width="150px" size="small " :model="form">
                <el-form-item label="Type" >
                    <el-select v-model="form.Type" placeholder="Please choose" @change="changeType($event)" style="width:350px;">
                        <el-option
                            v-for="item in options"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="Play code" style="width:500px;">
                    <el-input v-model="form.playcode"></el-input>
                </el-form-item>
                <el-form-item label="Get a list of regions">
                    <el-select v-model="form.getregion" placeholder="Please choose"  style="width:350px;">
                        <el-option
                            v-for="item in bGetRegionOptions"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table">
                <el-button class="form_butt1" @click="dialogFormVisible = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="platformyes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <!-- Add -->
        <div class="button_edi">
            <CButton class="form_butt" @click="addto" type="submit">Add</CButton>
            <CButton class="form_butt1" @click="deleteselect" type="submit">Delete</CButton>
        </div>
        <!-- delete -->
        <el-table
            :data="tableData.filter(data => !search || data.strSession.toLowerCase().includes(search.toLowerCase())).slice((currentPage2-1)*pageSize,currentPage2*pageSize)"
            @select='selectCall'
            @select-all='select_Call'
            stripe
            style="width: 100%"
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
            prop="strSession"
            label="Play code">
            </el-table-column>
            <el-table-column
            prop="bGetRegion"
            label="Get a list of regions">
            </el-table-column>
            <el-table-column fixed="right">
                <template slot="header" slot-scope="scope">
                    <el-input
                    v-model="search"
                    @change="handlechange(scope.$index,scope.row)"
                    size="mini"
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
            :total="total2">
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
          Index:this.$t("message.table.Index")
        },
        options: [{
            value: '4',
            label: '4 bit'
          }, {
            value: '36',
            label: '36 bit'
          }
        ],
        bGetRegionOptions: [{
            value: 'true',
            label: 'true'
          }, {
            value: 'false',
            label: 'false'
          }
        ],
        //pagination
        pageSize: 10,//Number of pages
        dialogFormVisible: false,//Add popup
        editPopup:false,//Edit popup
        form: {
          Type:'36',
          playcode:"1a35",
          getregion:"true",
        },
        editform: {
          Type:'36',
          playcode:"1a35",
          getregion:"true",
        },
        loading: false,
        edittoken:"",//The token to delete when editing
        editindex:"",//Index when editing
        
        tableData: [],//3
        total2: 0, // Total number 3
        currentPage2: 1, // Current page 3
        selectop:[]//Choose those
      };
    },
    mounted(){
      this.loadPlayCode();
    },
    methods:{
        EditchangeType(value){
            if (value==4) {
                this.editform["playcode"] = uuid(4, 16).toLowerCase();
            }else{
                this.editform["playcode"] = uuid().toLowerCase();
            }
        },
        changeType(value){
            if (value==4) {
                this.form["playcode"] = uuid(4, 16).toLowerCase();
            }else{
                this.form["playcode"] = uuid().toLowerCase();
            }
        },
        loadPlayCode(){
            var url = this.$store.state.IPPORT + "/api/v1/GetPlayCode?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
              console.log("a",result);
              if(result.status == 200){
                var itme=result.data.sessions;
                console.log(itme)
                this.tableData=[]
                for(var i=0;i<itme.length;i++){
                    var tabledata={
                        index:i+1,
                        strSession:itme[i].strSession,
                        bGetRegion:itme[i].bGetRegion+""
                    };
                    this.tableData.push(tabledata);
                    // console.log(this.tableData);
                }
                this.total2=this.tableData.length;
              }
            })
        },
        //  Edit Add OK button
        Success(){
            this.editPopup = false;
            var root = this.$store.state.IPPORT;
            //url
            var form=this.editform;
            var url = root + "/api/v1/AddPlayCode?&playcode="
            +encodeURIComponent(form.playcode)+
            "&getregion="+encodeURIComponent(form.getregion)+
            "&session="+ this.$store.state.token;
            console.log(url)
            this.$http.get(url).then(result=>{
                //console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.loading = false;
                        this.loadPlayCode();
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
        },
        edityes(){
            this.loading = true;
            console.log(this.editindex);
            this.editPopup = false;
            //url
            var form=this.editform;
            console.log("form",form);
            var url1 = this.$store.state.IPPORT + "/api/v1/DelPlayCode?playcode="+this.edittoken+"&session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                console.log("1",result,url1);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.Success();
                    }else{
                        this.loading = false;
                        this.$message({
                            message: "Edit failed",
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        platformyes(){
            this.dialogFormVisible=false;
            var form=this.form;
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v1/AddPlayCode?&playcode="
            +encodeURIComponent(form.playcode)+
            "&getregion="+encodeURIComponent(form.getregion)+
            "&session="+ this.$store.state.token;
            console.log(url);
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.tableData=[];
                        this.loadPlayCode();
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
        // Edit
        handleEdit(index,row){
            console.log(index,row);
            console.log(row.Audio,row.strUrl);
            var index_xlh="";
            console.log("Serial numberH5_FILE",this.currentPage2,((this.currentPage2-1)*10)+index);
            index_xlh=((this.currentPage2-1)*10)+index;
            this.editPopup = true;
            this.editindex=index_xlh;
            this.edittoken=row.strSession;
            if (row.strSession.length ==4) {
                this.editform["Type"]='4';
            }else{
                this.editform["Type"]='36';
            }
            this.editform["playcode"]=row.strSession;
            this.editform["getregion"]=row.bGetRegion;
            console.log(this.editform)
        },
        //Randomly get token data when you click Add
        addto(){
            this.dialogFormVisible=true;
            this.form["Type"]='36';
            this.form["playcode"] = uuid().toLowerCase();
        },
        //Click to delete
        deleteRow(index, row,rows) {
            console.log(rows,this.form.Type,index);
            var index_xlh="";
            console.log("Serial numberH5_FILE",this.currentPage2,((this.currentPage2-1)*10)+index);
            index_xlh=((this.currentPage2-1)*10)+index;
            //url
            var url = this.$store.state.IPPORT + "/api/v1/DelPlayCode?playcode="+encodeURIComponent(row.strSession)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                console.log(this.tableData);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        rows.splice(index_xlh, 1);
                        this.total2=this.tableData.length
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
            //url
            for(var i=selectop.length-1;i>=0;i--){
                //return false;
                if(selectop[i].type=='duo'){
                    this.tableData.splice(selectop[i].index, 1);
                }
                var url = this.$store.state.IPPORT + "/api/v1/DelPlayCode?playcode="+encodeURIComponent(selectop[i].strSession)+"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            if(selectop[0].type=='dan'){
                                this.tableData=[];
                                this.loadPlayCode();
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
        selectCall(row,index){
            console.log("INDEX",row,index);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    strSession:row[i].strSession,
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
                    strSession:row[i].strSession,
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
