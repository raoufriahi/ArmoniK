<template>
	<div class="dasboard_global">
        <!-- Edit popup -->
        <el-dialog
            class="updatepassword Userupdatepassword"
            title="Change Password"
            :visible.sync="dialogVisible"
            width="60%">
             <el-form class="el_form" label-position="left" label-width="80px" :model="editform">
                 <el-form-item :label="label.user">
                    <el-input disabled v-model="editform.strUser"></el-input>
                </el-form-item>
                <el-form-item label="Old Password:">
                    <el-input type="password" v-model="editform.strPasswd" show-password></el-input>
                    <span class="strPasswd"><i class="strPasswd1 el-icon-circle-check right"></i></span>
                    <span class="oldstrPasswd error">Wrong old password</span>
                </el-form-item>
                <el-form-item label="New password:">
                    <el-input type="password" v-model.trim="editform.Newpassword" show-password @input="update" ></el-input>
                    <span class="strPasswd2"><i class="strPasswd3 el-icon-circle-check"></i><span class="strPasswd32">Password security index is too low, please reset</span></span>
                    <span class="strPasswdhelp error">The new password cannot be the same as the old password</span>
                    <span class="strPasswdhelp2 error">New password cannot be empty</span>
                </el-form-item>
                <span class="updateHelp">
                    To improve your password security, it is recommended to use the following:
                </span>
                <ul class="rule">
                    <li><span class="rulechildren rule1"></span><span class="ruletext1">Password must be at least 8 characters long.</span></li>
                    <li><span class="rulechildren rule2"></span><span class="ruletext2">Password contains at least one uppercase letter.</span></li>
                    <li><span class="rulechildren rule3"></span><span class="ruletext3">Password contains at least one lowercase letter.</span></li>
                    <li><span class="rulechildren rule4"></span><span class="ruletext4">Password contains at least one number.</span></li>
                    <li><span class="rulechildren rule5"></span><span class="ruletext5">Password contains at least one special character. For example @ # & etc.</span></li>
                    <li><span class="rulechildren rule6"></span><span class="ruletext6">The password cannot contain 2 consecutive increasing or decreasing numbers. For example 12 321 5678 etc.</span></li>
                    <li><span class="rulechildren rule7"></span><span class="ruletext7">Password cannot contain username.</span></li>
                </ul>
                <el-form-item label="Confirm Password:" class="confirmpassword">
                    <el-input type="password" v-model="editform.Newpassword1" show-password @input="confirmpassword"></el-input>
                    <span class="strPasswd4"><i class="strPasswd5  el-icon-circle-check"></i></span>
                </el-form-item>
            </el-form>
            <span slot="footer" class="dialog-footer">
                <el-button class="save" type="primary" @click="changepwss()">Sure</el-button>
                <el-button class="saveBtn"  @click="dialogVisible = false">Cancel</el-button>
            </span>
        </el-dialog>
        <!-- Add -->
        <el-dialog
            class="updatepassword Userupdatepassword"
            title="Add user"
            :visible.sync="dialogFormVisible"
            width="70%">
             <el-form class="el_form" label-position="left" label-width="130px" :model="form">
                <el-form-item :label="label.user">
                    <el-input v-model="form.strUser"></el-input>
                </el-form-item>
                <el-form-item label="New password:">
                    <el-input type="password" v-model.trim="form.strPasswd" show-password @input="adduser"></el-input>
                    <span class="strPasswd22"><i class="strPasswd33 el-icon-circle-check"></i><span class="strPasswd323">Password security index is too low, please reset</span></span>
                </el-form-item>
                <span class="updateHelp">
                    To improve your password security, it is recommended to use the following:
                </span>
                <ul class="rule">
                    <li><span class="rulechildren rule11"></span><span class="ruletext11">Password length at least 8 characters.</span></li>
                    <li><span class="rulechildren rule22"></span><span class="ruletext22">Password contains at least one uppercase letter.</span></li>
                    <li><span class="rulechildren rule33"></span><span class="ruletext33">Password contains at least one lowercase letter.</span></li>
                    <li><span class="rulechildren rule44"></span><span class="ruletext44">Password contains at least one number.</span></li>
                    <li><span class="rulechildren rule55"></span><span class="ruletext55">Password contains at least one special character. For example @ # & etc.</span></li>
                    <li><span class="rulechildren rule66"></span><span class="ruletext66">The password cannot contain 2 consecutive increasing or decreasing numbers. For example 12 321 5678 etc.</span></li>
                    <li><span class="rulechildren rule77"></span><span class="ruletext77">Password cannot contain username.</span></li>
                </ul>
                <el-form-item label="Confirm Password:" class="addconfirmpassword">
                    <el-input type="password" v-model="form.strPasswd1" show-password @input="addconfirmpassword"></el-input>
                    <span class="strPasswd44"><i class="strPasswd55 el-icon-circle-check"></i></span>
                </el-form-item>
                <el-form-item :label="label.role">
                    <el-select v-model="form.strRoleToken" placeholder="Please choose">
                        <el-option
                            v-for="item in Role"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
             </el-form>
            <span slot="footer" class="dialog-footer">
                <el-button class="save" type="primary" @click="adduse()">Sure</el-button>
                <el-button class="saveBtn"  @click="dialogFormVisible = false">Cancel</el-button>
            </span>
        </el-dialog>
        <div class="button_edi">
            <CButton class="form_butt" @click="openAddform" type="submit">Add user</CButton>
            <CButton class="form_butt1" @click="deleteuser" type="submit">Delete users</CButton>
        </div>
        <el-table
            :data="tableData.filter(data => !search || data.strUser.toLowerCase().includes(search.toLowerCase())).slice((currentPage-1)*pageSize,currentPage*pageSize)"
            stripe
            @select='selectCall'
            @select-all='select_Call'
            style="width: 100%">
            <el-table-column
            type="selection"
            width="55">
            </el-table-column>
            <el-table-column
            prop="strUser"
            label="Username"
            width="180">
            </el-table-column>
            <el-table-column
            prop="strRole"
            label="Role"
            min-width="50">
            </el-table-column>
            <el-table-column
                min-width="50"
                class="size"
                fixed="right">
                <template  slot-scope="scope">
                    <el-button @click="handleClick(scope.$index,scope.row)" type="text" size="small">{{$t("message.setting.Change")}}</el-button>
                </template>
            </el-table-column>
        </el-table>
        <el-pagination
            style="text-align: center;"
            layout=" prev, pager, next,total, jumper"
            @size-change="handleSizeChange1" 
            @current-change="handleCurrentChange1"
            :current-page="currentPage"
            :total="total">
        </el-pagination>
	</div>
</template>

<script>
import '../../../assets/js/jQuery.md5'
export default {
	name: 'roleinof',
	data(){
		return{
            total: 0, // total number 1
            currentPage: 1, // current page 1
            pageSize: 10, // Number of pages
            search: '', // Search
			tableData: [],
            editPopup:false,
            dialogFormVisible:false,
            eltitle:this.$t("message.setting.Configuration"),
			label:{
                Edit:this.$t("message.setting.edit"),

                label:this.$t("message.setting.user"),//Pick 1
                user:this.$t("message.setting.username"),
                Password:this.$t("message.setting.password"),
                role:this.$t("message.setting.role"),
                type:this.$t("message.setting.Authority"),
                olPassword:this.$t("message.setting.currentpass"),
                nePassword:this.$t("message.setting.newpass"),
                confirmpass:this.$t("message.setting.confirmpassword"),//Repeat password
                confirmpass1:this.$t("message.setting.confirmpass"),//Repeat new password
            },
			form: {
                strUser:"Operator",
                strPasswd: "12345",
                strPasswd1:"12345",
                strRoleToken: "Administrator",

            },
            Role:[],
            editform: {
                strUser:"",
                strPasswd: "",
                strUserType: "",
                Newpassword:"",
                Newpassword1:""
            },
            dialogVisible:false,
            reg8:true,
            reg88:true,
            selectop:[]//choose those
		}
    },
	mounted(){
        if(this.$store.state.root==='Operator'){
            return false
        }else{
            this.user();
            this.rolelist();
        }
	},
	created(){
	},
	methods:{
        //When adding a user, monitor whether the duplicate password is the same as the new password
        addconfirmpassword(){
            if( this.form.strPasswd1!==this.form.strPasswd){
                this.$message(this.$t("message.setting.Twopassword"));
                $('.strPasswd55').removeClass('el-icon-circle-check');
                $('.strPasswd55').addClass('el-icon-circle-close');
                $('.strPasswd55').removeClass('right');
                $('.strPasswd55').addClass('error');
                $(".strPasswd44").css('display', 'block');
                $(".strPasswd55").css('display', 'block');
            }else{
                $('.strPasswd55').removeClass('el-icon-circle-close');
                $('.strPasswd55').addClass('el-icon-circle-check');
                $('.strPasswd55').removeClass('error');
                $('.strPasswd55').addClass('right');
                $(".strPasswd44").css('display', 'block');
                $(".strPasswd55").css('display', 'block');
            }
        },
        //When changing the password, monitor whether the duplicate password is the same as the new password
        confirmpassword(){
            var form=this.editform;
            if( form.Newpassword!==form.Newpassword1){
                // this.$message(this.$t("message.setting.Twopassword"));
                $('.strPasswd5').removeClass('el-icon-circle-check');
                $('.strPasswd5').addClass('el-icon-circle-close');
                $('.strPasswd5').removeClass('right');
                $('.strPasswd5').addClass('error');
                $(".strPasswd4").css('display', 'block');
                $(".strPasswd5").css('display', 'block');
            }else{
                $('.strPasswd5').removeClass('el-icon-circle-close');
                $('.strPasswd5').addClass('el-icon-circle-check');
                $('.strPasswd5').removeClass('error');
                $('.strPasswd5').addClass('right');
                $(".strPasswd4").css('display', 'block');
                $(".strPasswd5").css('display', 'block');
            }
        },
        openAddform(){
            this.dialogFormVisible=true;
            this.$nextTick(()=>{
                if (this.dialogFormVisible==true) {
                    document.getElementsByClassName("strPasswd22")[0].style.display="none";
                    document.getElementsByClassName("strPasswd44")[0].style.display="none";
                    document.getElementsByClassName("strPasswd55")[0].style.display="none";
                }
            })
        },
        adduser(){
            var reg1 = new RegExp(/(?=.*[A-Z])/);
            var reg2 = new RegExp(/(?=.*[a-z])/);
            var reg3 = new RegExp(/(?=.*[0-9])/);
            var reg4 = new RegExp(/(?=(?:.*?[!@#$%*()_+^&}{:;?.]){1})/);
            var reg5 = new RegExp(/((?:0(?=1)|1(?=2)|2(?=3)|3(?=4)|4(?=5)|5(?=6)|6(?=7)|7(?=8)|8(?=9)|9(?=0)){1,2}\d)/);
            var reg6 = new RegExp(/((?:9(?=8)|8(?=7)|7(?=6)|6(?=5)|5(?=4)|4(?=3)|3(?=2)|2(?=1)|1(?=0)){1,2}\d)/);
            var reg7 = RegExp(this.form.strUser,"g");
            if (this.form.strPasswd.length>=8) {
                document.getElementsByClassName("rule11")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext11")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule11")[0].style.background="red";
                document.getElementsByClassName("ruletext11")[0].style.color="red";
            }
            if (reg1.test(this.form.strPasswd)) {
                document.getElementsByClassName("rule22")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext22")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule22")[0].style.background="red";
                document.getElementsByClassName("ruletext22")[0].style.color="red";
            }
            if (reg2.test(this.form.strPasswd)) {
                document.getElementsByClassName("rule33")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext33")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule33")[0].style.background="red";
                document.getElementsByClassName("ruletext33")[0].style.color="red";
            }
            if (reg3.test(this.form.strPasswd)) {
                document.getElementsByClassName("rule44")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext44")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule44")[0].style.background="red";
                document.getElementsByClassName("ruletext44")[0].style.color="red";
            }
            if (reg4.test(this.form.strPasswd)) {
                document.getElementsByClassName("rule55")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext55")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule55")[0].style.background="red";
                document.getElementsByClassName("ruletext55")[0].style.color="red";
            }
            if (this.form.strPasswd.length>=2) {
                if (reg5.test(this.form.strPasswd)||reg6.test(this.form.strPasswd)) {
                    document.getElementsByClassName("rule66")[0].style.background="red";
                    document.getElementsByClassName("ruletext66")[0].style.color="red";
                }else{
                    document.getElementsByClassName("rule66")[0].style.background="#80C269";
                    document.getElementsByClassName("ruletext66")[0].style.color="#80C269";
                }
            }else if(this.form.strPasswd.length==1){
                document.getElementsByClassName("rule66")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext66")[0].style.color="#80C269";
            }
            if (this.form.strPasswd) {
                if (reg7.exec(this.form.strPasswd)==null) {
                    document.getElementsByClassName("rule77")[0].style.background="#80C269";
                    document.getElementsByClassName("ruletext77")[0].style.color="#80C269";
                    this.reg88=true;
                }else{
                    document.getElementsByClassName("rule77")[0].style.background="red";
                    document.getElementsByClassName("ruletext77")[0].style.color="red";
                     this.reg88=false;
                }
            }
            if (this.form.strPasswd=="") {
                document.getElementsByClassName("rule11")[0].style.background="grey";
                document.getElementsByClassName("ruletext11")[0].style.color="grey";
                document.getElementsByClassName("rule22")[0].style.background="grey";
                document.getElementsByClassName("ruletext22")[0].style.color="grey";
                document.getElementsByClassName("rule33")[0].style.background="grey";
                document.getElementsByClassName("ruletext33")[0].style.color="grey";
                document.getElementsByClassName("rule44")[0].style.background="grey";
                document.getElementsByClassName("ruletext44")[0].style.color="grey";
                document.getElementsByClassName("rule55")[0].style.background="grey";
                document.getElementsByClassName("ruletext55")[0].style.color="grey";
                document.getElementsByClassName("rule66")[0].style.background="grey";
                document.getElementsByClassName("ruletext66")[0].style.color="grey";
                document.getElementsByClassName("rule77")[0].style.background="grey";
                document.getElementsByClassName("ruletext77")[0].style.color="grey";
                this.$nextTick(()=>{
                    $(".strPasswd22").hide();
                })
            }else{
                if (this.form.strPasswd.length<8||!reg1.test(this.form.strPasswd)||!reg2.test(this.form.strPasswd)||!reg3.test(this.form.strPasswd)||!reg4.test(this.form.strPasswd)||reg5.test(this.form.strPasswd)||reg6.test(this.form.strPasswd)||this.reg88==false) {
                    this.$nextTick(()=>{
                        $('.strPasswd33').removeClass('el-icon-circle-check');
                        $('.strPasswd33').addClass('el-icon-circle-close');
                        $('.strPasswd33').removeClass('right');
                        $('.strPasswd33').addClass('error');
                        $('.strPasswd22').removeClass('right');
                        $('.strPasswd22').addClass('error');
                        $(".strPasswd22").show();
                        $('.strPasswd323').show();
                    })
                }else{
                    this.$nextTick(()=>{
                        $('.strPasswd33').removeClass('el-icon-circle-close');
                        $('.strPasswd33').addClass('el-icon-circle-check');
                        $('.strPasswd33').removeClass('error');
                        $('.strPasswd33').addClass('right');
                        $('.strPasswd22').removeClass('error');
                        $('.strPasswd22').addClass('right');
                        $(".strPasswd22").show();
                        $('.strPasswd323').hide();
                    })
                }
            }
            
        },
        update(){
            // this.name = "admin"
            var reg1 = new RegExp(/(?=.*[A-Z])/);
            var reg2 = new RegExp(/(?=.*[a-z])/);
            var reg3 = new RegExp(/(?=.*[0-9])/);
            var reg4 = new RegExp(/(?=(?:.*?[!@#$%*()_+^&}{:;?.]){1})/);
            var reg5 = new RegExp(/((?:0(?=1)|1(?=2)|2(?=3)|3(?=4)|4(?=5)|5(?=6)|6(?=7)|7(?=8)|8(?=9)|9(?=0)){1,2}\d)/);
            var reg6 = new RegExp(/((?:9(?=8)|8(?=7)|7(?=6)|6(?=5)|5(?=4)|4(?=3)|3(?=2)|2(?=1)|1(?=0)){1,2}\d)/);
            var reg7 = RegExp(this.editform.strUser,"g");
            $(".strPasswdhelp").hide();
            $(".strPasswdhelp2").hide();
            if (this.editform.Newpassword.length>=8) {
                document.getElementsByClassName("rule1")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext1")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule1")[0].style.background="red";
                document.getElementsByClassName("ruletext1")[0].style.color="red";
            }
            if (reg1.test(this.editform.Newpassword)) {
                document.getElementsByClassName("rule2")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext2")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule2")[0].style.background="red";
                document.getElementsByClassName("ruletext2")[0].style.color="red";
            }
            if (reg2.test(this.editform.Newpassword)) {
                document.getElementsByClassName("rule3")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext3")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule3")[0].style.background="red";
                document.getElementsByClassName("ruletext3")[0].style.color="red";
            }
            if (reg3.test(this.editform.Newpassword)) {
                document.getElementsByClassName("rule4")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext4")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule4")[0].style.background="red";
                document.getElementsByClassName("ruletext4")[0].style.color="red";
            }
            if (reg4.test(this.editform.Newpassword)) {
                document.getElementsByClassName("rule5")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext5")[0].style.color="#80C269";
            }else{
                document.getElementsByClassName("rule5")[0].style.background="red";
                document.getElementsByClassName("ruletext5")[0].style.color="red";
            }
            if (this.editform.Newpassword.length>=2) {
                if (reg5.test(this.editform.Newpassword)||reg6.test(this.editform.Newpassword)) {
                    document.getElementsByClassName("rule6")[0].style.background="red";
                    document.getElementsByClassName("ruletext6")[0].style.color="red";
                }else{
                    document.getElementsByClassName("rule6")[0].style.background="#80C269";
                    document.getElementsByClassName("ruletext6")[0].style.color="#80C269";
                }
            }else if(this.editform.Newpassword.length==1){
                document.getElementsByClassName("rule6")[0].style.background="#80C269";
                document.getElementsByClassName("ruletext6")[0].style.color="#80C269";
            }
              if (this.editform.Newpassword) {
                if (reg7.exec(this.editform.Newpassword)==null) {
                    document.getElementsByClassName("rule7")[0].style.background="#80C269";
                    document.getElementsByClassName("ruletext7")[0].style.color="#80C269";
                    this.reg8=true;
                }else{
                    document.getElementsByClassName("rule7")[0].style.background="red";
                    document.getElementsByClassName("ruletext7")[0].style.color="red";
                    this.reg8=false;
                }
            }
            if (this.editform.Newpassword=="") {
                document.getElementsByClassName("rule1")[0].style.background="grey";
                document.getElementsByClassName("ruletext1")[0].style.color="grey";
                document.getElementsByClassName("rule2")[0].style.background="grey";
                document.getElementsByClassName("ruletext2")[0].style.color="grey";
                document.getElementsByClassName("rule3")[0].style.background="grey";
                document.getElementsByClassName("ruletext3")[0].style.color="grey";
                document.getElementsByClassName("rule4")[0].style.background="grey";
                document.getElementsByClassName("ruletext4")[0].style.color="grey";
                document.getElementsByClassName("rule5")[0].style.background="grey";
                document.getElementsByClassName("ruletext5")[0].style.color="grey";
                document.getElementsByClassName("rule6")[0].style.background="grey";
                document.getElementsByClassName("ruletext6")[0].style.color="grey";
                document.getElementsByClassName("rule7")[0].style.background="grey";
                document.getElementsByClassName("ruletext7")[0].style.color="grey";
                this.$nextTick(()=>{
                    $(".strPasswd2").hide();
                })
            }else{
                if (this.editform.Newpassword.length<8||!reg1.test(this.editform.Newpassword)||!reg2.test(this.editform.Newpassword)||!reg3.test(this.editform.Newpassword)||!reg4.test(this.editform.Newpassword)||reg5.test(this.editform.Newpassword)||reg6.test(this.editform.Newpassword)||this.reg8==false) {
                    this.$nextTick(()=>{
                        $('.strPasswd3').removeClass('el-icon-circle-check');
                        $('.strPasswd3').addClass('el-icon-circle-close');
                        $('.strPasswd3').removeClass('right');
                        $('.strPasswd3').addClass('error');
                        $('.strPasswd2').removeClass('right');
                        $('.strPasswd2').addClass('error');
                        $(".strPasswd2").show();
                        $('.strPasswd32').show();
                    })
                }else{
                    this.$nextTick(()=>{
                        $('.strPasswd3').removeClass('el-icon-circle-close');
                        $('.strPasswd3').addClass('el-icon-circle-check');
                        $('.strPasswd3').removeClass('error');
                        $('.strPasswd3').addClass('right');
                        $('.strPasswd2').removeClass('error');
                        $('.strPasswd2').addClass('right');
                        $(".strPasswd2").show();
                        $('.strPasswd32').hide();
                    })
                }
            }
        },
        changepwss(){
            var reg1 = new RegExp(/(?=.*[A-Z])/);
            var reg2 = new RegExp(/(?=.*[a-z])/);
            var reg3 = new RegExp(/(?=.*[0-9])/);
            var reg4 = new RegExp(/(?=(?:.*?[!@#$%*()_+^&}{:;?.]){1})/);
            var reg5 = new RegExp(/((?:0(?=1)|1(?=2)|2(?=3)|3(?=4)|4(?=5)|5(?=6)|6(?=7)|7(?=8)|8(?=9)|9(?=0)){1,2}\d)/);
            var reg6 = new RegExp(/((?:9(?=8)|8(?=7)|7(?=6)|6(?=5)|5(?=4)|4(?=3)|3(?=2)|2(?=1)|1(?=0)){1,2}\d)/);
            var reg7 = RegExp(this.editform.strUser,"g");
            var form=this.editform;
            let root=this.$store.state.IPPORT;
            if (form.Newpassword=="") {
                $('.strPasswd2').hide();
                $('.strPasswd3').hide();
                $('.strPasswd32').hide();
                $(".strPasswdhelp").hide();
                $(".strPasswdhelp2").show();
                return false;
            }else{
                $(".strPasswdhelp2").hide();
            }
            if( form.strPasswd==""||form.Newpassword==""||form.Newpassword1==""){
                return false;
            }
            if( $.md5(form.strPasswd)!==this.$store.state.oldstrPasswd){
                $('.strPasswd').hide();
                $(".oldstrPasswd").show();
                return false;
            }else{
                $(".oldstrPasswd").hide();
                $('.strPasswd').show();
            }
            if( form.strPasswd==form.Newpassword){
                $('.strPasswd2').hide();
                $('.strPasswd3').hide();
                $('.strPasswd32').hide();
                $(".strPasswdhelp2").hide();
                $(".strPasswdhelp").show();
                return false;
            }else{
                $(".strPasswdhelp").hide();
            }
            if( form.Newpassword!==form.Newpassword1){
                $('.strPasswd5').removeClass('el-icon-circle-check');
                $('.strPasswd5').addClass('el-icon-circle-close');
                $('.strPasswd5').removeClass('right');
                $('.strPasswd5').addClass('error');
                $(".strPasswd4").css('display', 'block');
                $(".strPasswd5").css('display', 'block');
                return false;
            }else{
                $('.strPasswd5').removeClass('el-icon-circle-close');
                $('.strPasswd5').addClass('el-icon-circle-check');
                $('.strPasswd5').removeClass('error');
                $('.strPasswd5').addClass('right');
                $(".strPasswd4").css('display', 'block');
                $(".strPasswd5").css('display', 'block');
            }
            
            if (form.Newpassword.length<8||!reg1.test(form.Newpassword)||!reg2.test(form.Newpassword)||!reg3.test(form.Newpassword)||!reg4.test(form.Newpassword)||reg5.test(form.Newpassword)||reg6.test(form.Newpassword)||this.reg8==false) {
                $('.strPasswd3').removeClass('el-icon-circle-check');
                $('.strPasswd3').addClass('el-icon-circle-close');
                $('.strPasswd3').removeClass('right');
                $('.strPasswd3').addClass('error');
                $('.strPasswd2').removeClass('right');
                $('.strPasswd2').addClass('error');
                $(".strPasswd2").show();
                $('.strPasswd32').show();
                return false;
            }else{
                $('.strPasswd3').removeClass('el-icon-circle-close');
                $('.strPasswd3').addClass('el-icon-circle-check');
                $('.strPasswd3').removeClass('error');
                $('.strPasswd3').addClass('right');
                $('.strPasswd2').removeClass('error');
                $('.strPasswd2').addClass('right');
                $(".strPasswd2").show();
                $('.strPasswd32').hide();
            }
            var url = root + "/api/v1/UpdateUser?user="+encodeURIComponent(form.strUser)+"&oldpassword="+encodeURIComponent($.md5(form.strPasswd))+"&newpassword="+encodeURIComponent($.md5(form.Newpassword))+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.dialogVisible = false;
                        if(form.strUser==this.$store.state.user){
                            this.$router.push({
                                path: '/logout'
                            })
                        }
                        this.$message(this.$t("message.setting.Changecg"));
                    }else{
                        this.$message(this.$t("message.setting.Changesb"));
                    }
                    
                }
            })
        },
        //delete users
        deleteuser(){
            var selectop=this.selectop;
            let root=this.$store.state.IPPORT;
            for(var i=0;i<selectop.length;i++){
                var url = root + "/api/v1/DeleteUser?user="+encodeURIComponent(selectop[i].strUser)+"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            this.tableData=[];
                            this.user();
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
        //Add user
        adduse(){
            var form=this.form;
            console.log(form)
            let root=this.$store.state.IPPORT;
            var reg1 = new RegExp(/(?=.*[A-Z])/);
            var reg2 = new RegExp(/(?=.*[a-z])/);
            var reg3 = new RegExp(/(?=.*[0-9])/);
            var reg4 = new RegExp(/(?=(?:.*?[!@#$%*()_+^&}{:;?.]){1})/);
            var reg5 = new RegExp(/((?:0(?=1)|1(?=2)|2(?=3)|3(?=4)|4(?=5)|5(?=6)|6(?=7)|7(?=8)|8(?=9)|9(?=0)){1,2}\d)/);
            var reg6 = new RegExp(/((?:9(?=8)|8(?=7)|7(?=6)|6(?=5)|5(?=4)|4(?=3)|3(?=2)|2(?=1)|1(?=0)){1,2}\d)/);
            var reg7 = RegExp(this.form.strUser,"g");
            if( form.strPasswd==""&&form.strPasswd1==""){
                this.$message(this.$t("message.setting.beblank"));
                return false;
            }
            if( form.strPasswd1!==form.strPasswd){
                this.$message(this.$t("message.setting.Twopassword"));
                $('.strPasswd55').removeClass('el-icon-circle-check');
                $('.strPasswd55').addClass('el-icon-circle-close');
                $('.strPasswd55').removeClass('right');
                $('.strPasswd55').addClass('error');
                $(".strPasswd44").css('display', 'block');
                $(".strPasswd55").css('display', 'block');
                return false;
            }else{
                $('.strPasswd55').removeClass('el-icon-circle-close');
                $('.strPasswd55').addClass('el-icon-circle-check');
                $('.strPasswd55').removeClass('error');
                $('.strPasswd55').addClass('right');
                $(".strPasswd44").css('display', 'block');
                $(".strPasswd55").css('display', 'block');
            }
            if (form.strPasswd.length<8||!reg1.test(form.strPasswd)||!reg2.test(form.strPasswd)||!reg3.test(form.strPasswd)||!reg4.test(form.strPasswd)||reg5.test(form.strPasswd)||reg6.test(form.strPasswd)||this.reg88==false) {
                $('.strPasswd33').removeClass('el-icon-circle-check');
                $('.strPasswd33').addClass('el-icon-circle-close');
                $('.strPasswd33').removeClass('right');
                $('.strPasswd33').addClass('error');
                $('.strPasswd22').removeClass('right');
                $('.strPasswd22').addClass('error');
                $(".strPasswd22").show();
                $('.strPasswd323').show();
                return false;
            }else{
                $('.strPasswd33').removeClass('el-icon-circle-close');
                $('.strPasswd33').addClass('el-icon-circle-check');
                $('.strPasswd33').removeClass('error');
                $('.strPasswd33').addClass('right');
                $('.strPasswd22').removeClass('error');
                $('.strPasswd22').addClass('right');
                $(".strPasswd22").show();
                $('.strPasswd323').hide();
            }

            var url = root + "/api/v1/CreateUser?user="+encodeURIComponent(form.strUser)+"&password="+encodeURIComponent($.md5(form.strPasswd))+"&roletoken="+encodeURIComponent(form.strRoleToken)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.dialogFormVisible=false;
                        this.user();
                    }else{
                        this.$message({
                            message: this.$t("message.setting.Creationfailed"),
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
		//Click to edit
		handleClick(index,row){
            this.dialogVisible=true;
			console.log("***",index,row);
            this.$store.state.oldstrPasswd=row.strPasswd;
			this.editform["strUser"]=row.strUser;
            this.$nextTick(()=>{
                if (this.dialogVisible==true) {
                    document.getElementsByClassName("strPasswd")[0].style.display="none";
                    document.getElementsByClassName("strPasswd2")[0].style.display="none";
                    document.getElementsByClassName("strPasswd4")[0].style.display="none";
                    document.getElementsByClassName("strPasswdhelp")[0].style.display="none";
                    document.getElementsByClassName("strPasswdhelp2")[0].style.display="none";
                    document.getElementsByClassName("oldstrPasswd")[0].style.display="none";
                    document.getElementsByClassName("strPasswd5")[0].style.display="none";
                    document.getElementsByClassName("strPasswd55")[0].style.display="none";
                }
            })
		},
        // Selected function
        selectCall(row){
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    strUser:row[i].strUser,
                    strPasswd:row[i].strPasswd,
                    strUserType:row[i].strUserType,
                };
                this.selectop.push(selectop);
            }
            
        },
        select_Call(row){
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    strUser:row[i].strUser,
                    strPasswd:row[i].strPasswd,
                    strUserType:row[i].strUserType,
                };
                this.selectop.push(selectop);
            }
        },
        // table list
        user(){
            let root=this.$store.state.IPPORT;
            var url = root + "/api/v1/GetUserList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log("***",result);
                if(result.status==200){
                    this.tableData=result.data.users;
                    this.total=this.tableData.length
                }
            })
        },
        rolelist(){
            let root=this.$store.state.IPPORT;
            var url1 = root + "/api/v1/GetRoleList?session="+ this.$store.state.token;
            this.$http.get(url1).then(result=>{
                console.log("**--*",result);
                if(result.status==200){
                    var data=result.data.roles;
                    for(var i=0;i<data.length;i++){
                        var Role={
                            value: data[i].strRoleToken,
                            label: data[i].strRoleToken
                        }
                        this.Role.push(Role);
                    }
                    
                        console.log(this.Role)
                }
            })
        },
        
        handleSizeChange1(val) {
            this.currentPage = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            this.currentPage = val;
        },
	}
}
</script>

<style lang="scss" scoped>
.dasboard_global{
	/* popup input */
	.user_pop{
		.pop_but{
			width: 60px;
			background:rgba(50,113,255,1);
			border-radius:5px;
			padding: 5px 10px;
		}
		.editinput{
			border:1px solid rgba(181,181,181,1);
			-webkit-appearance: none;
			background:none;
			border-radius: 4px;
			-webkit-box-sizing: border-box;
			box-sizing: border-box;
			display: inline-block;
			font-size: inherit;
			height: 40px;
			line-height: 40px;
			outline: 0;
			padding: 0 15px;
			-webkit-transition: border-color .2s cubic-bezier(.645,.045,.355,1);
			transition: border-color .2s cubic-bezier(.645,.045,.355,1);
			width: 100%;
		}
	}
    .updatepassword{
        .el-dialog{
            .el-dialog__body {
                padding-left: 10%;
                .el_form{
                    width: 100%; 
                    .el-form-item{
                        color: black !important;
                        .el-input{
                            width: 300px !important;
                        }
                        .strPasswd{
                            margin-left:5% !important; 
                        }
                        .strPasswdhelp{
                            margin-left:5% !important; 
                        }
                        .strPasswdhelp2{
                            margin-left:5% !important; 
                        }
                        .oldstrPasswd{
                            margin-left:5% !important; 
                        }
                        .strPasswd2{
                            margin-left:5% !important; 
                            i{
                                margin-right:10px !important;
                            }
                        }
                        .strPasswd22{
                            margin-left:5% !important; 
                            i{
                                margin-right:10px !important;
                            }
                        }
                        .strPasswd4{
                            margin-left:5% !important; 
                            align-items:center;
                        }
                        .strPasswd44{
                            margin-left:5% !important; 
                            align-items:center;
                        }
                        .right{
                            color:#80C269 !important;
                        }
                        .error{
                            color: #EB3700 !important;
                        }
                    }
                    .updateHelp{
                        margin-left: 12% !important;
                        color:grey;
                    }
                    .rule {
                        list-style: none;
                        margin-left: 6% !important;
                        margin-top: 1% !important;
                        li{
                            margin-bottom: 1px;
                            list-style: none;
                            color:grey;
                            .rulechildren{
                                height: 10px;
                                width: 10px;
                                border-radius: 5px;
                                background-color: grey;
                                display: inline-block;
                                margin-right: 10px;
                            }
                        }
                    }
                    .confirmpassword{
                        /deep/.el-form-item__content{
                            display:flex;
                            flex-direction:row;
                            .strPasswd5{
                                line-height: 40px;
                            }
                        }
                    }
                    .addconfirmpassword{
                        /deep/.el-form-item__content{
                            display:flex;
                            flex-direction:row;
                            .strPasswd55{
                                line-height: 40px;
                            }
                        }
                    }
                }
            }
        }
    }
}

</style>