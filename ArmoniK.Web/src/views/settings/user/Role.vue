<template>
    <div>
        <!-- Edit popup-->
        <el-dialog close="dialog" width="40%" :title="label.Edit" :visible.sync="editPopup">
            <el-form label-position="right" label-width="140px" :model="editform">
                <div class="tree_flex">
                    <div class="ed_tree">
                        <el-tree
                            :data="camdata" 
                            show-checkbox
                            ref="tree">
                            <span slot-scope="{ data }" style="width:100%;">
                                    <div style="width:100%;display: flex;justify-content: space-between;">
                                        <span >
                                            <span :class="data.iconclass" style="font-size:12px;"></span>
                                            <span :class="data.iconclass1" style="padding-left: 4px;">{{data.label}}</span>
                                        </span>
                                    </div>
                                </span>
                        </el-tree>
                    </div>
                    <div class="tow_node_root">
                            <div>
                                <!-- button  -->
                                <el-button class="button_addpv" @click="edaddcam" round size="mini">{{$t("message.setting.ADD")}}</el-button>
                            </div>
                            <div>
                                <el-button class="button_addpv" @click="eddlcam" round size="mini">{{$t("message.setting.DeleteAll")}}</el-button>
                            </div>
                        </div>
                    <div class="ed_tree">
                        <!--  show-checkbox :check-strictly="true" -->
                        <el-tree
                            :data="camroledata" 
                            show-checkbox
                            :props="defaultrole"
                            ref="tree1">
                            <span slot-scope="{ data }" style="width:100%;">
                                <div style="width:100%;display: flex;justify-content: space-between;">
                                    <span >
                                        <span :class="data.iconclass" style="color:rgb(142, 132, 132);"></span>
                                        <span :class="data.iconclass1" style="padding-left: 4px;">{{data.strRoleToken}}</span>
                                    </span>
                                </div>
                            </span>
                        </el-tree>
                    </div>
                </div>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button class="form_butt1" @click="editPopup = false">{{$t("message.setting.Cancel")}}</el-button>
            </div>
        </el-dialog>
        <!-- 添加 -->
        <el-dialog :title="eltitle" width="25%" :visible.sync="dialogFormVisible">
            <!-- 2 -->
            <el-form label-position="left"  label-width="90px" :model="form">
                <el-form-item :label="label.roleuser" class="role">
                    <el-input v-model="form.strRoleToken"></el-input>
                </el-form-item>
                <el-form-item label-width="160px" :label="label.Videoroot" class="role">

                </el-form-item>
                <div class="el_tree">
                    <el-tree
                        :data="camdata" 
                        show-checkbox
                        ref="tree">
                        <span slot-scope="{ data }" style="width:100%;">
                                <div style="width:100%;display: flex;justify-content: space-between;">
                                    <span >
                                        <span :class="data.iconclass" style="font-size:12px;"></span>
                                        <span :class="data.iconclass1" style="padding-left: 4px;">{{data.label}}</span>
                                    </span>
                                </div>
                            </span>
                    </el-tree>
                </div>
                <el-form-item :label="label.operroot">
                    <el-select v-model="form.strRole" placeholder="Please choose">
                        <el-option
                            v-for="item in options"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button class="form_butt1" @click="dialogFormVisible = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="platformyes1">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <div class="button_edi">
            <CButton class="form_butt" type="submit" @click="addto">Add role</CButton>
            <CButton class="form_butt1" type="submit" @click="deleteselect">Delete role</CButton>
        </div>
        <el-table
            @select='selectCall'
            @select-all='select_Call'
            :data="tableData1.filter(data => !search || data.Name.toLowerCase().includes(search.toLowerCase())).slice((currentPage1-1)*pageSize,currentPage1*pageSize)"
            stripe
            style="width: 100%">
            <el-table-column
                type="selection"
                width="55">
            </el-table-column>
            <el-table-column
            prop="strRoleToken"
            :label="label.roleuser"
            width="180">
            </el-table-column>
            <el-table-column
            prop="bConfig"
            :label="label.Confroot"
            width="180">
            </el-table-column>
            <el-table-column
            prop="bOperate"
            :label="label.operroot">
            </el-table-column>
            <el-table-column
                min-width="50"
                class="size"
                fixed="right">
                <template slot-scope="scope">
                    <el-button @click="handleEdit(scope.$index,scope.row)"  type="text" size="small">{{$t("message.setting.edit")}}</el-button>
                </template>
            </el-table-column>
        </el-table>
        <!-- pagination -->
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
import {listdatagload,listdatag,AllMethods} from '../../public/regional'
  export default {
    data() {
        return {
            Illegal:false,//illegal lock
            camdata:[],
            camroledata:[],
            eltitle:this.$t("message.setting.Configuration"),
            anonymous:false,//Anonymous
            search: '',//search
            activeName: 'first',
            tabPosition: 'button',//tab position
            inputedit:[],
            label:{
                Edit:this.$t("message.table.Edit"),

                label:this.$t("message.setting.user"),//Pick 1
                label_role:this.$t("message.setting.Rolemanagement"),//Pick 1
                label_system:this.$t("message.setting.CyberSecurity"),

                user:this.$t("message.setting.username"),
                Password:this.$t("message.setting.password"),
                role:this.$t("message.setting.role"),
                type:this.$t("message.setting.Authority"),
                olPassword:this.$t("message.setting.currentpass"),
                nePassword:this.$t("message.setting.newpass"),
                confirmpass:this.$t("message.setting.confirmpassword"),
                confirmpass1:this.$t("message.setting.confirmpass"),

                roleuser:this.$t("message.setting.RoleName"),
                Confroot:this.$t("message.setting.Configure"),
                operroot:this.$t("message.setting.Operation"),
                Videoroot:this.$t("message.setting.Camera"),

                label2:this.$t("message.setting.user"),
                label3:this.$t("message.setting.role"),
                label4:this.$t("message.setting.action"),

                label5:this.$t("message.setting.name"),
                label6:this.$t("message.setting.currentpass"),
                label7:this.$t("message.setting.newpass"),
                label8:this.$t("message.setting.confirmpass")
            },
            options: [{
                    value: 'Administrator',
                    label: 'Administrator'
                }, {
                    value: 'Operator',
                    label: 'Operator'
                }
            ],
            Role:[],
            defaultrole:{
                children: 'cam',
                label: 'strRoleToken'
            },
            defaultProps: {
                children: 'node',
                label: 'strName',
                cam:"cam"
            },
            //pagination
            pageSize: 10,//Number of pages
            dialogFormVisible: false,//Add popup
            editPopup:false,//Edit popup
            form: {
                switch:"first",
                strUser:"Operator",
                strPasswd: "12345",
                strPasswd1:"12345",
                strUserType: "Operator",
                
                strRole:"Administrator",
                strRoleToken:"Administrator"

            },
            editform: {
                strUser:"",
                strPasswd: "",
                strUserType: "",
                Newpassword:"",
                Newpassword1:""
            },
            edittoken:"",//The token to delete when editing
            editindex:"",//Index when editing
            
            tableData1: [],
            currentPage1: 1, // Current page number
            total1: 0, // Total number 1
            selectop:[]//Choose those
        };
    },
    created(){
        AllMethods().then(res=>{
            this.camdata = res.listdatag;
        }).catch(err=>{
            console.log(err);
        })
    },
    mounted(){
        this.rolesre();
    },
    methods:{
        // Second table list
        rolesre(){
            var url1 = this.$store.state.IPPORT + "/api/v1/GetRoleList?session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                if(result.status==200){
                    this.tableData1=[];
                    var data=result.data.roles;
                    for(var i=0;i<data.length;i++){
                        var datasrc={
                            strRoleToken:data[i].strRoleToken,
                            bConfig:data[i].permission.bConfig+"",
                            bOperate:data[i].permission.bOperate+""
                        }

                        var Role={
                            value: data[i].strRoleToken,
                            label: data[i].strRoleToken
                        }
                        this.Role.push(Role);
                        this.tableData1.push(datasrc);
                    }
                }
            })
        },

        addto(){
            this.dialogFormVisible=true;
        },
        //Click to delete
        deleteselect() {
            var selectop=this.selectop;
            var form=this.form;
            for(var i=0;i<selectop.length;i++){
                var url = this.$store.state.IPPORT + "/api/v1/DeleteRole?roletoken="+encodeURIComponent(selectop[i].strRoleToken)+"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData1=[];
                            this.rolesre();
                        }else{
                            this.$message({
                                message: "Failed to delete",
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
            var root = process.env.API_ROOT;
            if (root == undefined){
                root = window.location.protocol + '//' + window.location.host + window.location.pathname;
            }
            //url
            var form=this.editform;
            if( form.Newpassword!==form.Newpassword1){
                this.$message(this.$t("message.setting.Twopassword"));
                return false;
            }

            this.editPopup = false;
            var url1 = this.$store.state.IPPORT + "/api/v1/UpdateUser?user="+encodeURIComponent(form.strUser)+"&oldpassword="+encodeURIComponent($.md5(form.strPasswd))+"&newpassword="+encodeURIComponent($.md5(form.Newpassword))+"&session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                console.log("1",result,url1);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.$router.push({ path:'../../login'})
                        this.$store.commit(types.LOGOUT);
                        this.$message(this.$t("message.setting.Changecg"));
                    }else{
                        this.$message(this.$t("message.setting.Changesb"));
                    }
                    
                }
            })
        },
        
        platformyes1(){
            var tokencheked=this.$refs.tree.getCheckedNodes();
            this.dialogFormVisible=false;
            var form=this.form;
            var root = process.env.API_ROOT;
            if (root == undefined){
                root = window.location.protocol + '//' + window.location.host + window.location.pathname;
            }
            var url = this.$store.state.IPPORT + "/api/v1/CreateRole?roletoken="+encodeURIComponent(form.strRoleToken)+"&role="+encodeURIComponent(form.strRole)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.tableData1=[];
                        this.rolesre();
                        for(var i=0 ;i<tokencheked.length;i++){
                            for(var i=0 ;i<tokencheked.length;i++){
                            if (tokencheked[i].token) {
                                if (!tokencheked[i].children) {
                                this.addcam(tokencheked[i].token,form.strRoleToken,tokencheked[i].label);
                                }
                            }
                        }
                        }
                    }else{
                        this.$message({
                            message: "Add failed",
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        
        edaddcam(){
            var tokencheked=this.$refs.tree.getCheckedNodes();
            var tokencheked1=this.$refs.tree1.getCheckedNodes();
            console.log(tokencheked,tokencheked1)
            for(var i=0 ;i<tokencheked.length;i++){
                for(var l=0 ;l<tokencheked1.length;l++){
                    for(var i=0 ;i<tokencheked.length;i++){
                            if (tokencheked[i].token) {
                                if (!tokencheked[i].children) {
                                this.addcam(tokencheked[i].token,tokencheked1[l].strRoleToken,tokencheked[i].label);
                                }
                            }
                        }
                }
            }
        },
        
        addcam(token,roletoken,name){
            var root = process.env.API_ROOT;
            if (root == undefined){
                root = window.location.protocol + '//' + window.location.host + window.location.pathname;
            }
            console.log(token,roletoken,name);
            var url = this.$store.state.IPPORT + "/api/v1/AddCamToRole?roletoken="+encodeURIComponent(roletoken)+"&camtoken="+encodeURIComponent(token)+"&cam="+encodeURIComponent(name)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    if(result.data.bStatus==true){
                        var cam={
                            strRoleToken:name,
                            strCamName:token,
                        }
                        if(this.camroledata.length!=0){
                            this.camroledata[0].cam.push(cam)
                        }
                        
                    }else{
                        this.$message({
                            message: "Camera"+name+"Add failed",
                            type: 'warning'
                        });
                        // return false;
                    }
                }
            })
        },
        
        //Edit
        handleEdit(index,row){
            this.editPopup = true;
            this.editindex=index;
            this.editform["strUser"]=row.strUser;
            this.editform["strUserType"]=row.strUserType;
            this.camroledata=[];
            var url = this.$store.state.IPPORT + "/api/v1/GetRoleInfo?roletoken="+encodeURIComponent(row.strRoleToken)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    var data=result.data;
                    var roledata={
                            strRoleToken:data.strRoleToken,
                            strRole: data.strRole,
                            cam:[]
                        }
                    var camdata=data.cams;
                    for(var l=0; l<camdata.length;l++){
                        var camrole={
                            strRoleToken:data.cams[l].strCamName,
                            strCamName:data.cams[l].strCamToken,
                        }
                        roledata.cam.push(camrole);
                    }
                    this.camroledata.push(roledata);
                }
            })
        },
        //Delete camera
        eddlcam(){
            var tokencheked=this.$refs.tree1.getCheckedNodes();
            var tokencheked1=this.$refs.tree1.getHalfCheckedNodes();
            var strRoleToken='';
            if (!tokencheked[0].cam) {
                strRoleToken=tokencheked1[0].strRoleToken;
            }else{
                strRoleToken=tokencheked[0].strRoleToken;
            }
            var root = process.env.API_ROOT;
            if (root == undefined){
                root = window.location.protocol + '//' + window.location.host + window.location.pathname;
            }
            
            var camdata=this.camroledata[0].cam;
            if (tokencheked[0].cam) {
                for(var i=1;i<tokencheked.length;i++){
                    for(var l=0 ; l<camdata.length;l++){
                        if(tokencheked[i].strCamName==camdata[l].strCamName){
                            camdata.splice(l,1);
                            var url = this.$store.state.IPPORT + "/api/v1/DelCamInRole?roletoken="+encodeURIComponent(strRoleToken)+"&camtoken="+encodeURIComponent(tokencheked[i].strCamName)+"&session="+ this.$store.state.token;
                            this.$http.get(url).then(result=>{
                                if(result.status==200){
                                    if(result.data.bStatus==true){
                                        // this.editPopup=false
                                    }else{
                                        this.$message({
                                            message: tokencheked[i].strRoleToken,
                                            type: 'warning'
                                        });
                                    }
                                }
                            })
                        }
                    }
                }
            }else{
                for(var i=0;i<tokencheked.length;i++){
                    for(var l=0 ; l<camdata.length;l++){
                        if(tokencheked[i].strCamName==camdata[l].strCamName){
                            camdata.splice(l,1);
                            var url = this.$store.state.IPPORT + "/api/v1/DelCamInRole?roletoken="+encodeURIComponent(strRoleToken)+"&camtoken="+encodeURIComponent(tokencheked[i].strCamName)+"&session="+ this.$store.state.token;
                            this.$http.get(url).then(result=>{
                                if(result.status==200){
                                    if(result.data.bStatus==true){
                                        // this.editPopup=false
                                    }else{
                                        this.$message({
                                            message: tokencheked[i].strRoleToken,
                                            type: 'warning'
                                        });
                                        // return false;
                                    }
                                }
                            })
                        }
                    }
                }
            }

        },
        // Selected function
        selectCall(row){
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    strUser:row[i].strUser,
                    strPasswd:row[i].strPasswd,
                    strUserType:row[i].strUserType,
                    strRoleToken:row[i].strRoleToken
                };
                this.selectop.push(selectop);
            }
            
        },
        select_Call(row){
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                console.log(row[i].Token)
                var selectop={
                    strUser:row[i].strUser,
                    strPasswd:row[i].strPasswd,
                    strUserType:row[i].strUserType,
                    strRoleToken:row[i].strRoleToken
                };
                this.selectop.push(selectop);
            }
        },
        handleSizeChange1(val) {
            console.log(`Per page ${val} strip`);
            this.currentPage1 = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            console.log(`Current page: ${val}`);
            this.currentPage1 = val;
        },
        handlechange(){}
    },
  };
</script>

<style lang="scss" scoped>
.tree_flex{
    display: flex;
    flex-wrap: wrap;
    justify-content: space-between;
    align-items: center;
    /* Edit the two buttons */
    .ed_tree{
        width: 40%;
        height: 280px;
        // overflow: scroll;
        overflow: auto;
        border-radius: 4px;
        border:1px solid #DCDFE6;
        padding: 10px;
        margin-bottom: 20px;
        .el-tree{
            min-width: 250px;
        }
        &::-webkit-scrollbar {/*Overall style of the scroll bar*/
            width: 8px;     /*The height and width correspond to the size of the horizontal and vertical scroll bars respectively*/
            height: 8px;
            scrollbar-arrow-color:red;
        }
        &::-webkit-scrollbar-thumb {/*Small squares inside the scroll bar*/
            border-radius: 5px;
            -webkit-box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
            box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
            background: rgba(218, 218, 218,0.2);
            scrollbar-arrow-color:red;
        }
        &::-webkit-scrollbar-track {/*Track inside scroll bar*/
            -webkit-box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
            box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
            border-radius: 0;
            background: rgba(218, 218, 218,0.1);
        }
    }
    .tow_node_root div{
        margin-bottom: 25px;
    }
    .button_addpv{
        width: 100%;
        background: #3ABCFE;
        border-radius: 3px;
        border: none;
        color: white;
    }
}
</style>