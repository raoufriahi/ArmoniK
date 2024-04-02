<template>
    <div id="page-wrapper">
        <div class="event_left">
        <el-collapse v-model="activeNames">
        <el-collapse-item name="1" id="headswitch">
          <template slot="title">
            <div style="display: flex; justify-content: space-between; width: 85%; align-items: center;">
              <div>Device</div>
            </div>
          </template>
          <el-tree
            :data="data"
            show-checkbox
            node-key="id"
            ref="tree"
            highlight-current
            :props="defaultProps">
            <span slot-scope="{ data }">
                <i :class="data.iconclass" style="font-size:12px;"></i>
                <span :class="data.iconclass1" style="padding-left: 4px;">{{data.label}}</span>
            </span>
          </el-tree>
        </el-collapse-item>
        </el-collapse>
        <div class="changeType" >
            <div style="padding-left:10px;padding-top:10px;padding-bottom:10px;">Type</div>
            <el-checkbox  :indeterminate="isIndeterminate" v-model="checkAll" @change="handleCheckAllChange" style="padding-left:10px;padding-top:10px;padding-bottom:10px;">Select all</el-checkbox>
            <el-checkbox-group  v-model="checkList" @change="changeType" style="padding-left:10px;padding-top:10px;padding-bottom:10px;">
                <el-checkbox  v-for="item in subscribeType" :label="item.value" :key="item.value">{{item.label}}</el-checkbox>
                <!-- <el-checkbox label="H5S_EVENT_ONLINE"></el-checkbox>
                <el-checkbox label="H5S_EVENT_OFFLINE"></el-checkbox>
                <el-checkbox label="H5S_EVENT_MOTION"></el-checkbox> -->
            </el-checkbox-group>
            <el-button style="padding-left:10px;" class="subBtn" @click="subevent">Subscription</el-button>
            <el-button style="padding-left:10px;" class="subBtn" @click="unsubevent">Unsubscribe</el-button>
        </div>
      
        </div>
        <!-- 头部 -->
        <!--  -->
       <div class="event_right">
            <el-table stripe :data="tableData1.slice((currentPage-1)*pageSize,currentPage*pageSize)" style="width: 100%; min-height:82vh;overflow-y: auto;">
            <el-table-column prop="index" :label="G_label.Index" width="70">
                <template slot-scope="scope"> {{scope.$index+1}} </template>
            </el-table-column>
            <el-table-column prop="Type" :label="G_label.Type" width="300"></el-table-column>
            <el-table-column prop="Token" :label="G_label.Tokenevent" width="120"></el-table-column>
            <el-table-column
                prop="UUID"
                :label="G_label.UUID"
                width="160">
            </el-table-column>
            <el-table-column
                prop="Time"
                :label="G_label.Time"
                width="160">
            </el-table-column>
            <el-table-column
                prop="Detail"
                :label="G_label.Detail">
            </el-table-column>
        </el-table>
        <!-- 分页 -->
        <el-pagination
            style="text-align: center;"
            layout=" prev, pager, next,total, jumper"
            @size-change="handleSizeChange" 
            @current-change="handleCurrentChange"
            :current-page="currentPage"
            :total="total">
        </el-pagination>
       </div>
    </div>
</template>

<script>
import {listdatagload,listdatag,AllMethods} from '.././public/regional'
import {IotEventExt} from '../.././assets/js/ioteventext'
import uuid from '../.././assets/js/uuid'
  export default {
    data() {
      return {
        G_label:{
            Index:this.$t("message.table.Index"),
            Type:this.$t("message.table.Type"),
            Tokenevent:this.$t("message.table.Tokenevent"),
            UUID:this.$t("message.table.UUID"),
            Time:this.$t("message.table.Time"),
            Detaill:this.$t("message.table.Detaill")
        },
        //分页
        currentPage: 1, // 当前页码
        total: 0, // 总条数
        pageSize: 10,//一页数量
        tableData1:[],
        tableData3:[],
        
        activeNames: ["1", "2"], //左边
        data: [],
        data1: [],
        defaultProps: {
            children: "children",
            label: "label",
            token: "token",
            iconclass: "iconclass",
        },
        eventType:[],
        eventToken:[],
        subscribeType:[
            {
                label:'Online',
                value:'H5S_EVENT_ONLINE'
            },
            {
                label:'Offline',
                value:'H5S_EVENT_OFFLINE'
            },
            {
                label:'Motion Detection',
                value:'H5S_EVENT_MOTION'
            }
        ],
        checkList:[],
        checkAll: false,
        isIndeterminate: true,
        Eventuuid:uuid(),
      }
    },
    watch:{
		eventdata(data){
			this.tableData1=data;
		}
	},
	computed: {
		eventdata() {
			return this.$store.state.gEvent
		}
	},
    created(){
        AllMethods().then(res=>{
        this.data = res.listdatag;
        }).catch(err=>{
        console.log(err);
        })
    },
    mounted(){
        // console.log(this.realEvent.e1,'88888');
        this.gEventval();   
    },
    methods:{
        // 取消订阅事件
        unsubevent(){
            let obj = {};
            let arr1 =[];
            if (this.$store.state.e1) {
                var uuid = JSON.parse(sessionStorage.getItem('subscriptType'))['uuid'];
                this.$store.state.gEvent=[];
                this.$store.state.e1.keepaliveTimer({types:'unsubscribe',uuid:uuid});
                var nodes=this.$refs.tree.getCheckedNodes();
                var token = JSON.parse(sessionStorage.getItem('subscriptType'))['token'];
                var token2 = JSON.parse(JSON.stringify(token))
                if (nodes.length>0) {
                    for (let s = 0; s < nodes.length; s++) {
                        if (nodes[s].children) {
                            nodes[s].children.forEach((item,index) => {
                                if (token.indexOf(item.token)!=-1) {
                                    token.splice(token.indexOf(item.token), 1);
                                }
                            });
                        }
                    }
                    if (this.checkList.length>0) {
                        let type = JSON.parse(sessionStorage.getItem('subscriptType'))['types'];
                        let propsData = type.filter( item => !this.checkList.some(ele => ele === item));
                        if (token.length>0&&propsData.length>0) {
                            obj.token = token;
                            obj.types =propsData;
                        }else if(token.length==0&&propsData.length>0){
                            obj.token = token2;
                            obj.types =propsData;
                        }else if(token.length>0&&propsData.length==0){
                            obj.token = token;
                            obj.types=["H5S_EVENT_ONLINE","H5S_EVENT_OFFLINE","H5S_EVENT_MOTION"]
                        }else if(token.length==0&&propsData.length==0){
                            obj.token=[];
                            obj.types =[];
                        }
                        obj.uuid=uuid;
                    }else{
                        let type = JSON.parse(sessionStorage.getItem('subscriptType'))['types'];
                        obj.types =type;
                        obj.token = token;
                        obj.uuid=uuid;
                    }
                }else{
                    if (this.checkAll==true) {
                        obj.token=[];
                        obj.types =[];
                        obj.uuid=uuid;
                    }else{
                        let type = JSON.parse(sessionStorage.getItem('subscriptType'))['types'];
                        let propsData = type.filter( item => !this.checkList.some(ele => ele === item));
                        for (let i = 0; i < this.data.length; i++) {
                            const children = this.data[i].children;
                            for (let j = 0; j < children.length; j++) {
                                arr1.push(children[j].token);
                                obj.token = arr1;
                            }
                        }
                        obj.types =propsData;
                        obj.uuid=uuid;
                    }
                }
                sessionStorage.setItem('subscriptType',JSON.stringify(obj));
                this.$store.state.e1.keepaliveTimer(obj);
            }
        },
        handleCheckAllChange(val) {
            var data = ["H5S_EVENT_ONLINE","H5S_EVENT_OFFLINE","H5S_EVENT_MOTION"]
            this.checkList = val ? data : [];
            this.isIndeterminate = false;
        },
        gEventval(){
            setInterval(function(){
                this.total=this.tableData1.length;
            }.bind(this),1000)
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
        
        changeType(value){
            let checkedCount = value.length;
            this.checkAll = checkedCount === this.subscribeType.length;
            this.isIndeterminate = checkedCount > 0 && checkedCount < this.subscribeType.length;
        },
        subevent(){
            // this.tableData1=[];
            // var e1=undefined;
            this.$store.state.gEvent=[];
            console.log(this.checkList);
            let obj = {};
            let arr1 =[];
            let type=[];
            console.log("node值",this.$refs.tree.getCheckedNodes());
            var nodes=this.$refs.tree.getCheckedNodes();
            if (nodes.length>0) {
                for(var l=0 ;l<nodes.length; l++){
                    if (!nodes[l].children) {
                        if(nodes[l].token!=undefined){
                            arr1.push(nodes[l].token);
                            obj.token = arr1;
                            type = JSON.parse(JSON.stringify(this.checkList))
                            // console.log(type);
                            if (type.length>0) {
                                obj.types =this.checkList;
                            }else{
                            obj.types=["H5S_EVENT_ONLINE","H5S_EVENT_OFFLINE","H5S_EVENT_MOTION"]
                            }
                        }
                    }
                }
            }else{
                for (let i = 0; i < this.data.length; i++) {
                    const children = this.data[i].children;
                    for (let j = 0; j < children.length; j++) {
                        arr1.push(children[j].token);
                        obj.token = arr1;
                    }
                }
                obj.types =this.checkList;
            }
            if (obj.types.length>0||obj.token) {
                if (JSON.parse(sessionStorage.getItem('subscriptType'))) {
                    var uuid = JSON.parse(sessionStorage.getItem('subscriptType'));
                    if (uuid.types) {
                        obj.uuid=uuid.uuid;
                    }else{
                        obj.uuid=uuid.type.uuid;
                    }
                }else{
                    obj.uuid= this.Eventuuid;
                }
                sessionStorage.setItem('subscriptType',JSON.stringify(obj))
                console.log(obj);
                this.tableData3=[];
                if (!this.$store.state.e1) {
                     obj={type:obj}
                    var conf1 = {
                        protocol: window.location.protocol, //http: or https:
                        host:this.$store.state.WSROOT, //localhost:8080
                        rootpath:'/', // '/'
                        callback: this.EventCB, 
                        userdata: null, // user data
                        session: this.$store.state.token, //session got from login
                        Subscription: obj,
                    };
                    this.$store.state.e1 = new IotEventExt(conf1);
                    this.$store.state.e1.connect();
                }else{
                    this.$store.state.e1.keepaliveTimer(obj);
                }
            }
        },
        EventCB(event, userdata){
            var msgevent = JSON.parse(event);
            let timeitem={
                Token: msgevent.strDevToken,
                Type: msgevent.type,
                UUID:msgevent.strUUID,
                Time:msgevent.strTime,                      
                Detail:event
            };
            this.$store.state.gEvent.unshift(timeitem);
        },
        //树形节点点击
        handleNodeClick(data, checked, indeterminate) {
        //   console.log(data.token);
        this.dataToken=[];
        this.dataToken.push(data.token);
        this.tableData = [];
        let root = this.$store.state.IPPORT;
        let url = root + "/api/v2/GetEventList?token=" + data.token + "&session=" + this.$store.state.token;
        this.$http.get(url).then((result) => {
            // console.log(result);
            if (result.status == 200) {
            for (let i = 0; i < result.data.length; i++) {
                const data = result.data[i];
                // this.time(data, result.data[i].token, result.data[i].strtime);
                this.tableData.push(data);
                // console.log(this.tableData);
            }
            }
        });
        },
        //测试机仓
        loadtest() {
        let root = this.$store.state.IPPORT;
        if (this.$store.state.root == "Operator") {
            return false;
        }
        let url = root + "/api/v1//GetSrcCamera?session=" + this.$store.state.token;
        // return falsel;
        this.$http.get(url).then((result) => {
            // console.log(result);
            if (result.status == 200) {
            let data = result.data;
            let srcGroup = { children: [] };
            srcGroup.label = this.$t("message.live.camera");
            srcGroup.iconclass = "iconfont  icon-kaiqishexiangtou1";
            for (let i = 0; i < data.src.length; i++) {
                let item = data.src[i];
                if (item["nOriginalType"] == "H5_CH_GB") {
                continue;
                } else {
                // 主副流
                let node = [{
                    token: item["strToken"],
                    streamprofile: "main",
                    label: this.$t("message.live.mainstream"),
                    name: item["strName"] + "--" + this.$t("message.live.mainstream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                    {
                    token: item["strToken"],
                    streamprofile: "sub",
                    label: this.$t("message.live.substream"),
                    name: item["strName"] + "--" + this.$t("message.live.substream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                ];
                let newItem = {
                    token: item["strToken"],
                    label: item["strName"],
                    iconclass: "iconfont  icon-kaiqishexiangtou1",
                    iconclass2: "iconfont  icon-shexiangtou",
                    name: item["strName"] + "--" + this.$t("message.live.mainstream"),
                    children: node,
                    disabled_me: false,
                };

                if (!item["bOnline"])
                    newItem["iconclass"] = "iconfont icon-kaiqishexiangtou";

                if (item["nType"] == "H5_CLOUD")
                    newItem["iconclass"] = "mdi mdi-cloud-upload fa-fw";

                if (item["bRec"] == true)
                    newItem["iconclass2"] = "iconfont icon-radioboxfill none";

                srcGroup.children.push(newItem);
                }
            }
            this.data.push(srcGroup);
            }
        });
        },
        //写作业
        loadDevice() {
        var root = this.$store.state.IPPORT;
        if (this.$store.state.root == "Operator") {
            return false;
        }
        var url = root + "/api/v1/GetDevice?session=" + this.$store.state.token;

        //重组
        this.$http.get(url).then((result) => {
            if (result.status == 200) {
            // var srcData = [];
            var data = result.data.dev;
            // console.log(data)
            if (Array.isArray(data)) {
                data.sort((a, b) => {
                if (a.strName === b.strName) return 0;
                return a.strName > b.strName ? 1 : -1;
                });
                for (var i = 0; i < data.length; i++) {
                var item = data[i];
                this.data.push({
                    token: item.strToken,
                    label: item.strName,
                    iconclass: "iconfont icon-kaiqishexiangtou1",
                    children: [],
                });
                this.loadSrc(item.strToken);
                }
                // console.log(this.data)
            }
            }
        });
        },
        loadSrc(strToken) {
        var root = this.$store.state.IPPORT;
        let _this = this;

        var url = root + "/api/v1/GetDeviceSrc?token=" + strToken + "&session=" + this.$store.state.token;

        this.$http.get(url).then((result) => {
            // console.log(result.data)
            if (result.status == 200) {
                var data = result.data;
                var srcGroup = [];
                for (var i = 0; i < data.src.length; i++) {
                var item = data.src[i];
                // 主副流
                var node = [
                    {
                    token: item["strToken"],
                    streamprofile: "main",
                    label: this.$t("message.live.mainstream"),
                    name:
                        item["strName"] + "--" + this.$t("message.live.mainstream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                    {
                    token: item["strToken"],
                    streamprofile: "sub",
                    label: this.$t("message.live.substream"),
                    name:
                        item["strName"] + "--" + this.$t("message.live.substream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                ];
                for (var l = 0; l < node.length; l++) {
                    if (item["bDisable"] == true) {
                    node[l].disabled_me = true;
                    }
                }

                var newItem = {
                    token: item["strToken"],
                    label: item["strName"],
                    iconclass: "iconfont  icon-kaiqishexiangtou1",
                    iconclass1: "",
                    name:
                    item["strName"] + "--" + this.$t("message.live.mainstream"),
                    children: node,
                    disabled_me: false,
                };

                if (!item["bOnline"])
                    newItem["iconclass"] = "iconfont icon-kaiqishexiangtou";

                if (item["nType"] == "H5_CLOUD")
                    newItem["iconclass"] = "mdi mdi-cloud-upload fa-fw";

                if (item["bRec"] == true)
                    newItem["iconclass2"] = "iconfont icon-radioboxfill none";

                if (item["bDisable"] == true) {
                    newItem["disabled_me"] = true;
                    newItem["iconclass1"] = "camera";
                }
                if (
                    item["bDisable"] == true &&
                    this.$store.state.devicemarktoggle == "none"
                ) {
                    continue;
                }
                srcGroup.push(newItem);
                }
                var srcData = this.data.find((item) => item.token === strToken);
                // console.log(srcData)
                if (srcData) {
                srcData.children = srcGroup;
                }
            }
            })
            .catch((error) => {
            console.log("GetSrc failed", error);
            });
        },
        //数字仓机
        NumberDevice() {
        var root = this.$store.state.IPPORT;
        if (this.$store.state.root == "Operator") {
            return false;
        }
        //url
        var url = root + "/api/v1/GetGbDevice?session=" + this.$store.state.token;

        //重组
        this.$http.get(url).then((result) => {
            if (result.status == 200) {
            var data = result.data.dev;
            // console.log(data)
            if (Array.isArray(data)) {
                data.sort((a, b) => {
                if (a.strName === b.strName) return 0;

                return a.strName > b.strName ? 1 : -1;
                });
                for (var i = 0; i < data.length; i++) {
                var item = data[i];

                this.data.push({
                    token: item.strToken,
                    label: item.strName,
                    iconclass: "iconfont icon-kaiqishexiangtou1",
                    children: [],
                });
                this.NumberSrc(item.strToken);
                }
                // console.log(this.data)
            }
            }
        });
        },
        NumberSrc(strToken) {
        var root = this.$store.state.IPPORT;
        let _this = this;
        var url =
            root +
            "/api/v1/GetGbDeviceSrc?token=" +
            strToken +
            "&session=" +
            this.$store.state.token;

        this.$http
            .get(url)
            .then((result) => {
            if (result.status == 200) {
                var data = result.data;
                var srcGroup = [];
                for (var i = 0; i < data.src.length; i++) {
                var item = data.src[i];
                // 主副流
                var node = [
                    {
                    token: item["strToken"],
                    streamprofile: "main",
                    label: this.$t("message.live.mainstream"),
                    name:
                        item["strName"] + "--" + this.$t("message.live.mainstream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                    {
                    token: item["strToken"],
                    streamprofile: "sub",
                    label: this.$t("message.live.substream"),
                    name:
                        item["strName"] + "--" + this.$t("message.live.substream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                ];
                var newItem = {
                    token: item["strToken"],
                    label: item["strName"],
                    iconclass: "iconfont  icon-kaiqishexiangtou1",
                    name:
                    item["strName"] + "--" + this.$t("message.live.mainstream"),
                    children: node,
                    disabled_me: false,
                };

                if (!item["bOnline"])
                    newItem["iconclass"] = "iconfont icon-kaiqishexiangtou";

                if (item["nType"] == "H5_CLOUD")
                    newItem["iconclass"] = "mdi mdi-cloud-upload fa-fw";

                if (item["bRec"] == true)
                    newItem["iconclass2"] = "iconfont icon-radioboxfill none";

                srcGroup.push(newItem);
                }
                var srcData = this.data.find((item) => item.token === strToken);
                // console.log(srcData)
                if (srcData) {
                srcData.children = srcGroup;
                }
            }
            })
            .catch((error) => {
            console.log("GetSrc failed", error);
            });
        },

        Regional() {
        var root = this.$store.state.IPPORT;
        var url = root + "/api/v1/GetRegion?session=" + this.$store.state.token;
        this.$http.get(url).then((result) => {
            console.log(result);
            var oldarr = result.data.root;
            var oldarr1 = result.data.src;
            // console.log(oldarr,oldarr1)
            var dataroot = this.getchild(oldarr, oldarr1);
            // this.data1.push(dataroot);
            console.log(dataroot);
            this.datapush(dataroot);
            console.log(this.data1);
        });
        },
        datapush(data) {
        this.a(data);
        },
        a(data) {
        console.log(data.cam);
        if (data.node.length != 0) {
            for (let i = 0; i < data.node.length; i++) {
            data.cam.push(data.node[i]);
            }
        }
        delete data.node;
        for (let i = 0; i < data.cam.length; i++) {
            // console.log(data);
            this.b(data.cam[i]);
            this.data1 = [];
            this.data1.push(data);
        }
        },
        b(data) {
        if (data.name) {
            return;
        }
        if (data.node.length != 0) {
            for (let i = 0; i < data.node.length; i++) {
            data.cam.push(data.node[i]);
            }
        }
        delete data.node;
        for (let i = 0; i < data.cam.length; i++) {
            console.log(data.cam[i]);
            if (data.cam[i].cam) {
            this.c(data.cam[i]);
            }
        }
        },
        c(data) {
        if (data.name) {
            return;
        }
        if (data.node.length != 0) {
            for (let i = 0; i < data.node.length; i++) {
            data.cam.push(data.node[i]);
            }
        }
        delete data.node;
        for (let i = 0; i < data.cam.length; i++) {
            console.log(data.cam[i]);
            if (data.cam[i].cam) {
            this.d(data.cam[i]);
            }
        }
        },
        d(data) {
        if (data.name) {
            return;
        }
        if (data.node.length != 0) {
            for (let i = 0; i < data.node.length; i++) {
            data.cam.push(data.node[i]);
            }
        }
        delete data.node;
        for (let i = 0; i < data.cam.length; i++) {
            console.log(data.cam[i]);
            if (data.cam[i].cam) {
            this.e(data.cam[i]);
            }
        }
        },
        e(data) {
        if (data.name) {
            return;
        }
        if (data.node.length != 0) {
            for (let i = 0; i < data.node.length; i++) {
            data.cam.push(data.node[i]);
            }
        }
        delete data.node;
        },
        getchild(arr, arr1) {
        for (var i in arr.cam) {
            if (!arr.cam[i].strName) {
            for (var j in arr1) {
                if (arr.cam[i].strToken == arr1[j].strToken) {
                var node1 = [
                    {
                    strToken: arr1[j].strToken,
                    streamprofile: "main",
                    strName: this.$t("message.live.mainstream"),
                    name:
                        arr1[j].strName + "--" + this.$t("message.live.mainstream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                    {
                    strToken: arr1[j].strToken,
                    streamprofile: "sub",
                    strName: this.$t("message.live.substream"),
                    name:
                        arr1[j].strName + "--" + this.$t("message.live.substream"),
                    iconclass: "mdi mdi-playlist-play fa-fw",
                    disabled_me: false,
                    },
                ];
                arr.cam[i].node = node1;
                arr.cam[i].strName = arr1[j].strName;
                arr.cam[i].name =
                    arr1[j].strName + "--" + this.$t("message.live.mainstream");
                arr.cam[i].bOnline = arr1[j].bOnline;
                arr.cam[i].iconclass = "iconfont  icon-kaiqishexiangtou1";
                arr.cam[i].disabled_me = false;
                if (!arr1[j].bOnline)
                    arr.cam[i].iconclass = "iconfont icon-kaiqishexiangtou";

                if (arr1[j].nConnectType == "H5_CLOUD")
                    arr.cam[i].iconclass = "iconfont  icon-kaiqishexiangtou1";

                if (arr1[j].bRec == true)
                    arr.cam[i].iconclass2 = "iconfont icon-radioboxfill none";

                if (arr1[j].bDisable == true) {
                    arr.cam[i].node[0].disabled_me = true;
                    arr.cam[i].node[1].disabled_me = true;
                    arr.cam[i].disabled_me = true;
                    arr.cam[i].iconclass1 = "camera";
                }
                }
            }
            }
        }

        var camarr = [];
        var camarr1 = [];
        for (var i in arr.cam) {
            camarr.push(arr.cam[i].strToken);
        }
        for (var i in arr1) {
            camarr1.push(arr1[i].strToken);
        }
        let diff = camarr.filter((item) => !new Set(camarr1).has(item));
        // console.log(diff)
        for (var i in arr.cam) {
            for (var j in diff) {
            if (arr.cam[i].strToken == diff[j]) {
                arr.cam[i].strName = "设备暂时不可用";
                arr.cam[i].iconclass = "iconfont  icon-kaiqishexiangtou";
            }
            }
        }

        if (arr.node && arr.node.length > 0) {
            for (var i = 0; i < arr.node.length; i++) {
            arr.node[i] = this.getchild(arr.node[i], arr1);
            }
        }
        return arr;
        },
    },
    // watch:{
    //     tableData1(){
    //         console.log(this.eventType.length,this.eventToken.length);
    //         if (this.eventType.length>0||this.eventToken.length>0) {
    //             // if (this.eventToken) {
    //                 for (var i = 0; i < this.tableData1.length; i++) {
    //                     var Token = this.tableData1[i].Token;
    //                     console.log(Token);
    //                     for (var k = 0; k < this.eventToken.length; k++) {
    //                         var token = this.eventToken[k];
    //                         console.log(token);
    //                         if (Token==token) {
    //                             console.log(this.tableData1[i]);
    //                             // let arr = new Set(...this.tableData1[i])
    //                             this.tableData3.unshift(this.tableData1[i]);
    //                         }
    //                     }
    //                 }
                // }
                // }else if (this.eventType&&this.eventToken.length<0) {
                //      for (var i = 0; i < this.tableData1.length; i++) {
                //         var Type = this.tableData1[i].Type;
                //         console.log(Type);
                //         for (var k = 0; k < this.eventType.length; k++) {
                //             var type = this.eventType[k];
                //             console.log(type);
                //             if (Type==type) {
                //                 console.log(this.tableData1[i]);
                //                 this.tableData3.unshift(this.tableData1[i]);
                //             }
                //         }
                //     }
                // }else if (this.eventToken&&this.eventType) {
                //     for (var i = 0; i < this.tableData1.length; i++) {
                //         var Token = this.tableData1[i].Token;
                //         var Type = this.tableData1[i].Type;
                //         console.log(Token);
                //         for (var k = 0; k < this.eventToken.length; k++) {
                //             var token = this.eventToken[k];
                //             console.log(token);
                //             for (var k = 0; k < this.eventType.length; k++) {
                //                 var type = this.eventType[k];
                //                 console.log(type);
                //                 if (Token==token) {
                //                 console.log(this.tableData1[i]);
                //                 this.tableData3.unshift(this.tableData1[i]);
                //             }
                //             }
                //         }
                //     }
                // }
            //    this.tableData1.map((item)=>{
            //        console.log(item);
            //     //    this.eventToken.map((token)=>{
            //     //        console.log(item.token,token.strDevToken);
            //     //        if (item.token==token.strDevToken) {
            //     //            this.tableData3.push(item);
            //     //            console.log(this.tableData3);
            //     //        }
            //     //    })
            //    })
    //         }else{
    //             this.tableData3=this.realEvent.gEvent;
    //         }
    //     }
    // }
  };
</script>
<style scoped lang="scss">
    #page-wrapper{
        display: flex;
        // padding-top: 10px;
        justify-content: space-between;
        -webkit-box-pack: justify;
        .event_left{
            width: 16%;
            min-width: 290px;
            // height: 86vh;
            margin: 0 5px;
            .subBtn{
                background:#00479D;
                border-radius:2px;
                padding: 8px 16px;
                font-size:12px;
                font-family:PingFang SC;
                font-weight:600;
                color:rgba(255,255,255,1);
                margin-right: 20px;
                border-color: none;
                border: none;
                width: 100px;
                height: 30px;
            }
        }
        .event_right{
            width: 84%;
            .eventTable /deep/ .has-gutter tr th {
                text-align: center;
            }
            .eventTable /deep/ tbody tr td {
                text-align: center;
            }
        }
    }
    
</style>