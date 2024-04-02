<template>
  <div class="vqd">
    <div class="vqd_left">
      <el-input
        class="vqd_left_input"
        placeholder="Enter keywords to filter"
        v-model="filterText">
      </el-input>
      <el-collapse v-model="activeNames">
        <el-collapse-item name="1" id="headswitch">
          <template slot="title">
            <div
              style="
                display: flex;
                justify-content: space-between;
                width: 85%;
                align-items: center;">
              <div>{{ $t("message.live.device") }}</div>
              <div class="liveview_colltitle">
                <div class="liveview_titleicon1" @click.stop="Refresh('device')"></div>
              </div>
            </div>
          </template>
          <el-tree
            :data="data"
            node-key="id"
            ref="tree"
            :filter-node-method="filterNode"
            @node-click="handleNodeClick"
            :props="defaultProps"
          >
            <span slot-scope="{ data }">
              <i :class="data.iconclass" style="font-size:10px;"></i>
              <span :class="data.iconclass1" style="padding-left: 4px;">{{data.label||data.strName}}</span>
              <span v-if="data.length" style="padding-left: 4px;">{{data.online}}/{{data.length}}</span>
              <el-popover placement="bottom" trigger="manual" v-if="data.firstLevel" class="Status" popper-class="popperOptions" style="flex: 1; text-align: right;height: 36px;line-height: 36px;" :ref="refNamePopover+data.statusID">
                <div class="StatusPrompt" >
                  <div style="width:100%;display: flex;justify-content: space-between;">
                    <span style="font-size:12px;">
                      <span style="padding-left:5px">
                        <i class="iconfont icon-shexiangji" style="font-size:12px;"></i>
                        Online:{{data.online}}
                      </span>
                      <span class="el-tree-camera" style="padding-left:5px">
                        <i class="iconfont icon-shexiangji el-tree-camera" style="font-size:12px;"></i>
                        Offline:{{data.offline}}
                      </span>
                      <span class="el-tree-camera" style="padding-left:5px">
                        <i class="iconfont icon-jinyong1 el-tree-camera " style="font-size:12px;"></i>
                        Disabled:{{data.disable}}
                      </span>
                      <span class="el-tree-camera" style="padding-left:5px">
                        <i class="iconfont icon-kaiqishexiangtou1 el-tree-camera" style="font-size:12px;"></i>
                        Idle:{{data.idle}}
                      </span>
                    </span>
                  </div>
                </div>
                <el-button  slot="reference" type="text"  class="iconfont icon-xianshi2 statusBtn" style="margin-left: 20px;font-size:12px" @click="StatusPrompt(data.statusID,$event)"></el-button>
              </el-popover>
            </span>
          </el-tree>
        </el-collapse-item>
      </el-collapse>
    </div>
    <div class="vqd_right">
      <div class="liveview_right" id="videoPanel">
        <div class="liveview_right_but">
          <div class="liveview_right_but1">
              <CButton
              class="form_butt iconfont icon-duihao"
              @click="Allon(true)"
              type="submit"
              >All on</CButton
            >
            <CButton
              class="form_butt iconfont icon-jinyong"
              @click="Alloff(false)"
              type="submit" style="margin-left:10px"
              >Close all</CButton
            >
          </div>
          <div class="liveview_right_but2">
            <div class="concurrency">
              <span style="height:28px;line-height:28px;">Concurrency</span>&nbsp;
              <el-input size="mini" v-model="bingfa"></el-input>
              <el-button type="primary" size="mini" @click="concurrency">Sure</el-button>
            </div>
            <el-select class="elselect" size="small" v-model="value" @change='onchange'>
              <el-option
                v-for="item in options"
                :key="item.value"
                :label="item.label"
                :value="item.value">
              </el-option>
            </el-select>
          </div>
        </div>
        <el-table
          v-loading="loading"
          element-loading-text="Saving"
          element-loading-background="rgba(0, 0, 0, 0)"
          stripe
          style="width: 100%"
          :data="
            tableData1
              .filter(
                (data) =>
                  !search ||
                  data.name.toLowerCase().includes(search.toLowerCase()) ||
                  data.token.toLowerCase().includes(search.toLowerCase())
              )
              .slice((currentPage - 1) * pageSize, currentPage * pageSize)
          "
        >
          <el-table-column prop="token" :label="label.Token"></el-table-column>

          <el-table-column prop="name" :label="Name1"></el-table-column>
          <el-table-column :label="open_Close">
            <template slot-scope="scope">
              <el-switch
                v-model="scope.row.open_close"
                :disabled="scope.row.disabled"
                @change="open(scope.$index,scope.row)"
                active-color="#13ce66"
                inactive-color="#ff4949"
              >
              </el-switch>
            </template>
          </el-table-column>
          <el-table-column fixed="right">
            <template slot="header" slot-scope="scope">
              <el-input
                v-model="search"
                @change="handlechange(scope.$index, scope.row)"
                size="mini"
                placeholder="Enter keywords"
              />
            </template>
            <template slot-scope="scope">
            <div class="">
              <el-button
                @click="handledel(scope.$index, scope.row)"
                class="iconfont icon-ashbin delete-icon"
                type="text"
              ></el-button>
            </div>
          </template>
          </el-table-column>
        </el-table>
        <!-- Pagination -->
        <el-pagination
          style="text-align: center"
          layout=" prev, pager, next,total, jumper"
          @size-change="handleSizeChange"
          @current-change="handleCurrentChange"
          :current-page="currentPage"
          :total="total"
        >
        </el-pagination>
      </div>
    </div>
  </div>
</template>
<script>
import {listdatagload,listdatag,AllMethods} from '../../public/regional'
export default {
  data() {
    return {
      bingfa:5,
      value:'',//Inspection time
      values:'',
      array:{
        '1000':'1 Sec',
        '5000':'5 Sec',
        '10000':'10 Sec',
        '30000':'30 Sec',
        '60000':'1 Min',
        '180000':'3 Min',
        '300000':'5 Min',
      },
      options: [
        {
          value:'1000',
          label:'1 Sec'
        },{
          value:'5000',
          label:'5 Sec'
        },{
          value: '10000',
          label: '10 Sec'
        }, {
          value: '30000',
          label: '30 Sec'
        }, {
          value: '60000',
          label: '1 Min'
        }, {
          value: '180000',
          label: '3 Min'
        }, {
          value: '300000',
          label: '5 Min'
        }],
      search: "", //Search
      label: {
        Token: this.$t("message.table.Token"),
        GBID: this.$t("message.table.GBID"),
      },
      open_Close: this.$t("message.camera.open_Close"),
      Name1: this.$t("message.camera.name"),
      audio: this.$t("message.camera.audio"),
      activeNames: ["1"],
      data: [],
      defaultProps: {
        children: "children",
        label: "label",
        token: "token",
        iconclass: "iconclass",
      },
      threevideo: [],
      loading: false,
	  //Pagination
      currentPage: 1, // Current page number
      total: 0, // Total number
      pageSize: 10, // Number of pages
      tableData: [],
      vqddatalist:{},
      filterText: "", // Search bar
      refNamePopover: 'popover-', // popover Ref name prefix
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
    this.vqdlist();
  },
  computed: {
    // Fuzzy search based on computed properties
    tableData1() {
      const searchTableInfo = this.searchTableInfo;
      if (searchTableInfo) {
        return this.tableData.filter((data) => {
          console.log("success" + data);
          return Object.keys(data).some((key) => {
            console.log(data[key]);
            // This method is case sensitive! Therefore, the toLowerCase() method was required to convert all the queried content to lowercase.
            return String(data[key]).indexOf(searchTableInfo) > -1;
          });
        });
      }
      return this.tableData;
    },
  },
  methods: {
    concurrency(){
      if (this.bingfa>100) {
        this.bingfa = 100
      }
      let url = this.$store.state.IPPORT+ "/api/v2/UpdataConcurrency?concurrency="+parseInt(this.bingfa)+"&session=" + this.$store.state.token;
      console.log(url);
      this.$http.get(url).then((result) => {
        if (result.data.bStatus) {
          this.$message({
            message: 'Set successfully',
            type: "success",
          });
        }
        if (result.data.bStatus!=true) {
          this.value = this.values
          this.$message({
            message: this.$t("message.camera.Editor_failure"),
            type: "success",
          });
        }
      })
    },
    //Refresh
    Refresh(){
      this.data=[];
      AllMethods().then(res=>{
        this.data = res.listdatag;
      }).catch(err=>{
        console.log(err);
      })
    },
    // Online and offline status tips
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
    vqdlist(){
      this.vqddatalist={}
      let url = this.$store.state.IPPORT+ "/api/v2/GetVqdList?session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        console.log(result);
        this.value=this.array[result.data.data.intervaltime];
        this.bingfa=result.data.data.concurrency;
        this.values=this.array[result.data.data.intervaltime];
        if (result.data.data.cam.length!=0) {
          for (let i = 0; i < result.data.data.cam.length; i++) {
            this.vqddatalist[result.data.data.cam[i].token]=result.data.data.cam[i].enable;
          }
        }
        // console.log(this.vqddatalist);
      })
    },
    open(index,date){
      let token = date.token;
      let index_xlh = (this.currentPage - 1) * 10 + index;
      console.log(index_xlh,date);
      let enable = date.open_close;
      let url = this.$store.state.IPPORT+ "/api/v2/AddVqd?token="+token+"&enable="+enable+"&session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        this.loading = true;
        console.log(result);
        if (result.data.bStatus) {
          this.vqdlist();
          this.loading = false;
          this.$message({
            message: this.$t("message.camera.Save_successfully"),
            type: "success",
          });
        }else{
          this.loading = false;
          this.tableData[index_xlh].open_close=!enable;
          this.$message({
            message: this.$t("message.camera.Editor_failure"),
            type: "success",
          });
          
        }
      })
    },
    //Delete
    handledel(index, row) {
      if (this.vqddatalist[row.token] == undefined) {
        console.log(this.vqddatalist[row.token]);
        return
      }
      this.loading = true;
      var index_xlh = (this.currentPage - 1) * 10 + index;
      this.tableData[index_xlh].open_close=false;
      let url = this.$store.state.IPPORT+ "/api/v2/DeleteVqd?token="+row.token+"&session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        console.log(result);
        if (result.data.bStatus) {
          this.loading = false;
          this.vqdlist();
          this.$message({
            message:this.$t("message.camera.Delete_successful"),
            type:"success"
          })
        }else{
          this.loading = false;
          this.$message({
            message:this.$t("message.camera.Delete_failed"),
            type:"success"
          })
        }
      })
    },
    onchange(){
      let url = this.$store.state.IPPORT+ "/api/v2/IntervalTime?number="+this.value+"&session=" + this.$store.state.token;
      console.log(url);
      this.$http.get(url).then((result) => {
        if (result.data.bStatus) {
          this.$message({
            message: 'Take effect after restart',
            type: "success",
          });
          this.$root.bus.$emit('webrtc', true);
        }
        if (result.data.bStatus!=true) {
          this.value = this.values
          this.$message({
            message: this.$t("message.camera.Editor_failure"),
            type: "success",
          });
        }
      })
    },
    //Search
    handlechange() {},
    //Test cabin
    loadtest() {
      let _this = this;
      var root = this.$store.state.IPPORT;
      //url
      var url =
        root + "/api/v1//GetSrcCamera?session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        if (result.status == 200) {
          var data = result.data;
          this.threevideo.push(data.src);
          console.log(data);
          var srcGroup = { children1: [] };
          srcGroup.label = this.$t("message.live.camera");
          srcGroup.iconclass = "iconfont icon-kaiqishexiangtou1";
          for (var i = 0; i < data.src.length; i++) {
            var item = data.src[i];
            if (item["nOriginalType"] == "H5_CH_GB") {
              continue;
            } else {
              var newItem = {
                token: item["strToken"],
                label: item["strName"],
              };

              srcGroup.children1.push(newItem);
              // this.tableData3.push(newItem)
            }
          }
          this.data.push(srcGroup);
        }
      });
    },
    //sdk
    loadDevice() {
      var root = this.$store.state.IPPORT;
      //url
      var url = root + "/api/v1/GetDevice?session=" + this.$store.state.token;
      //Reorganization
      this.$http.get(url).then((result) => {
        if (result.status == 200) {
          var srcData = [];
          var data = result.data;
          this.threevideo.push(data.dev);
          for (var i = 0; i < data.dev.length; i++) {
            var item = data.dev[i];
            var srclevel = [];
            srclevel["strToken"] = item.strToken;
            srclevel["strName"] = item.strName;
            this.loadSrcdev(srclevel, srcData);
          }
        }
      });
    },
    loadSrcdev(srclevel, srcData) {
      var root = this.$store.state.IPPORT;
      var url =
        root +
        "/api/v1/GetDeviceSrc?token=" +
        srclevel.strToken +
        "&session=" +
        this.$store.state.token;
      // console.log("*******",url)
      this.$http.get(url).then((result) => {
          if (result.status == 200) {
            var data = result.data;
            var srcGroup = { children1: [] };
            srcGroup.label = srclevel.strName;
            srcGroup.iconclass = "iconfont icon-kaiqishexiangtou1";
            for (var i = 0; i < data.src.length; i++) {
              var item = data.src[i];
              var newItem = {
                token: item["strToken"],
                label: item["strName"],
                open_close:false,
              };
              srcGroup.children1.push(newItem);
            }
            this.data.push(srcGroup);
          }
        })
        .catch((error) => {
          console.log("GetSrc failed", error);
        });
    },
    //Digital warehouse machine
    NumberDevice() {
      var root = this.$store.state.IPPORT;
      //url
      var url = root + "/api/v1/GetGbDevice?session=" + this.$store.state.token;

      //Reorganization
      this.$http.get(url).then((result) => {
        if (result.status == 200) {
          var srcData = [];
          var data = result.data;
          this.threevideo.push(data.dev);
          console.log(data);
          for (var i = 0; i < data.dev.length; i++) {
            var item = data.dev[i];
            var srclevel = [];
            srclevel["strToken"] = item.strToken;
            srclevel["strName"] = item.strName;
            this.NumberSrc(srclevel, srcData);
          }
        }
      });
    },
    NumberSrc(srclevel, srcData) {
      var root = this.$store.state.IPPORT;
      var url =
        root +
        "/api/v1/GetGbDeviceSrc?token=" +
        srclevel.strToken +
        "&session=" +
        this.$store.state.token;

      this.$http
        .get(url)
        .then((result) => {
          if (result.status == 200) {
            var data = result.data;
            console.log(data);
            var srcGroup = { children1: [] };
            srcGroup.label = srclevel.strName;
            srcGroup.iconclass = "iconfont icon-kaiqishexiangtou1";
            for (var i = 0; i < data.src.length; i++) {
              var item = data.src[i];

              var newItem = {
                token: item["strToken"],
                label: item["strName"],
                open_close: false,
              };
              srcGroup.children1.push(newItem);
              // this.tableData3.push(newItem);
            }
            this.data.push(srcGroup);
          }
        })
        .catch((error) => {
          console.log("GetSrc failed", error);
        });
    },

    //Tree node click
    handleNodeClick(data, checked, indeterminate) {
      console.log(data);
      this.currentPage = 1;
      this.tableData = [];
      var data1 =data
      var data = data.children;
      var tabledata = {};
      let open_closes = '';
      if (data) {
        for (var i = 0; i < data.length; i++) {
          if (this.vqddatalist[data[i].token]==='true') {
            open_closes=true;
          }else{
            open_closes=false;
          }
            tabledata = {
              name: data[i].label,
              token: data[i].token,
              open_close: open_closes,
            };
          
          this.tableData.push(tabledata);
        }
        console.log(this.tableData);
        this.total = this.tableData.length;
      }else{
        if (this.vqddatalist[data1['token']]==='true') {
          open_closes=true;
        }else{
          open_closes=false;
        }
        tabledata = {
          name: data1['label'],
          token: data1['token'],
          open_close: open_closes,
        };
        this.tableData.push(tabledata);
        // console.log(this.tableData);
        this.total = this.tableData.length;
      }
    },
    Allon(on) {
      console.log(on);
      this.loading = true;
      let root = this.$store.state.IPPORT;
      let tableData = this.tableData;
      if (tableData.length==0) {
        this.loading = false;
        this.$message({
                message: 'Please select a device',
                type: "warning",
              });
      }
      for (let i = 0; i < tableData.length; i++) {
        if (tableData[i].open_close) {
          console.log(tableData[i].open_close);
          this.loading = false;
        }else{
          let url = root+ "/api/v2/AddVqd?token="+tableData[i].token+"&enable="+on+"&session=" + this.$store.state.token;
          this.$http.get(url).then((result) => {
            console.log(result);
            this.loading = false;
            if (result.data.bStatus) {
              this.tableData[i].open_close = on;
              this.vqdlist();
              this.$message({
                message: this.$t("message.camera.Save_successfully"),
                type: "success",
              });
            }else{
              this.tableData[i].open_close=!on;
              this.$message({
                message: this.$t("message.camera.Editor_failure"),
                type: "success",
              });
            }
          })
        }
      }
    },
    Alloff(off) {
      console.log(off);
      this.loading=true;
      let root = this.$store.state.IPPORT;
      let tableData = this.tableData;
      if (tableData.length==0) {
        this.loading = false;
      }
      for (let i = 0; i < tableData.length; i++) {
        if (!tableData[i].open_close) {
          console.log(tableData[i].open_close);
          this.loading = false;
        }else{
          let url = root+ "/api/v2/AddVqd?token="+tableData[i].token+"&enable="+off+"&session=" + this.$store.state.token;
          this.$http.get(url).then((result) => {
            if (result.data.bStatus) {
              this.tableData[i].open_close = off;
              this.vqdlist();
              this.loading = false;
              this.$message({
                message: this.$t("message.camera.Save_successfully"),
                type: "success",
              });
            }else{
              this.loading = false;
              this.tableData[i].open_close=!off;
              this.$message({
                message: this.$t("message.camera.Editor_failure"),
                type: "success",
              });
            }
          })
        }
      }
    },
    //Pagination
    handleSizeChange(val) {
      console.log(`Per page ${val} strip`);
      this.currentPage = 1;
      this.pageSize = val;
    },
    handleCurrentChange(val) {
      console.log(`Current page: ${val}`);
      this.currentPage = val;
    },

    async handleSizeChange2(val) {
      await this.$nextTick();
      console.log(`Per page ${val} strip`);
      this.currentPage2 = 1;
      this.pageSize2 = val;
    },
    async handleCurrentChange2(val) {
      await this.$nextTick();
      console.log(`Current page: ${val}`);
      this.currentPage2 = val;
    },
    Searchtable(data,filterNode) {
      var data1 =data
      var tabledata = {};
      let open_closes = '';
       if (filterNode) {
         if (this.vqddatalist[data1['token']]==='true') {
          open_closes=true;
        }else{
          open_closes=false;
        }
        tabledata = {
          name: data1['label'],
          token: data1['token'],
          open_close: open_closes,
        };
        this.tableData.push(tabledata);
        // console.log(this.tableData);
        this.total = this.tableData.length;
      }
    },
    //Fuzzy query
    filterNode(value, data) {
      if (!value) return true;
      var data1= data.label.indexOf(value) !== -1;
      return data.label.indexOf(value) !== -1;
    },
  },
   //Fuzzy query
  watch: {
    filterText(val) {
      // console.log("filter", val);
      if (val) {
        this.tableData = [];
        this.total=0;
      }else{
        this.tableData = [];
        this.total=0;
      }
      this.$refs.tree.filter(val);
    },
  },
};
</script>
<style lang="scss" scoped>
.el-table__body-wrapper::-webkit-scrollbar {
  /*width: 0;width is 0 hidden*/
  width: 0px;
}
.vqd {
  width: 100%;
  height: 100%;
  display: flex;
  justify-content: space-between;
  .delete-icon{
    font-size: 20px;
    padding: 0;
  }
  .vqd_left {
    width: 17%;
    min-width: 290px;
    .vqd_left_input {
      margin: 5px 0;
    }
    .liveview_colltitle{
      display: flex;
      .liveview_titleicon1{
        width: 35px;
        height: 35px;
        background: url("~@/views/liveview/imgs/liveview_titleicon1.png") no-repeat;
        background: {
          size: 100%;
        };
      }
    }
    /deep/ .el-collapse .el-collapse-item {
      height: 100% !important;
      .el-collapse-item__wrap{
        .el-collapse-item__content{
          height: 74vh;
          padding: 0 10px;
          overflow: auto;
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
      }
    }
    .StatusPrompt {
      display:none;
    }
    .StatusPromptColor{
      color:#67cfb5 !important;
    }
  }
  .vqd_right {
    width: 83%;
    .liveview_right_but {
      height: 50px;
      display: flex;
      padding-left: 10px;
      justify-content: space-between;
      .form_butt {
        font-size: 12px;
        // color: #fff;
        background: none;
        border: 1px solid #fff !important;
        border-radius: 5px;
        opacity: 0.9;
        &:hover {
          border: 1px solid #3dabff !important;
          color: #3dabff !important;
        }
      }
      .liveview_right_but2 {
        display: flex;
        padding-right: 10px;
        .elselect{
          width: 100px;
        }
      }
      .addChannelSloganBtn {
        margin-left: auto;
        background: #3abbfe;
        border-radius: 2px;
        padding: 2px 16px;
        font-size: 14px;
        font-family: PingFang SC;
        font-weight: 600;
        color: rgba(255, 255, 255, 1);
        margin-right: 20px;
        border-color: none;
      }
    }
  }
}
</style>