<template>
  <div class="event"
    v-loading="loading"
    element-loading-text="拼命加载中"
    element-loading-spinner="el-icon-loading"
    element-loading-background="rgba(0, 0, 0, 0.8)">
    <!-- 左侧数据栏 -->
    <div class="event_left">
      <el-input class="event_left_input" placeholder="输入关键字进行过滤" v-model="filterText"></el-input>
      <!-- 设备树 -->
      <el-collapse v-model="activeNames">
        <el-collapse-item name="1" id="headswitch">
          <template slot="title">
            <div style="display: flex; justify-content: space-between; width: 85%; align-items: center;">
              <div>设备</div>
            </div>
          </template>
          <el-tree :data="data" show-checkbox node-key="id" :filter-node-method="filterNode" ref="tree" @node-click="handleNodeClick" :props="defaultProps" >
            <span slot-scope="{ data }" style="width: 100%">
              <div style=" width: 100%; display: flex; justify-content: space-between;">
                <!-- draggable="true" @dragstart="dragStart($event,data.token,data.label,data.streamprofile, data.name,data.disabled_me,node)" -->
                <span class="size_color" draggable="true" @dragstart="dragStart($event,data.token,data.label,data.streamprofile,data.name,data.disabled_me)">
                  <span style="font-size: 12px" :class="data.iconclass" :id="'icon' + data. token"></span>
                  <span :class="data.iconclass1" style="padding-left: 4px">{{data.label}}</span>
                </span>
              </div>
            </span>
          </el-tree>
        </el-collapse-item>
        <!-- <el-collapse-item name="2" id="viewclis">
          <template  slot="title"></template>
        </el-collapse-item> -->
      </el-collapse>
      <!-- 日期，最近几条 -->
      <div class="queryCriteria" style="padding: 0px 10px;">
        <el-form :label-position="labelPosition" label-width="80px" :model="formLabelAlign">
          <el-form-item label="时间">
            <el-date-picker
              prefix-icon="iconfont icon-rili"
              class="fixed_input"
              v-model="formLabelAlign.eventTime"
              type="datetimerange"
              size="mini"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              :default-time="['00:00:00', '23:59:59']"
              unlink-panels
              value-format="yyyy-MM-ddTHH:mm:ss"
            >
            </el-date-picker>
          </el-form-item>
          <div class="snap_you_topfast" style="padding:10px 0 20px 0;">
              <span>最近</span>
              <el-select v-model="value" placeholder="请选择" @change="datechange"  class="EventDate">
                  <el-option
                  v-for="item in selectdate"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value">
                  </el-option>
              </el-select>
          </div>
          <el-form-item>
              <div style="display:flex;justify-content:space-between;align-items:center;">
                   <el-input class="limit" v-model="formLabelAlign.limit" style="width: 70px ;" size="small" ></el-input>
                    <el-button class="queryBtn"  @click="eventSearch">查询</el-button>
              </div>
          </el-form-item>
        </el-form>
      </div>
    </div>
    <!-- 右边数据 -->
    <div id="page-wrapper" class="event_right">
      <!-- 头部 -->
      <el-table class="eventTable" stripe :data="this.tableData.slice((currentPage - 1) * pageSize, currentPage * pageSize)" style="width: 100%; min-height:82vh; overflow-y: auto">
        <el-table-column label="编号" width="80" type="index" :index="indexMethod">
          <!-- <template slot-scope="scope">{{scope.$index+1}}</template> -->
        </el-table-column>
        <el-table-column prop="strName" label="名称"></el-table-column>
        <el-table-column prop="token" :label="G_label.Tokenevent"></el-table-column>
        <el-table-column prop="type" :label="G_label.Type">
          <template slot-scope="scope">
            <span v-if="scope.row.blur&&!scope.row.black">图像模糊</span>
            <span v-else-if="!scope.row.blur&&scope.row.black">图像变黑</span>
            <span v-else-if="scope.row.blur&&scope.row.black">图像模糊变黑</span>
            <span v-else-if="scope.row.state">图像正常</span>
            <span v-else-if="!scope.row.state">离线状态</span>
          </template>
        </el-table-column>
        <el-table-column prop="strTime" label="时间"></el-table-column>
        <el-table-column prop="state" label="在线/离线">
          <template slot-scope="scope">
            <el-switch
              v-model="scope.row.state"
              disabled
              active-color="#13ce66"
              inactive-color="#ff4949"
            >
            </el-switch>
          </template>
        </el-table-column>
      </el-table>
      <!-- 分页 -->
      <el-pagination
        style="text-align: center"
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
export default {
  data() {
    return {
      loading:false,
      G_label: {
        Index: this.$t("message.table.Index"),
        Type: this.$t("message.table.Type"),
        Tokenevent: this.$t("message.table.Tokenevent"),
        UUID: this.$t("message.table.UUID"),
        Time: this.$t("message.table.Time"),
        Detaill: this.$t("message.table.Detaill"),
      },
      //分页
      currentPage: 1, // 当前页码
      total: 0, // 总条数
      pageSize: 10, //一页数量
      // tableData1: this.EVENT.gEvent,
      tableData: [],
      dataToken:[],
      uuidArr:[],
      labelPosition: "top",
      formLabelAlign: {
        eventTime: [ new Date(new Date().getTime() - 1 * 60 * 60 * 1000),new Date()],
        limit: "32",
      },
      activeNames: ["1", "2"], //左边
      filterText: "", //搜索框
      data: [],
      data1: [],
      defaultProps: {
        children: "children",
        label: "label",
        token: "token",
        iconclass: "iconclass",
      },
      selectdate: [{
        value: 'hours',
        label: '一小时'
      }, {
        value:'day',
        label: '一天'
      }, {
        value: 'week',
        label: '一周'
      }, {
        value: 'month',
        label: '一个月'
      }],
      value:'一小时'
    };
  },
  created(){
    AllMethods().then(res=>{
      this.data = res.listdatag;
    }).catch(err=>{
      console.log(err);
    })
  },
  mounted() {
    // this.loadEvent();
    // this.loadtest();
    // this.$nextTick(() => {
    //   this.loadDevice();
    //   this.NumberDevice();
    // });
    console.log(this.EVENT.gEvent);
  },
  methods: {
    // 整月日期
    datechange(){
      this.formLabelAlign.eventTime =[];
      if (this.value=='hours') {
        var data=[
          new Date(new Date().getTime() - 3600 * 1000 * 1),
          new Date()
        ];
        this.formLabelAlign.eventTime=data;
      }else if (this.value=='day'){
        var data=[
          new Date(new Date().getTime() - 3600 * 1000 * 24),
          new Date()
        ];
        this.formLabelAlign.eventTime=data;
      }else if (this.value=='week'){
        var data=[
          new Date(new Date().getTime() - 3600 * 1000 * 24 * 7),
          new Date()
        ];
        this.formLabelAlign.eventTime=data;
      }else if (this.value=='month'){
        var data=[
          new Date(new Date().getTime() - 3600 * 1000 * 24 * 30),
          new Date()
        ];
        this.formLabelAlign.eventTime=data;
      }
    },
    getTime(data){
      let myDate;
      if (data) {
        myDate = new Date(data);
      }else{
        myDate = new Date();
      }
      let month = myDate.getMonth() + 1 < 10 ? `0${myDate.getMonth() + 1}` : myDate.getMonth() + 1;
      let day = myDate.getDate() < 10 ? `0${myDate.getDate()}` : myDate.getDate();
      let hours = myDate.getHours() < 10 ? `0${myDate.getHours()}` : myDate.getHours();
      let min = myDate.getMinutes() < 10 ? `0${myDate.getMinutes()}` : myDate.getMinutes();
      let sec = myDate.getSeconds() < 10 ? `0${myDate.getSeconds()}` : myDate.getSeconds();
      return `${myDate.getFullYear()}-${month}-${day}T${hours}:${min}:${sec}%2B08:00`;
    },
    //查询事件
    eventSearch() {
      let _this = this;
      this.loading = true;
      let form = _this.formLabelAlign;
    //   console.log(form);
      if (form.eventTime) {
        var starttime = form.eventTime[0];
        var endtime = form.eventTime[1];
      }
      starttime=this.getTime(starttime);
      endtime= this.getTime(endtime);
      this.tableData = [];
      let root = this.$store.state.IPPORT;
      let url ='' ;
      if(this.dataToken.length>0){
         url=root + "/api/v2/GetVqdResultList?token=" +this.dataToken[0]+(starttime==undefined?'':("&starttime=" +encodeURIComponent(starttime) + "&endtime=" + encodeURIComponent(endtime))) + "&limit=" + form.limit+'&session='+this.$store.state.token;
      }else{
          if(!(form.limit=="")&& !(starttime=="")&& !(endtime=="")){
          url=root + "/api/v2/GetVqdResultList?session="+this.$store.state.token+(starttime==undefined?'':("&starttime=" + encodeURIComponent(starttime) + "&endtime=" + encodeURIComponent(endtime))) + "&limit=" + form.limit;
        }
      }
      console.log(url);
      this.$http.get(url).then((result) => {
        console.log(result);
        if (result.status == 200) {
          this.loading = false;
          for (let i = 0; i < result.data.length; i++) {
            const data = result.data[i];
            this.tableData.push(data);
          }
          this.total = this.tableData.length;
        }
      });
    },
    indexMethod(index) {
      return (this.currentPage - 1) * this.pageSize + index + 1;
    },
    // loadEvent
    loadEvent() {
      this.tableData = [];
      let root = this.$store.state.IPPORT;
      let url = root + "/api/v2/GetVqdResultList?limit=1000&session="+this.$store.state.token;
      this.$http.get(url).then((result) => {
        // console.log(result);
        if (result.status == 200) {
          for (let i = 0; i < result.data.length; i++) {
            const data = result.data[i];
            this.tableData.push(data);
          }
        }
      });
    },
    //树形节点点击
    handleNodeClick(data, checked, indeterminate) {
      if (data.token&&data.name) {
        this.dataToken=[];
        this.dataToken.push(data.token);
        this.tableData = [];
        let root = this.$store.state.IPPORT;
        let url = root + "/api/v2/GetVqdResultList?token=" + data.token + "&session=" + this.$store.state.token;
        this.$http.get(url).then((result) => {
          // console.log(result);
          if (result.status == 200) {
            for (let i = 0; i < result.data.length; i++) {
              const data = result.data[i];
              this.tableData.push(data);
            }
            this.total = this.tableData.length;
          }
        });
      }
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
              let newItem = {
                token: item["strToken"],
                label: item["strName"],
                iconclass: "iconfont  icon-kaiqishexiangtou1",
                iconclass2: "iconfont  icon-shexiangtou",
                name: item["strName"] + "--" + this.$t("message.live.mainstream"),
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
              var newItem = {
                token: item["strToken"],
                label: item["strName"],
                iconclass: "iconfont  icon-kaiqishexiangtou1",
                iconclass1: "",
                name:
                  item["strName"] + "--" + this.$t("message.live.mainstream"),
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
              var newItem = {
                token: item["strToken"],
                label: item["strName"],
                iconclass: "iconfont  icon-kaiqishexiangtou1",
                name:
                  item["strName"] + "--" + this.$t("message.live.mainstream"),
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
    //模糊查询
    filterNode(value, data, node) {
      // 如果什么都没填就直接返回
      if (!value) return true;
      // 如果传入的value和data中的label相同说明是匹配到了
      if (data.label.indexOf(value) !== -1) {
        return true;
      }
      // 否则要去判断它是不是选中节点的子节点
      return this.checkBelongToChooseNode(value, data, node);
    },
    // 判断传入的节点是不是选中节点的子节点
    checkBelongToChooseNode(value, data, node) {
      const level = node.level;
      // 如果传入的节点本身就是一级节点就不用校验了
      if (level === 1) {
        return false;
      }
      // 先取当前节点的父节点
      let parentData = node.parent;
      // 遍历当前节点的父节点
      let index = 0;
      while (index < level - 1) {
        // 如果匹配到直接返回
        if (parentData.data.label.indexOf(value) !== -1) {
          return true;
        }
        // 否则的话再往上一层做匹配
        parentData = parentData.parent;
        index++;
      }
      // 没匹配到返回false
      return false;
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
  },
  //模糊查询
  watch: {
    filterText(val) {
      console.log("filter", val);
      this.$refs.tree.filter(val);
    },
    // tableData1(val){
    //   console.log(val);
    //   this.uuidArr=[];
    //   let root = this.$store.state.IPPORT;
    //   let addChUrl = '';
    //   const uuid = val[val.length-1].UUID;
    //   console.log(uuid);
    //   addChUrl = root + "/api/v2/GetEventList?uuid=" + uuid;
    //   this.uuidArr.push(addChUrl);
    //   console.log(this.uuidArr);
      
    //   for (let k = 0; k < this.uuidArr.length; k++) {
    //     const uuid = this.uuidArr[k];
    //      this.$http.get(uuid).then((result) => {
    //       if (result.status == 200) {
    //         for (let i = 0; i < result.data.length; i++) {
    //           const itme = result.data;
    //           let tabledata = {
    //             desc : itme[i].desc,
    //             handler : itme[i].handler,
    //             id : itme[i].id,
    //             name : itme[i].name,
    //             priority : itme[i].priority,
    //             state : itme[i].state,
    //             strtime : itme[i].strtime,
    //             time : itme[i].time,
    //             token : itme[i].token,
    //             type : itme[i].type,
    //             uuid : itme[i].uuid,
    //           };
    //           console.log(this.tableData);
    //           this.tableData.unshift(tabledata);
    //         }
    //       }
    //     });
    //   }
    // }
  },
};
</script>
<style scoped lang="scss">
.event{
    display: flex;
    // padding-top: 10px;
    justify-content: space-between;
    -webkit-box-pack: justify;
    .event_left{
        width: 16%;
        min-width: 290px;
        height: 86vh;
        margin: 0 5px;
        .event_left_input {
            margin: 10px 0;
        }
        .EventDate{
          margin: 0 0 0 10px;
          /deep/ .el-input__inner{
            height:30px;
            }
          }
        .fixed_input {
            width: 100%;
            /deep/ input {
                background-color: #2d2d2d;
            }
            /deep/ span {
                color: white;
            }
        }
        .limit /deep/ .el-input__inner {
           height: 28px ;
        }
        // .queryCriteria{
        //     background-color: #292929;
        //     color: #ffffff;
        // }
        // .queryCriteria form /deep/ .el-form-item__label{
        //     padding-bottom: 0px !important;
        //     color: white;
        //     font-size: 15px;
        // }
        .queryCriteria /deep/ .el-date-table td.in-range div, .el-date-table td.in-range div:hover, .el-date-table.is-week-mode .el-date-table__row.current div, .el-date-table.is-week-mode .el-date-table__row:hover div {
        background-color: #F2F6FC;
        }
        
        .queryBtn{
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
        .el-collapse{
            .el-collapse-item{
                height: 40vh;
                .el-collapse-item__wrap{
                    .el-collapse-item__content{
                        height: 35vh;
                        padding: 0 10px;
                        overflow: auto;
                    }
                }
            }
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
            /deep/.el-table {
				.el-table__header-wrapper{
					.el-table__header{
						width: 100% !important;
					}
				}
				.el-table__body-wrapper{
					.el-table__body{
						width: 100% !important;
					}
				}
			}
    }


}
body /deep/ .el-date-table td.in-range div, .el-date-table td.in-range div:hover, .el-date-table.is-week-mode .el-date-table__row.current div, .el-date-table.is-week-mode .el-date-table__row:hover div {
    background-color: none !important; 
}
</style>