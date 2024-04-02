<template>
    <div class="playback">
        <div class="playback_snap_zuo">
            <!-- 模糊查询搜查 -->
            <el-input
                class="snap_zuo_input"
                placeholder="Enter keywords to filter"
                v-model="filterText">
            </el-input>
            <div class="snap_zuo_title">Device</div>
            <!-- 这是原下拉框代码 -->
            <el-tree
                :data="data"
                show-checkbox
                node-key="id"
                :filter-node-method="filterNode"
                ref="tree"
                highlight-current
                :props="defaultProps">
                <span slot-scope="{ data }">
                    <i :class="data.iconclass" style="font-size:10px;"></i>
                    <span :class="data.iconclass1" style="padding-left: 4px;">{{data.label||data.strName}}</span>
                    <span v-if="data.length" style="padding-left: 4px;">{{data.online}}/{{data.length}}</span>
                    <el-popover placement="bottom-end" trigger="manual" v-if="data.firstLevel" class="Status" popper-class="popperOptions" style="flex: 1; text-align: right;height: 36px;line-height: 36px;" :ref="refNamePopover+data.statusID">
                        <div class="StatusPrompt" >
                            <div style="width:100%;display: flex;justify-content: space-between;">
                                <span style="font-size:12px;">
                                    <span style="padding-left:5px">
                                        <i class="iconfont icon-shexiangji" style="font-size:12px;"></i>
                                        在线:{{data.online}}
                                    </span>
                                    <span class="el-tree-camera" style="padding-left:5px">
                                        <i class="iconfont icon-shexiangji el-tree-camera" style="font-size:12px;"></i>
                                        离线:{{data.offline}}
                                    </span>
                                    <span class="el-tree-camera" style="padding-left:5px">
                                        <i class="iconfont icon-jinyong1 el-tree-camera " style="font-size:12px;"></i>
                                        禁用:{{data.disable}}
                                    </span>
                                    <span class="el-tree-camera" style="padding-left:5px">
                                        <i class="iconfont icon-kaiqishexiangtou1 el-tree-camera" style="font-size:12px;"></i>
                                        空闲:{{data.idle}}
                                    </span>
                                </span>
                            </div>
                        </div>
                        <el-button  slot="reference" type="text"  class="iconfont icon-xianshi2 statusBtn" style="margin-left: 20px;font-size:12px" @click="StatusPrompt(data.statusID,$event)"></el-button>
                    </el-popover>
                </span>
            </el-tree>
        </div>
        <div class="playback_snap_you">
            <div class="snap_you_top">
                <div class="snap_you_topfast">
                    <div class="snap_you_topinterval">Date</div>
                    <el-select v-model="value" placeholder="Please choose" @change="datechange">
                        <el-option
                        v-for="item in selectdate"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                        </el-option>
                    </el-select>
                </div>
                <div class="snap_you_topdate">
                    <div class="snap_you_topinterval">Start date</div>
                    <el-date-picker
                        v-model="startvalue"
                        type="datetime"
                        placeholder="Choose date and time">
                    </el-date-picker>
                    <div class="snap_you_topinterval1"> —— </div>
                    <div class="snap_you_topinterval">End date</div>
                    <el-date-picker
                        v-model="endvalue"
                        type="datetime"
                        placeholder="Choose date and time">
                    </el-date-picker>
                </div>
                <div class="snap_you_topbutt">
                    <el-button class="snap_you_topbutt1" @click="getCheckedNodes">{{$t("message.archive.search")}}</el-button>
                    <el-button class="snap_you_topbutt2" @click="tableDatak">{{$t("message.archive.Clear")}}</el-button>
                </div>
            </div>
            <div class="snap_you_bottom">
                <!-- 有按钮 -->
                <el-table
                    stripe
                    :data="tableData1.slice((currentPage-1)*pageSize,currentPage*pageSize)"
                    style="width: 100%;">
                    <el-table-column
                        prop="token"
                        :label="label.Name" >
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">{{ scope.row.token }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column
                        prop="name"
                        :label="label.Token">
                            <template slot-scope="scope">
                            <span>{{ scope.row.name }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column
                        prop="starf"
                        :label="label.Time">
                            <template slot-scope="scope">
                            <i class="el-icon-time"></i>
                            <span>{{ scope.row.starf }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column
                        fixed="right">
                        <template slot-scope="scope">
                            <div class="snap_you_lable_butt">
                                <a :href="scope.row.url+'?session='+session" :download="scope.row.urlto"><button type="button" style="margin-right: 40px;" class="iconfont icon-download"></button></a>
                                <button type="button" class="iconfont icon-play" @click="Refresh1(scope.$index, scope.row)" data-toggle="modal" data-target="#myModal"></button>
                            </div>
                        </template>
                        </el-table-column>
                </el-table>
                <!-- 分页 -->
                <el-pagination
                    style="text-align: center;"
                    layout="prev, pager, next"
                    @size-change="handleSizeChange" 
                    @current-change="handleCurrentChange"
                    :current-page="currentPage"
                    :total="total">
                </el-pagination>
            </div>
        </div>
        <el-dialog
            class="dasboard_modal"
            :title="label.picture"
            :visible.sync="myModal1"
            width="25%">
            <div>{{$t("message.archive.Time")}}:{{rowstarf}}</div>
            <span slot="footer" class="dialog-footer">
                <img :src="url+'?session='+session" class="imgmin"/>
            </span>
        </el-dialog>
    </div>
</template>

<script>
import {listdatagload,listdatag,AllMethods} from '../public/regional'
export default {
    
    name:"screenshots",
    data() {
        return {
            label:{
                Name:this.$t("message.table.Name"),
                Token:this.$t("message.table.Token"),
                Time:this.$t("message.table.Time"),
                Picture:this.$t("message.archive.Picture")
            },
            timelink:0,//滑块
            max:0,//滑块最大值
            value: '',
            session:this.$store.state.token,
            endvalue: new Date().getTime(),
            startvalue:new Date().getTime() - 3600 * 1000 * 1,
            myModal1:false,
            selectdate: [{
                    value: (new Date().getTime() - 3600 * 1000 * 1),
                    label: this.$t("message.archive.Onehour")
                }, {
                    value: (new Date().getTime() - 3600 * 1000 * 24),
                    label: this.$t("message.archive.Oneday")
                }, {
                    value: (new Date().getTime() - 3600 * 1000 * 24 * 7),
                    label: this.$t("message.archive.Oneweek")
                }, {
                    value: (new Date().getTime() - 3600 * 1000 * 24 * 30),
                    label: this.$t("message.archive.Onemonth")
            }],
            //分页
            currentPage: 1, // 当前页码
            total: 0, // 总条数
            pageSize: 10,//一页数量
            search: '',
            filterText: '',
            data: [],
            defaultProps: {
                children: this.$store.state.root=="Operator"?'cam':'children',
                label: this.$store.state.root=="Operator"?'strName':'label',
                token:this.$store.state.root=="Operator"?'strToken':"token",
                iconclass:"iconclass",
                online:'online',
            },
            tableData1: [],
            rowstarf:"",//跟进进度条开始时间
            url:"",//图片地址
            refNamePopover: 'popover-', // popover ref名称前缀
        }
    },
    created(){
        if(this.$store.state.root=="Operator"){
        }else{
            AllMethods().then(res=>{
                this.data = res.listdatag;
            }).catch(err=>{
                console.log(err);
            })
        }
    },
    mounted(){
        if(this.$store.state.root=="Operator"){
			this.Regional()
		}
    },
    methods:{
        // 在线离线状态提示
        StatusPrompt(id,e){
            let refName = this.refNamePopover + id;
            if (this.$refs[refName].showPopper) {
                this.$refs[refName].doClose();
                for (let i = 0; i < e.currentTarget.classList.length; i++) {
                    e.currentTarget.classList.remove('StatusPromptColor')
                    e.stopPropagation();
                    return;
                }
            }else{
                this.$refs[refName].doShow();
                for (let i = 0; i < e.currentTarget.classList.length; i++) {
                    e.currentTarget.classList.add('StatusPromptColor')
                    e.stopPropagation();
                    return;
                }
            }
        },
         Regional(){
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v1/GetRegion?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                var oldarr=result.data.root;
                var oldarr1=result.data.src;
                // console.log(oldarr,oldarr1)
                var dataroot=this.getchild(oldarr,oldarr1);
                // this.data1.push(dataroot);
                console.log(dataroot);
                this.datapush(dataroot)
                console.log(this.data1);
            })
        },
        datapush(data){
            this.a(data)
        },
        a(data){
            console.log(data);
            if(data.node.length!=0){
                for (let i = 0; i < data.node.length; i++) {
                data.cam.push(data.node[i])
            }
            }
            delete data.node
            for (let i = 0; i < data.cam.length; i++) {
                console.log(data.cam[i].strName);
                if (data.cam[i].strName!='设备暂时不可用') {
                    this.b(data.cam[i])
                    this.data=[];
                    this.data.push(data);
                }
            }
        },
        b(data){
            if (data.name) {
                return
            }
            if(data.node.length!=0){
                for (let i = 0; i < data.node.length; i++) {
                    data.cam.push(data.node[i])
                }
            }
            delete data.node
            for (let i = 0; i < data.cam.length; i++) {
                if (data.cam[i].strName!='设备暂时不可用') {
                    console.log(data.cam[i]);
                    if (data.cam[i].cam) {
                        this.c(data.cam[i])
                    }
                }
            }
        },
        c(data){
            if (data.name) {
                return
            }
            if(data.node.length!=0){
                for (let i = 0; i < data.node.length; i++) {
                data.cam.push(data.node[i])
            }
            }
            delete data.node;
            for (let i = 0; i < data.cam.length; i++) {
                if (data.cam[i].strName!='设备暂时不可用') {
                    console.log(data.cam[i]);
                    if (data.cam[i].cam) {
                        this.d(data.cam[i])
                    }
                }
            }
        },
        d(data){
            if (data.name) {
                return
            }
            if(data.node.length!=0){
                for (let i = 0; i < data.node.length; i++) {
                data.cam.push(data.node[i])
            }
            }
            delete data.node;
            for (let i = 0; i < data.cam.length; i++) {
                if (data.cam[i].strName!='设备暂时不可用') {
                console.log(data.cam[i]);
                if (data.cam[i].cam) {
                    this.e(data.cam[i])
                }}
            }
        },
        e(data){
            if (data.name) {
                return
            }
            if(data.node.length!=0){
                for (let i = 0; i < data.node.length; i++) {
                    if (data.cam[i].strName!='设备暂时不可用') {
                        data.cam.push(data.node[i])
                    }
            }
            }
            delete data.node;
        },
        getchild(arr,arr1) {
            for(var i in arr.cam){
                if(!arr.cam[i].strName){
                    for(var j in arr1){
                        if(arr.cam[i].strToken == arr1[j].strToken){
                            var node1=[{
                                strToken : arr1[j].strToken,
                                streamprofile : "main",
                                strName :this.$t('message.live.mainstream'),
                                name:arr1[j].strName+"--"+this.$t('message.live.mainstream'),
                                iconclass : 'mdi mdi-playlist-play fa-fw',
                                disabled_me:false
                            },{
                                strToken : arr1[j].strToken,
                                streamprofile : "sub",
                                strName :this.$t('message.live.substream'),
                                name:arr1[j].strName+"--"+this.$t('message.live.substream'),
                                iconclass : 'mdi mdi-playlist-play fa-fw',
                                disabled_me:false
                            }]
                            arr.cam[i].node=node1;
                            arr.cam[i].strName = arr1[j].strName;
                            arr.cam[i].name=arr1[j].strName+"--"+this.$t('message.live.mainstream')
                            arr.cam[i].bOnline = arr1[j].bOnline;
                            arr.cam[i].iconclass="iconfont  icon-shexiangji"
                            arr.cam[i].disabled_me=false
                            if(!arr1[j].bOnline){
                                // arr.cam[i].iconclass = 'iconfont icon-kaiqishexiangtou';
                                arr.cam[i].iconclass = 'iconfont icon-shexiangji el-tree-camera';
                                arr.cam[i].iconclass1 = 'el-tree-camera';
                            }
                            if(arr1[j].nConnectType == 'H5_CLOUD')
                                arr.cam[i].iconclass = 'iconfont  icon-shexiangji';

                            if(arr1[j].bRec == true)
                                // arr.cam[i].iconclass2  = 'iconfont icon-radioboxfill none';
                                arr.cam[i].iconclass2= 'none';

                            if(arr1[j].bDisable== true){
                                
                                arr.cam[i].node[0].disabled_me=true;
                                arr.cam[i].node[1].disabled_me=true;
                                arr.cam[i].disabled_me=true;
                                // arr.cam[i].iconclass1= 'camera';
                                arr.cam[i].iconclass= 'iconfont icon-jinyong1 el-tree-camera';
                                arr.cam[i].iconclass1 = 'el-tree-camera';
                            }
                            if (arr1[j].bIdle == true) {
                                arr.cam[i].iconclass = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                                arr.cam[i].iconclass1 = 'el-tree-camera';
                            }
                        }
                    }
                }
            }

            var camarr=[]
			var camarr1=[]
			for(var i in arr.cam){
				camarr.push(arr.cam[i].strToken)
			}
			for(var i in arr1){
				camarr1.push(arr1[i].strToken)
			}
			let diff = camarr.filter(item => !new Set(camarr1).has(item))
			// console.log(diff)
			for(var i in arr.cam){
				for(var j in diff){
					if(arr.cam[i].strToken == diff[j]){
						arr.cam[i].strName = '设备暂时不可用';
						// arr.cam[i].iconclass="iconfont  icon-kaiqishexiangtou"
                        arr.cam[i].iconclass = 'iconfont icon-shexiangji el-tree-camera';
                        arr.cam[i].iconclass1 = 'el-tree-camera';
					}
				}
			}

            if(arr.node && arr.node.length>0){
                for (var i = 0; i < arr.node.length; i++) {
                    arr.node[i] = this.getchild(arr.node[i],arr1);
                }
            }
            return arr;
        },
        datechange(){
            console.log(this.value)
            this.startvalue=this.value
        },
        //播放
        Refresh1(index, row){
            console.log(index, row);
            this.rowstarf=row.starf;
            this.url=row.url;
            this.myModal1=true
        },
       
        // 表格归档 下载 刷新
        
        //按钮搜索
        async getCheckedNodes() {
            this.tableData1=[];
            console.log("node值",this.$refs.tree.getCheckedNodes());
            var nodes=this.$refs.tree.getCheckedNodes();
            var srartdate=new Date(this.startvalue).toISOString()+"08:00";
            var enddate= new Date(this.endvalue).toISOString()+"08:00";
            console.log(this.endvalue,this.startvalue,srartdate,enddate)
            for(var i=0 ;i<nodes.length; i++){
                if(nodes[i].token||nodes[i].strToken!=undefined){
                    console.log(nodes[i].token||nodes[i].strToken,nodes[i].label||nodes[i].strName)
                    var label=nodes[i].label||nodes[i].strName
                    // return false
                    var token = nodes[i].token||nodes[i].strToken;
                    var url = this.$store.state.IPPORT + "/api/v1/Search?type=snapshot&token="+token+"&start="+srartdate+"&end="+enddate+"&session="+ this.$store.state.token;                    console.log(url);
                    //return false;
                    await this.$http.get(url).then(result=>{
                        if(result.status == 200){
                            this.$message('Query successful');
                            var data=result.data;
                            for(var i=0;i<data.record.length;i++){
                                var item=data.record[i];
                                var urlto=item["strPath"].split("/");
                                var timeitem={
                                        name: item.strToken,
                                        token: label,
                                        starf : item['strStartTime'],
                                        percentage:0,
                                        url:item["strPath"],
                                        urlto:urlto[urlto.length-1],
                                        strFileName:""
                                    };
                                    this.tableData1.push(timeitem);
                            }
                            
                            this.total=this.tableData1.length;
                            console.log("length",this.total)
                        }
                    }).catch(error => {
                        console.log('Snapshot failed!', error);
                        this.$message('Query failed !');
                    });
                }
            }
        },
        //清空表格
        tableDatak(){
            this.tableData1=[];
            this.total=0;
        },
        //分页
        handleSizeChange(val) {
            console.log(`每页 ${val} 条`);
            this.currentPage = 1;
            this.pageSize = val;
        },
        handleCurrentChange(val) {
            console.log(`当前页: ${val}`);
            this.currentPage = val;
        },
        //模糊查询
        filterNode(value, data) {
            if (!value) return true;
            if (this.$store.state.root=="Operator") {
				return data.strName.indexOf(value) !== -1;
			}else{
				return data.label.indexOf(value) !== -1;
			}
        }
    },
     //模糊查询
    watch: {
      filterText(val) {
        this.$refs.tree.filter(val);
      }
    }
    
}
</script>

<style lang="scss" scoped>
.playback{
    display: flex;
    justify-content: space-between;
    .playback_snap_zuo{
        width: 18%;
        height: 86vh;
        .snap_zuo_title{
            width: 100%;
            padding: 12px 20px;
            font-size: 16px;
            font-family: PingFang SC;
            font-weight: 500;
            margin-bottom: 10px;
        }
        .StatusPromptColor{
            color:#67cfb5 !important;
        }
    }
    .playback_snap_you{
        width: 82%;
        height: 86vh;
        .snap_you_top{
            padding: 15px 20px;
            @extend .g_flex;
            justify-content: space-between;
            align-items: center;
            width: 100%;
            .el-input__inner{
                background: none;
                padding: 0;
            }
            .snap_you_topfast{
                width: 17%;
                min-width: 240px;
                @extend .g_flex;
                justify-content: space-around;
                align-items: center;
            }
            .snap_you_topdate{
                width: 45%;
                min-width: 600px;
                @extend .g_flex;
                justify-content: space-around;
                align-items: center;
            }
            .snap_you_topbutt{
                width: 15%;
                min-width: 160px;
                @extend .g_flex;
                justify-content: space-around;
                align-items: center;
                .snap_you_topbutt1{
                    background: #3ABBFE;
                    border: none;
                }
            }
        }
        .snap_you_bottom{
            .button_edi button{
                border: 0;
                background:none;
                font-size: 24px;
                margin-right: 40px;
                button:last-child{
                    margin-right: 0;
                }
            }
            .snap_you_lable_butt{
                width: 100%;
                display: flex;
                button{
                    background: none;
                    border: none;
                    font-size: 24px;
                }
            }
        }
    }
}
.imgmin{
    width: 100%;
}
.g_flex{
    display: flex;
}
</style>