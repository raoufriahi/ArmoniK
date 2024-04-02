<template>
    <div class="login_back">
        <el-dialog
            class="updatepassword"
            title="Change Password"
            :visible.sync="dialogVisible"
            width="60%"
            append-to-body>
             <el-form class="el_form" label-position="left" label-width="150px" :model="editform">
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
                <el-button type="primary" @click="changepwss()">Sure</el-button>
                <el-button @click="dialogVisible = false">Cancel</el-button>
            </span>
        </el-dialog>
        <div class="Copyrightnotice" id="Copyrightnotice">
            The system license will expire in {{capability}} days, please purchase the license in time to avoid affecting normal use.
        </div>
        <!-- <div class="login_but">
            <router-link :to="{name:'Downloadapp'}">
                <el-button class="but_jump" plain>{{this.$t("message.archive.Download")}}</el-button>
            </router-link>
        </div> -->
        <div class="login_head">
            <div class="login_title"></div>
            <div class="login_content">
                <CCol sm="12">
                    <CCard class="content_beck">
                        <CCardBody>
                            
                            <CForm novalidate @submit.stop.prevent="login">
                                <div class="prompt" id="prompt">
                                    <i class="iconfont icon-ts-caveat"></i> <span>The username or password is wrong, please re-enter the remaining times{{frequency}}Second-rate.</span>
                                </div>
                                <div class="prompt" id="prompt1">
                                    <i class="iconfont icon-ts-caveat"></i> <span>Too many errors, the account is locked, please{{lockingdate}}try again in minutes!</span>
                                </div>
                                <CInput placeholder="Username" v-model="name">
                                    <template #prepend-content><i class="content_icon iconfont icon-yonghuming"></i> </template>
                                </CInput>
                                <!-- <CInput placeholder="password" type="password" v-model="passw">
                                    <template #prepend-content><i class="content_icon iconfont icon-suozi"></i> </template>
                                </CInput> -->
                                <div class="LoginPassword">
                                    <span class="suozi">
                                        <i class="iconfont icon-suozi"></i>
                                    </span>
                                    <el-input class="passwordInput" placeholder="password" show-password v-model="passw">
                                    </el-input>
                                </div>
                                <div class="content_lang">
                                    <div class="lang_size">Language</div>
                                    <!-- <CSelect
                                    class="a"
                                        horizontal
                                        :options="selectOptions"
                                        placeholder="Please select"
                                    /> -->
                                    <!-- <i-select class="lang_back" :model="model1" style="width:200px">
                                        <i-option v-for="(item,index) in cityList" :key="index" :value="item.value">{{ item.label }}</i-option>
                                    </i-select> -->
                                    
                                    <el-select class="lang_back" v-model="lang" @change="langchang" placeholder="请选择">
                                        <el-option
                                            v-for="item in cityList"
                                            :key="item.value"
                                            :label="item.label"
                                            :value="item.value">
                                        </el-option>
                                    </el-select>
                                </div>
                                <div class="form-actions">
                                    <CButton class="form_butt" type="submit">
                                        {{$t("message.login.login")}}
                                    </CButton>
                                </div>
                            </CForm>
                        </CCardBody>
                    </CCard>
                </CCol>
            </div>
            <div class="login_buttom">{{date}} © Heimdall.com</div>
        </div>
    </div>
</template>
<script>
import '../assets/js/jQuery.md5'
export default {
    data() {
        return {
            date:new Date().getFullYear(),
            name:"",
            passw:"",
            capability:"",
            dialogVisible: false,
            selectOptions: [
                'English', 'Français',
            ],
            lang:this.$store.state.lang,
            cityList: [
                {
                    value: 'en',
                    label: 'English'
                },
                {
                    value: 'zhchs',
                    label: 'Français'
                },
            ],
            e1:undefined,
            frequency:0,
            lockingdate:0,
            dialogVisible:false,
            editform: {
                strUser:"",
                strPasswd: "",
                strUserType: "",
                Newpassword:"",
                Newpassword1:""
            },
            reg8:true
        };
    },
    mounted(){
        $("#prompt").hide();
        $("#prompt1").hide();
        //this.GetSystemInfo();
        console.log(sessionStorage.getItem('mcutoken'),sessionStorage.getItem('mcuuser'),sessionStorage.getItem('mculang'),sessionStorage.getItem('mcuroot'),this.$store.state.token,this.$store.state.user)
        if (this.$store.state.e1) {
            this.$store.state.e1.disconnect();
        }
        if (this.$store.state.e2) {
            this.$store.state.e2.disconnect();
        }
    },
    methods: {
        // Monitor whether the duplicate password is the same as the new password
        confirmpassword(){
            var form=this.editform;
            if( form.Newpassword!==form.Newpassword1){
                // this.$message(this.$t("message.setting.Twopassword"));
                this.$nextTick(()=>{
                    $('.strPasswd5').removeClass('el-icon-circle-check');
                    $('.strPasswd5').addClass('el-icon-circle-close');
                    $('.strPasswd5').removeClass('right');
                    $('.strPasswd5').addClass('error');
                    $(".strPasswd4").css('display', 'block');
                    $(".strPasswd5").css('display', 'block');
                })
                
            }else{
                this.$nextTick(()=>{
                     $('.strPasswd5').removeClass('el-icon-circle-close');
                    $('.strPasswd5').addClass('el-icon-circle-check');
                    $('.strPasswd5').removeClass('error');
                    $('.strPasswd5').addClass('right');
                    $(".strPasswd4").css('display', 'block');
                    $(".strPasswd5").css('display', 'block');
                })
            }
        },
        // Monitor the value of the new password input box
        update(){
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
        // Confirm to change the password
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
                 this.$message(this.$t("Please enter password"));
                return false;
            }
            if( $.md5(form.strPasswd)!==this.$store.state.oldstrPasswd){
                // this.$message(this.$t("message.setting.Twopassword"));
                $('.strPasswd').hide();
                $(".oldstrPasswd").show();
                return false;
            }else{
                $(".oldstrPasswd").hide();
                $('.strPasswd').show();
            }
            if( form.strPasswd==form.Newpassword){
                // this.$message(this.$t("message.setting.Twopassword"));
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
                // this.$message(this.$t("message.setting.Twopassword"));
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
                // console.log(url,result)
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
        KeepSession(session){
            let root=this.$store.state.IPPORT;
            var url =root + "/api/v1/Keepalive?session=" +session;
            this.$http.get(url).then(res=>{
                if (res.status == 200) {
                    console.log(res);
                     this.$store.state.token=session
                }
            })
        },
        GetSystemInfo() {
            let root=this.$store.state.IPPORT;
            var url =root + "/api/v1/GetLicEndTime?session=" + this.$store.state.token;
            console.log("------------",url)
            this.$http.get(url).then(result => {
                console.log(result);
                if (result.status == 200) {
                    if (result.data.strEndtime!='unlimited') {
                        var Enddate=new Date(result.data.strEndtime).getTime()
                        var date=new Date().getTime()
                        var Timedifference=1000*60*60*24*31
    
                        console.log(Enddate,date,Timedifference);
    
                        if((Enddate-date)<Timedifference){
                            console.log(date-Enddate,Timedifference,result.data.strEndtime);
                            this.capability=result.data.strEndtime
                            this.$store.state.Certificatetime='false'
                            sessionStorage.Certificatetime = 'false'
                            document.getElementById("Copyrightnotice").style.display='block';
                        }else{
                            this.$store.state.Certificatetime='true'
                            sessionStorage.Certificatetime = 'true'
                        }
                    }else{
                        this.$store.state.Certificatetime='false'
                        sessionStorage.Certificatetime = 'false'
                    }

                    console.log(Enddate-date,Timedifference);
                }
            }).catch(error => {
                console.log("GetSystemInfo", error);
            });
        },
        
        langchang(){
            console.log("111",this.lang)
            this.$i18n.locale=this.lang
            this.$store.state.lang=this.lang
            sessionStorage.mculang=this.lang
            console.log("11221",this.$store.state.lang,this.$i18n.locale)
        },

        login(){
            // return false;
            let root=this.$store.state.IPPORT;
            var url=root+'/api/v1/Login?user='+encodeURIComponent(this.name)+'&password='+encodeURIComponent($.md5(this.passw))+''
             console.log(root,url)
            this.$http.get(url).then(result=>{
                console.log(result)
                if(result.status == 200){
                    if(result.data.bStatus){
                        $("#prompt").hide();
                        $("#prompt1").hide();
                        var data=result.data;
                        console.log(data);
                        if (data.bWeakPassword==true) {
                            this.$store.state.oldstrPasswd=encodeURIComponent($.md5(this.passw));
                            this.$store.state.token=data.strSession
                            sessionStorage.mcutoken = this.$store.state.token
                            this.$message(this.$t("The password is a weak password, please refer to the following rules to modify the password"));
                            this.editform["strUser"]=this.name;
                            this.dialogVisible=true;
                            this.$nextTick(()=>{
                                if (this.dialogVisible==true) {
                                    document.getElementsByClassName("strPasswd")[0].style.display="none";
                                    document.getElementsByClassName("strPasswd2")[0].style.display="none";
                                    document.getElementsByClassName("strPasswd4")[0].style.display="none";
                                    document.getElementsByClassName("strPasswdhelp")[0].style.display="none";
                                    document.getElementsByClassName("strPasswdhelp2")[0].style.display="none";
                                    document.getElementsByClassName("oldstrPasswd")[0].style.display="none";
                                    document.getElementsByClassName("strPasswd5")[0].style.display="none";
                                }
                            })
                        }else{
                            if (sessionStorage.getItem('subscriptType')) {
                                sessionStorage.removeItem('subscriptType')
                                this.$store.state.e1.keepaliveTimer("unsubscribe");
                            }
                            this.$store.state.oldstrPasswd=encodeURIComponent($.md5(this.passw));
                            this.$store.state.token=data.strSession
                            sessionStorage.mcutoken = this.$store.state.token
                            this.loginroot(data.strSession,root);
                            this.KeepSession(data.strSession);
                        }
                    }else{
                        console.log(result.data)
                        if(result.data.nFaultTimes<=3){
                            this.frequency=3-result.data.nFaultTimes;
                            
                            $("#prompt").show();
                            $("#prompt1").hide();
                        }else{
                            this.lockingdate = Math.floor(result.data.tLockTimeResidue/60);
                            $("#prompt1").show();
                            $("#prompt").hide();
                        }
                        if(result.data.nFaultTimes==0){
                            $("#prompt").hide();
                            $("#prompt1").hide();
                        }else if(result.data.tLockTimeResidue==0){
                            $("#prompt1").hide();
                            $("#prompt").hide();
                        }
                        this.$message(this.$t("message.login.login_status_failed"));
                        if (result.data.bWeakPassword==true) {
                            this.$message(this.$t("The password is weak, please contact the administrator to change the password"));
                        }
                    }
                }
            })
        },
        loginroot(strSession,root){
     
            var url=root+'/api/v1/GetUserInfo?user='+encodeURIComponent(this.name)+'&session='+strSession;
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    var data=result.data;
                   
                    this.$store.state.user=data.strUser
                    this.$store.state.root=data.strUserType
                    sessionStorage.mcuuser = this.$store.state.user
                    sessionStorage.mcuroot=this.$store.state.root
                    this.$router.push({
                        path: 'Dashboard'
                    })
                }
            })
        },
    }
};
</script>
<style lang="scss" scopad>

.login_back{
    width: 100%;
    height: 100%;
    position: fixed;
    background: url('../assets/imgs/login_back.png') no-repeat;
    background-size: cover;
    .login_but{
        width: 100%;
        display: flex;
        justify-content: flex-end;
        padding: 0 20px;
        margin-top: 40px;
    }
    /* Copyright */
    .Copyrightnotice{
        position: fixed;
        top: 0;
        width: 100%;
        line-height: 40px;
        text-align: center;
        font-size: 14px;
        font-family: PingFang SC;
        font-weight: 500;
        color: #FFFFFF;
        z-index: 100000;
        background-color: #E62424;
        display: none;
    }
    .login_head{
        text-align: center;
        margin: 12% auto 0;
        .login_title{
            height: 70px;
            margin-bottom: 30px;
            background: url('../assets/imgs/H5s_logo.png')no-repeat center center;
        }
        .login_content{
            margin: 0 auto;
            padding: 20px 2%;
            width:24%;
            background:#FFFFFF;
            // border: 2px solid rgba(30,34,40,0.1);
            box-shadow: -10px -10px 20px 10px rgba(226,226,226,0.3);
            .content_beck{
                background: none;
                border: none;
                /*Error message */
                .prompt{
                    font-size:7px;
                    font-family:PingFang SC;
                    font-weight:500;
                    color:rgba(208,19,19,1);
                }
                .form-group{
                    font-size:10px;
                    font-family:PingFang SC;
                    font-weight:500;
                    color:rgba(34,38,44,1);
                    border-bottom: 1px solid rgba(226,226,226,1);
                    .content_icon{
                        color: #22262C;
                    }
                    .form-control{
                        background: none;
                        border: none;
                    }
                }
                .LoginPassword{
                    display: flex;
                    flex-direction:row;
                    justify-content: flex-start;
                    border-top: none !important;
                    border-left: none !important;
                    border-right: none !important;
                    border-bottom: 1px solid rgba(226,226,226,1);
                    .suozi{
                        display: inline-block;
                        width: 40px;
                        height: 36px;
                        line-height:36px;
                        color: #22262C;
                        padding: 5px 12px;
                    }
                    .passwordInput{
                        .el-input__inner{
                            border: none !important;
                            padding: 6px 12px;
                        }
                        .el-input__inner:focus {
                            -webkit-box-shadow:  0 0 0 0.2rem rgba(50,31,219,0.25);
                            box-shadow: 0 0 0 0.2rem rgba(50,31,219,0.25);
                            border-radius: 0px;
                        }
                        ::-webkit-input-placeholder { /* WebKit browsers */
                            color:grey !important;
                            font-size: 16px;
                        }
                    }
                }
                .input-group-text{
                    background: none;
                    border: none;
                }
                .content_lang{
                    border-bottom: 1px solid rgba(226,226,226,1);
                    .lang_size{
                        text-align: left;
                        font-family:PingFang SC;
                        font-weight:600;
                        color:rgba(34,38,44,1);
                    }
                    .lang_back{
                        width: 100%;
                        .el-input__inner{
                            background: none;
                            border: none;
                        }
                        
                    }
                }
                .form-actions{
                    .form_butt{
                        width: 100%;
                        border: none;
                        background:#3277FF;
                        margin: 20px 0;
                        padding: 5px;
                        font-size:14px;
                        font-family:PingFang SC;
                        font-weight:600;
                        color:rgba(255,255,255,1);
                    }
                }
            }
        }
        .login_buttom{
            position: fixed;
            bottom: 0;
            width: 100%;
            height: 40px;
            line-height: 40px;
            color: #FFFFFF;
            font-size:12px;
            font-family:PingFang SC;
            font-weight:600;
            background: rgba(0,0,0,0.4);
        }
    }
}
.updatepassword{
    .el-dialog{
        .el-dialog__header{
            background: #3277FF !important;
            .el-dialog__title{
                color: #fff !important;
                font-size: 14px;
            }
            .el-dialog__headerbtn .el-dialog__close{
                color: #FFFFFF !important;
            }
        }
        .el-dialog__body {
            padding-left: 10%;
            .el_form{
                width: 100%; 
                // display: flex;
                // flex-flow: row wrap;
                // justify-content: space-between;
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
                    .strPasswd4{
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
                    // padding-left:80px;
                    margin-left: 12% !important;
                }
                .rule {
                    list-style: none;
                    margin-left: 6% !important;
                    margin-top: 1% !important;
                    li{
                        // padding-left:60px;
                        margin-bottom: 1px;
                        list-style: none;
                        .rulechildren{
                            /*Equal width and height*/
                            height: 10px;
                            width: 10px;
                            /*Set the rounded border, the size is half of the width and height, that is, a perfect circle*/
                            border-radius: 5px;
                            /*background color*/
                            background-color: grey;
                            /*Set block to display*/
                            display: inline-block;
                            margin-right: 10px;
                        }
                    }
                }
                .confirmpassword{
                    .el-form-item__content{
                        display:flex;
                        flex-direction:row;
                        .strPasswd5{
                            line-height: 40px;
                        }
                    }
                }
            }
        }
    }
}
</style>