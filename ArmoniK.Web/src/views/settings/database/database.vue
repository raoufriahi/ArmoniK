<template>
    <div class="database">
        <div>
            <div style="padding-bottom:15px; font-size:16px;">Database configuration</div>
            <el-button :class="sqlite?'button_liveview':'button_liveview1'" type="primary"  size="mini" @click='database1'>SQLITE</el-button>
            <el-button :class="!sqlite?'button_liveview':'button_liveview1'" type="primary"  size="mini" @click='database2'>POSTGRES</el-button>
            <div v-if="status" style="display:inline;padding-left:300px;">Connection succeeded <span style="color:#a7f55a;"> ●</span></div>
            <div v-else style="display:inline;padding-left:300px;">Connection failed <span style="color:red;"> ●</span></div>
        </div>
        <div class="database_bot">
            <div class="Cluster_block_content">
                <el-form class="Cluster_form el_form" :class="yincang" label-position="top" label-width="100px" :model="form">
                    <el-form-item :label="label.host" style="padding-right:10px;">
                        <el-input v-model="form.host" :disabled='disabled'></el-input>
                    </el-form-item>
                    <el-form-item :label="label.port" style="padding-right:10px; width:120px;">
                        <el-input v-model="form.port" :disabled='disabled'></el-input>
                    </el-form-item>
                    <el-form-item :label="label.database" style="padding-right:10px; width:150px;">
                        <el-input v-model="form.database" :disabled='disabled'></el-input>
                    </el-form-item>
                    <el-form-item :label="label.username" style="padding-right:10px; width:150px;">
                        <el-input v-model="form.username" :disabled='disabled'></el-input>
                    </el-form-item>
                    <el-form-item :label="label.password" style="padding-right:10px;">
                        <el-input v-model="form.password" :disabled='disabled'></el-input>
                    </el-form-item>
                </el-form>
            </div>
        </div>
        <div class="Cluster_but LowerPart">
            <el-button class="save" @click="SetRedis" size="mini">{{$t("message.camera.save")}}</el-button>
        </div>
    </div>
</template>
<script>
export default {
    data(){
        return{
            status:false,
            sqlite:true,
            disabled:false,
            yincang:'yincang',
            label:{
                host:this.$t("message.database.host"),
                port:this.$t("message.database.port"),
                database:this.$t("message.database.database"),
                username:this.$t("message.database.username"),
                password:this.$t("message.database.password"),
            },
            form: {
                host:"127.0.0.1",
                port:"5432",
                database:"V1Event",
                username:"Root",
                password:"Root123",
            },
        }
    },
    methods:{
        database1(){
            this.sqlite=true;
            this.disabled = true;
            this.yincang = 'yincang';
        },
        database2(){
            this.sqlite=false;
            this.disabled = false;
            this.yincang = '';
        },
        SetRedis(){
            let url;
            if (this.sqlite) {
                url = this.$store.state.IPPORT + "/api/v2/GetPostgres?type=sqlite&session="+ this.$store.state.token;
            }else{
                url = this.$store.state.IPPORT + "/api/v2/GetPostgres?type=postgres&host="+this.form.host+"&port="+this.form.port+"&database="+this.form.database+"&username="+this.form.username+"&password="+this.form.password+"&session="+ this.$store.state.token;
            }
            this.$http.get(url).then(result=>{
                console.log(result,url);
                if (result.status === 200) {
                    if (result.data.bStatus) {
                        this.$root.bus.$emit('webrtc',true);
                        this.$message('Please restart after modification');
                    }else{
                        this.$message('Fail to edit');
                    }
                }
            })
        },
        databasetype(){
            let url = this.$store.state.IPPORT + "/api/v2/GetPostgresType?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result,url);
                if (result.data.type == 'sqlite') {
                    this.sqlite = true;
                    this.disabled = true;
                    this.yincang = 'yincang';
                }else{
                    this.sqlite = false;
                    this.disabled = false;
                    this.yincang = '';
                    this.form=result.data.postgres;
                }
            })
        },
        databasestatus(){
            let url = this.$store.state.IPPORT + "/api/v2/GetDatabaseStatus?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.data) {
                    this.status = true;
                }else{
                    this.status = false;
                }
            })
        }
    },
    mounted(){
        this.databasetype();
        this.databasestatus();
    }
    
}
</script>
<style lang="scss" scoped>
.button_liveview{
    font-size: 12px;
    padding: 5px 14px;
    background-color: #3ABCFE;
    border: 1px solid #3ABCFE;
}
.button_liveview1{
    font-size: 12px;
    padding: 5px 14px;
    background:none;
    border: 1px solid #3ABCFE;
}
.Cluster_but{
    // text-align: center;
    display: flex;
    justify-content: flex-end;
    width: 100%;
    padding: 15px 0px;
    .save{
        font-size: 14px;
        padding: 7px 20px;
        background: rgb(58, 188, 254);
        border: 0px none;
        color: white;
    }
}
</style>