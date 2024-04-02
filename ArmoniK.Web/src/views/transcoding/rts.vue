<template>
    <div>
        <el-dialog width="40%" :title="eltitle" :visible.sync="dialogFormVisible">
            <el-form class="el_form" ref="form" label-position='left' label-width="100px" size="small " :model="form">
                <el-form-item label="Name" class="inputSize">
                    <el-input v-model="form.name"></el-input>
                </el-form-item>
                <el-form-item label="Serial number" class="inputSize">
                    <el-input v-model="form.Token" :disabled="disabled"></el-input>
                </el-form-item>
                <el-form-item label="Address" class="inputSize">
                    <el-input v-model="form.IP"></el-input>
                </el-form-item>
                <!-- lingyige -->
                <el-form-item label="Port"  class="inputSize">
                    <el-input v-model="form.PORT"></el-input>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table" style="padding: 0px 20px 0px;">
                <el-button class="form_butt1" @click="dialogFormVisible = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="platformyes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <!-- Button -->
        <div class="sdk_button">
            <div class="button_edi" style="padding-top:5px;">
                <CButton class="form_butt" @click="addto" type="submit">Add</CButton>
            </div>
        </div>
        <!-- Sheet -->
        <el-table
            @select='selectCall' @select-all='select_Call'
            :data="tableData1.slice((currentPage1-1)*pageSize,currentPage1*pageSize)"
            style="width: 100%">
            <el-table-column prop="name" label="Name" width="180"></el-table-column>
            <el-table-column prop="Token" label="Serial number" width="180"></el-table-column>
            <el-table-column prop="strRemoteAlgoServerIp" label="Address"></el-table-column>
            <el-table-column prop="nRemoteAlgoServerPort" label="Port"></el-table-column>
            <el-table-column prop="bOnline" label="State"></el-table-column>
            <el-table-column prop="nSessionMax" label="Maximum transcoding"></el-table-column>
            <el-table-column prop="nSessions" label="Transcoding"></el-table-column>
            <el-table-column 
                fixed="right"
                label="Edit">
                <template slot-scope="scope">
                    <div class="" v-if="scope.row.Token=='local'?false:true">
                        <el-button type="text" size="small" @click="handleRefresh(scope.$index,scope.row)">Delete</el-button>
                        <el-button type="text" size="small" @click="handlerestart(scope.$index,scope.row)">Edit</el-button>
                    </div>
                </template>
            </el-table-column>
        </el-table>
        <el-pagination
            style="text-align: center;"
            @size-change="handleSizeChange1"
            @current-change="handleCurrentChange1"
            :current-page="currentPage1"
            :page-size="pageSize"
            layout=" prev, pager, next,total, jumper"
            :total="total2">
        </el-pagination>
    </div>
</template>
<script>
import uuid from '../../assets/js/uuid'
  export default {
    name:"GB28181",
    data() {
      return {
        disabled:true,
        eltitle:this.$t("message.setting.Configuration"),
        dialogFormVisible:false,
        //Pagination
        pageSize: 10,//Number of pages
        currentPage1: 1, // current page number
        total2: 0, // total number
        tableData1: [],//1
        form: {
            Token:'rts1',
            name:'rts1',
            IP:'192.168.100.1',
            PORT:'18000',
        },
        edit:false,
      };
    },
    mounted(){
        this.loadDevice();
    },
    methods:{
        platformyes(){
            var root =this.$store.state.IPPORT;
            this.dialogFormVisible = false
            if (this.edit) {
                var url = root + "/api/v1/DelTransRTS?token="+this.form.Token+"&session="+ this.$store.state.token;
                var urls = root + "/api/v1/AddTransRTS?name="+ this.form.name +"&token="+ this.form.Token +"&ip="+ this.form.IP +"&port="+ this.form.PORT +"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    if(result.status==200){
                        this.$http.get(urls).then(result=>{
                            if(result.status == 200){
                                this.loadDevice();
                            }
                        })
                    }
                })
            }else{
               //url
                var url = root + "/api/v1/AddTransRTS?name="+ this.form.name +"&token="+ this.form.Token +"&ip="+ this.form.IP +"&port="+ this.form.PORT +"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if(result.status == 200){
                        this.loadDevice();
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
        //Add
        addto(){
            this.edit = false;
            this.disabled=false;
            this.dialogFormVisible=true;
        },
        //data in the second table
        loadDevice(){
            this.tableData1=[];
		    var root =this.$store.state.IPPORT;
		   //url
            var url = root + "/api/v1/GetTransRTSList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status == 200){
                    //console.log(result);
                    var itme=result.data.server;
                    for(var i=0;i<itme.length;i++){
                        var tabledata={
                            Token:itme[i].strToken,
                            name:itme[i].strName,
                            bOnline:itme[i].bOnline+"",
                            nRemoteAlgoServerPort:itme[i].nRemoteAlgoServerPort,
                            nSessionMax:itme[i].nSessionMax,
                            nSessions:itme[i].nSessions,
                            strRemoteAlgoServerIp:itme[i].strRemoteAlgoServerIp
                        };
                        this.tableData1.push(tabledata);

                    }
                    this.total2=this.tableData1.length;
                }
            })
        },
        //Delete
        handleRefresh(index,row){
            var root =this.$store.state.IPPORT;
            var url = root + "/api/v1/DelTransRTS?token="+row.Token+"&session="+ this.$store.state.token;
            console.log(url);
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    this.loadDevice();
                    this.$message({
                        message: 'successfully deleted',
                        type: 'success'
                    });
                }
            })
        },
        //Edit
        handlerestart(index,row){
            console.log(row);
            this.disabled=true;
            this.edit=true;
            this.dialogFormVisible=true;
            let form = {
                Token:row.Token,
                name:row.name,
                IP:row.strRemoteAlgoServerIp,
                PORT:row.nRemoteAlgoServerPort,
            }
            this.form = form;
        },
        //pagination
        handleSizeChange1(val) {
            console.log(`per page ${val} strip`);
            this.currentPage1 = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            console.log(`Current page: ${val}`);
            this.currentPage1 = val;
        }
    }
  };
</script>