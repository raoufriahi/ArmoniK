<template>
  <div class="liveview">
    <div id="watermarktoggle Camera"></div>
    <!-- left data bar -->
    <div class="liveview_left">
      <el-input
        class="liveview_left_input"
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
                align-items: center;
              "
            >
              <div>{{ $t("message.live.device") }}</div>
              <div class="liveview_colltitle">
                <div class="liveview_titleicon1" @click.stop="Refresh('device')"></div>
              </div>
            </div>
          </template>
          <el-tree
            :data="data"
            show-checkbox
            node-key="id"
            :filter-node-method="filterNode"
            ref="tree"
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
    <!-- Right video bar -->
    <div class="liveview_right" id="videoPanel">
      <div class="liveview_right_but">
        <div class="camera_top_button_left">
          <CButton
            class="form_butt iconfont icon-duihao"
            @click="Allon(true)"
            type="submit"
            >All on</CButton
          >
          <CButton
            class="form_butt iconfont icon-jinyong"
            @click="Alloff(false)"
            type="submit"
            >Close all</CButton
          >
          <CButton
            class="form_butt iconfont icon-jinyong"
            @click="Allpart(false)"
            type="submit"
            >Disable offline</CButton
          >
          <CButton
            class="form_butt iconfont icon-baocun"
            @click="Allsave"
            type="submit"
            >Save all</CButton
          >
          </div>
          <div class="camera_top_button_right">
            <div calss="searchVideoSource">
              <el-input v-model="search" @change="handlechange()" size="mini" placeholder="Please enter key words">
                <i slot="suffix" class="iconfont icon-sousuo"></i>
              </el-input>
            </div>
            <CButton class="addChannelSloganBtn" @click="addChdialog" type="submit">Modify channel number</CButton>
        </div>
      </div>
      <el-table
        v-loading="loading"
        element-loading-text="Saving"
        element-loading-background="rgba(0, 0, 0, 0)"
        :stripe="false"
        :row-class-name="myclass"
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
        <el-table-column prop="token" :label="label.Token">
        </el-table-column>

        <el-table-column prop="name" :label="Name1">
        </el-table-column>
        <el-table-column :label="label.GBID" width="300">
          <template slot-scope="scope">
            <el-input
              @blur="openchange"
              v-model="scope.row.gbid"
              placeholder="Please enter content"
            ></el-input>
          </template>
        </el-table-column>
        <el-table-column label="Channel number">
          <template slot-scope="scope">
            <el-button class="channelClass" v-if="$store.state.darkMode=='c-dark-theme'" type="text" @click="changeChannelSlogan(scope.row)"  style="background-color:rgba(47,47,47,0.5); padding: 4px 20px;  font-size:12px;  color:#32B3FE;"> {{scope.row.ChannelNumber}}</el-button>
            <el-button class="channelClass" v-if="$store.state.darkMode=='blue'" type="text" @click="changeChannelSlogan(scope.row)"  style="background-color:rgba(28,36,66,0.5); padding: 4px 20px;  font-size:12px;  color:#32B3FE;"> {{scope.row.ChannelNumber}}</el-button>
            <el-button class="channelClass" v-if="$store.state.darkMode==false" type="text" @click="changeChannelSlogan(scope.row)"  style="background-color:rgba(255,255,255,0.1); padding: 4px 20px; font-size:12px; color:#32B3FE;border:1px solid #DCDFE6"> {{scope.row.ChannelNumber}}</el-button>
          </template>
        </el-table-column>
        <el-table-column :label="open_Close" width="100">
          <template slot-scope="scope">
            <el-switch
              @change="openchange"
              v-model="scope.row.open_close"
              :disabled="scope.row.disabled"
              active-color="#13ce66"
              inactive-color="#ff4949"
            >
            </el-switch>
          </template>
        </el-table-column>
        <el-table-column :label="audio" width="100">
          <template slot-scope="Scope">
            <el-switch
              @change="openchange"
              v-model="scope.row.audio"
              active-color="#13ce66"
              inactive-color="#ff4949"
            >
            </el-switch>
          </template>
        </el-table-column>
        <el-table-column  label="Operate">
          <template slot-scope="scope">
            <div class="button_edi">
              <el-button class="save-button" @click="handleEdit(scope.$index, scope.row)" type="text">保存</el-button>
              <el-button @click="handledel(scope.$index, scope.row)" type="text">清空</el-button>
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
    <!-- Add channel slogan popup -->
    <el-dialog
      title=""
      :visible.sync="AddChannelSloganDialog"
      class="addChannelSlogan"
    >
      <div slot="title">
        <div
          style="
            border-bottom: 2px solid #37bcff;
            width: 140px;
            text-alight: cnter;
          "
        >
          <span
            style="
              display: block;
              font-size: 14px;
              font-family: PingFang SC;
              font-weight: 500;
              text-align: center;
            "
            >Add channel number</span
          >
        </div>
      </div>
      <div style=" display: flex; justify-content: space-between; align-items: center; height: 7vh;" >
        <div>
          <span style="font-size: 14px; margin-right: 14px">Channel number</span>
          <div slot="footer" style="display: inline">
            <CButton
              class="EditChannelSloganBtn1"
              @click="addpartsCh()"
              type="submit"
              >Local update</CButton
            >
            <CButton class="EditChannelSloganBtn" @click="addCh()" type="submit"
              >Update all</CButton
            >
          </div>
          <el-input
            type="text"
            placeholder="(Enter the minimum value of 1)"
            style="
              width: 30%;
              margin-left: 15px;
              text-align: center;
              display: none;
            "
            v-model="chinput"
            ref="chNumber"
          ></el-input>
          <span style="margin-left: 15px; display: none">----</span>
          <el-input
            type="text"
            placeholder="(The maximum value is 20000)"
            style="width: 30%; margin-left: 15px; display: none"
            v-model="chinput2"
            ref="chNumber2"
          ></el-input>
        </div>
      </div>
      <div style="height: 550px">
        <el-table
          class="addChannelSloganTable"
          :data="
            tableData3.slice(
              (currentPage2 - 1) * pageSize2,
              currentPage2 * pageSize2
            )
          "
          stripe
          style="
            width: 100%;
            font-size: 16px;
            font-family: PingFang SC;
            font-weight: 500;
            height: 532px;
          "
        >
          <el-table-column
            label="Channel number"
            width="140"
            style="background: #353535"
            prop="nCh"
          >
          </el-table-column>
          <el-table-column label="Serial number" prop="strToken" width="240">
          </el-table-column>
          <el-table-column
            label="GB"
            width="240"
            prop="strGbID"
          ></el-table-column>
          <el-table-column width="140" label="Open/Close">
            <template slot-scope="scope">
              <el-switch 
                v-model="scope.row.bEnable"
                disabled
                active-color="#13ce66"
                inactive-color="#ff4949"
              >
              </el-switch>
            </template>
          </el-table-column>
          <el-table-column width="140" label="Audio">
            <template slot-scope="scope">
              <el-switch
                disabled
                v-model="scope.row.bEnableAudio"
                active-color="#13ce66"
                inactive-color="#ff4949"
              >
              </el-switch>
            </template>
          </el-table-column>
        </el-table>
      </div>
      <div class="block" style="text-align: center">
        <el-pagination
          @size-change="handleSizeChange2"
          @current-change="handleCurrentChange2"
          :current-page="currentPage2"
          layout="total,  prev, pager, next, jumper"
          :page-size="10"
          :total="zongyeshu"
        >
        </el-pagination>
      </div>
    </el-dialog>
    <!-- Channel number edit popup -->
    <el-dialog
      title=""
      :visible.sync="EditChannelSloganDialog"
      class="editChannelSlogan"
      @close="downeditdialog"
    >
      <div slot="title">
        <div style="display: flex; align-items: flex-start">
          <div>
            <span style="font-size: 16px">Select channel number mode</span>
            <el-select
              style="width: 50%; margin-left: 15px"
              v-model="valuech"
              @change="onSelecteCh(valuech)"
            >
              <el-option
                v-for="item in options"
                :key="item.value"
                :label="item.label"
                :value="item.value"
              ></el-option>
            </el-select>
          </div>

          <div>
            <span style="font-size: 16px">Current value of channel number</span>
            <el-input
              style="width: 50%; margin-left: 15px"
              v-model="chNowNumber"
            ></el-input>
          </div>
        </div>
      </div>

      <div class="EditChannelSloganDialogTabs">
        <el-tabs
          :tab-position="tabPosition"
          style="height: 600px"
          @tab-click="handleClick"
          v-model="activeName"
          ref="tabsLeftLineheight"
        >
          <el-tab-pane
            v-for="(v, i) in editchnumer"
            :key="i"
            :label="v"
            :name="v"
          >
            <div slot="label" ref="chnumermoshi" style="text-algin: center">
              <span style="font-size: 20px">{{ v }}</span>
            </div>
            <div
              style="
                width: 100%;
                height: 100%;
                display: flex;
                flex-direction: row;
                flex-wrap: wrap;
              "
            >
              <div
                style="
                  width: 12%;
                  line-height: 70px;
                  text-align: center;
                "
                v-for="(value, index) in editchnumers"
                :key="index"
              >
                <span
                  style="font-size: 20px; cursor: pointer"
                  :id="value"
                  ref="nowchnumber"
                  @click="editchanumber(value)"
                  >{{ value }}</span
                >
              </div>
            </div>
          </el-tab-pane>
        </el-tabs>
      </div>
      <div slot="footer" class="dialog-footer">
        <CButton
          @click="EditChannelSloganDialog = false"
          class="EditChannelSloganBtn1"
          type="submit"
          >Cancel</CButton
        >
        <CButton
          @click="editchaumbersure()"
          class="EditChannelSloganBtn"
          type="submit"
          >Confirm</CButton
        >
      </div>
    </el-dialog>
  </div>
</template>

<script>
import Vue from "vue";
import {listdatagload,listdatag,AllMethods} from '../../public/regional'
export default {
  name: "Camera",
  data() {
    return {
      search: "", //Search
      filterText: "", //Search bar
      activeNames: ["1"], //Left
      open_Close: this.$t("message.camera.open_Close"),
      Name1: this.$t("message.camera.name"),
      audio: this.$t("message.camera.audio"),
      save: this.$t("message.camera.save"),
      label: {
        Token: this.$t("message.table.Token"),
        GBID: this.$t("message.table.GBID"),
      },
      activeName: "1~64",
      //Pagination
      currentPage: 1, // Current page number
      total: 0, // Total number
      pageSize: 10, //Number of pages
      tableData: [],
      data: [],
      defaultProps: {
        children: "children",
        label: "label",
        token: "token",
        iconclass: "iconclass",
      },
      
      editform: {
        open_close: false,
        audio: false,
      },
      searchTableInfo: "",
      loading: false,

      tableData2: [],
      tableData3: [],
      tableData4: [],
      AddChannelSloganDialog: false,
      tabPosition: "left",
      EditChannelSloganDialog: false,
      chnumber: [],
      chinput: "",
      chinput2: "",

      editchnumer: [],
      editchnumers: [],

      chNowNumber: "",

      currentPage2: 1, // Current page number
      zongyeshu: 0, // Total number
      pageSize2: 10,
      options: [
        {
          value: "64",
        },
        {
          value: "128",
        },
        {
          value: "256",
        },
        {
          value: "512",
        },
        {
          value: "1024",
        },
      ],
      valuech: 64,
      partsUpdate: [],
      arrchannel: [],
      num1: "",
      deltetoken: "",
      addch2: [],
      threevideo: [],
      channeledit: [],
      chedit: [],
      refNamePopover: 'popover-', // popover ref name prefix
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
    // this.loadtest();
    // this.loadDevice();
    // this.NumberDevice();
  },
  methods: {
    //How to change cell style
    myclass({row,column,rowIndex,columnIndex}){
      if (row.iconclass3&&row.open_close==true) {
        if (row.iconclass3=='heightlight') {
          return 'setclass';
        }
      }
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
    sortNumber(a, b) {
      return a.nCh - b.nCh;
    },
    // Open the Add Channel Number popup
    addChdialog() {
      this.AddChannelSloganDialog = true;
      var root = this.$store.state.IPPORT;
      // url
      this.tableData3=[]
      var url = root + "/api/v1//GetCamera?session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        // console.log("a", result);
        if (result.status == 200) {
          this.tableData3 = result.data.cam
          this.zongyeshu = this.tableData3.length;
          this.tableData3 = result.data.cam.sort(
            (pre, cur) => pre.nCh - cur.nCh
          );
        }
      });
    },
    // Update all
    async addCh() {
      this.tableData2 = [];
      var root = this.$store.state.IPPORT;
      var url =
        root + "/api/v1/GetSrc?session=" + (await this.$store.state.token);
      this.$http
        .get(url)
        .then((result) => {
          // console.log("a", result);
          if (result.status == 200) {
            var itme = result.data.src;
            var tabledata = {};
            for (var i = 0; i < itme.length; i++) {
              tabledata = {
                name: itme[i].strName,
                token: itme[i].strToken,
                open_close: true,
                gbid: "",
                audio: false,
                disabled: false,
                ChannelNumber: itme[i].nChannelNumber,
              };
              if (itme[i].nType != "H5_CH_DEV") {
                tabledata["disabled"] = true;
              }
              this.tableData2.push(tabledata);
            }

            for (let key = 0; key < this.tableData2.length; key++) {
              var url =
                root +
                "/api/v1/DelCamera?token=" +
                this.tableData2[key].token +
                "&session=" +
                this.$store.state.token;
              // console.log("-****************", url);
              this.$http
                .get(url)
                .then((result) => {
                  // console.log(result);
                  if (result.data.bStatus == true) {
                    if (
                      this.tableData2[key].gbid == "" ||
                      this.tableData2[key].gbid == "invalid"
                    ) {
                      var addChUrl = `${root}/api/v1/AddCamera?token=${
                        this.tableData2[key].token
                      }&enable=${this.tableData2[key].open_close}&audio=${
                        this.tableData2[key].audio
                      }&ch=${key + 1}&session=${this.$store.state.token}`;
                    } else {
                      var addChUrl = `${root}/api/v1/AddCamera?token=${
                        this.tableData2[key].token
                      }&enable=${this.tableData2[key].open_close}&audio=${
                        this.tableData2[key].audio
                      }&gbid=${this.tableData2[key].gbid}&ch=${
                        key + 1
                      }&session=${this.$store.state.token}`;
                    }
                    this.$http
                      .get(addChUrl)
                      .then((result) => {
                        // console.log(result);
                        if (result.data.bStatus == true) {
                          for (let m = 0; m < this.tableData.length; m++) {
                            const element = this.tableData[m].token;
                            this.loadSrc(this.tableData[m], element);
                            this.AddChannelSloganDialog = false;
                          }
                        }
                      })
                      .catch((error2) => {
                        console.log("cuowu", error2);
                      });
                  }
                })
                .catch((error1) => {
                  console.log("cuowu-2111", error1);
                });
            }
          }
        })
        .catch((error) => {
          console.log("cuowu-111111111", error);
        });
    },

    // Local update
    async addpartsCh() {
      this.tableData2 = [];
      this.tableData4= [];
      var objchnumber =[];
      var arrch = []
      var root = this.$store.state.IPPORT;
      var url =
        root + "/api/v1/GetSrc?session=" + (await this.$store.state.token);
      this.$http
        .get(url)
        .then((result) => {
          console.log("a", result);
          if (result.status == 200) {
            var itme = result.data.src;
            var tabledata = {};
            for (var i = 0; i < itme.length; i++) {
              tabledata = {
                name: itme[i].strName,
                token: itme[i].strToken,
                open_close: true,
                gbid: "",
                audio: false,
                disabled: false,
                ChannelNumber: itme[i].nChannelNumber,
              };
              if (itme[i].nType != "H5_CH_DEV") {
                tabledata["disabled"] = true;
              }
                this.tableData2.push(tabledata);
            }
            var obj ={};
            var datach = {};
            for (let n = 0; n < this.tableData3.length; n++) {
              var key = this.tableData3[n].strToken;
              obj[key] =this.tableData3[n].strToken;
              var chnumber = this.tableData3[n].nCh;
              objchnumber.push(chnumber)
              if(this.tableData3[n].nCh==0){
                datach = {
                  token: this.tableData3[n].strToken,
                  open_close: this.tableData3[n].bEnable,
                  gbid:this.tableData3[n].strGbID,
                  audio: this.tableData3[n].bEnableAudio,
                  disabled: false,
                  ChannelNumber: this.tableData3[n].nCh,
              };
              this.tableData4.push(datach);
              }
            }
            for (let key = 0; key < this.tableData2.length; key++) {
              if(obj.hasOwnProperty(this.tableData2[key].token)==false){
                this.tableData4.push(this.tableData2[key]);
              }
            }
            for (let i = 1; i < 1024; i++) {
              if (objchnumber.indexOf(i) == -1) {
                arrch.push(i)
              }
            }
            let addChUrl = '';
            let arr = [];

            for (let m = 0; m < this.tableData4.length;m++) {
              var url =
                root +
                "/api/v1/DelCamera?token=" +
                this.tableData4[m].token +
                "&session=" +
                this.$store.state.token;
              // console.log("-****************", url);
              this.$http
                .get(url)
                .then((result) => {
                  console.log(result);
                  if (result.data.bStatus == true) {
                    let gbid = this.tableData4[m].gbid;
                    let token = this.tableData4[m].token;
                    let open_close = this.tableData4[m].open_close;
                    let audio = this.tableData4[m].audio;
                    if (gbid == "" || gbid == "invalid") {
                          addChUrl = `${root}/api/v1/AddCamera?token=${
                            token
                          }&enable=${open_close}&audio=${
                            audio
                          }&ch=${arrch[m]}&session=${this.$store.state.token}`;
                      } else {
                          addChUrl = `${root}/api/v1/AddCamera?token=${
                            token
                          }&enable=${open_close}&audio=${
                            audio
                          }&gbid=${gbid}&ch=${arrch[m]}&session=${
                            this.$store.state.token
                          }`;
                    }
                    arr.push(addChUrl)
                    this.addpartsChannel(arr)
                  }}).catch((error) => {
                  console.log("cuowu1", error);
              })
            }
          }
        })
        .catch((error) => {
          console.log("cuowu", error);
        });
    },
    addpartsChannel(arr){
      for (let i = 0; i < arr.length; i++) {
        console.log(arr[i]);
          this.$http
              .get(arr[i])
              .then((result) => {
                // console.log(result);
                if (result.status == 200 && result.data !== undefined) {
                  if(result.data.bStatus == true){
                    for (let m = 0; m < this.tableData.length; m++) {
                      const element = this.tableData[m].token;
                      this.loadSrc(this.tableData[m], element);
                      this.AddChannelSloganDialog = false;
                    }
                  }
                }
              })
          .catch((error) => {
            console.log("cuowu2", error);
          });
      }
    },
    //   Edit select channel number

    // Open the Edit Select Channel Number popup
    changeChannelSlogan(row) {
      this.addch2 = row;
      this.num1 = row.ChannelNumber;
      this.editchnumer = [];
      this.arrchannel = [];
      this.EditChannelSloganDialog = true;
      this.deltetoken = row.token;
      // Current value of channel number
      this.chNowNumber = row.ChannelNumber;

      if (row.ChannelNumber > 64 && row.ChannelNumber < 129) {
        this.valuech = 128;
      } else if (row.ChannelNumber > 128 && row.ChannelNumber < 257) {
        this.valuech = 256;
      } else if (row.ChannelNumber > 256 && row.ChannelNumber < 513) {
        this.valuech = 512;
      } else if (row.ChannelNumber > 512 && row.ChannelNumber < 1025) {
        this.valuech = 1024;
      }

      //define the outer array
      for (var i = 1; i <= 1024; i++) {
        //Loop through
        if (i % 64 == 1) {
          //Whenever the remainder is 1, the array is rebuilt,
          var a = [];
          this.arrchannel.push(a); //Save the array to the outer array
        }
        a.push(i); //Store the elements in the array
      }
      for (let j = 0; j < 1 || j < row.ChannelNumber / 64; j++) {
        this.editchnumer.push(
          `${this.arrchannel[j][0]}~${this.arrchannel[j].slice(-1)}`
        );
        for (let g = 0; g < this.editchnumer.length; g++) {
          this.activeName = this.editchnumer[g];
          for (let k = 0; k < this.arrchannel.length; k++) {
            const element = this.arrchannel[g];
            this.editchnumers = element;
          }
        }
      }

      var root = this.$store.state.IPPORT;
      var url = root + "/api/v1/GetCamera?session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        if (result.status == 200) {
          if (result.data.bStatus == false) {
          } else {
            var itme = result.data.cam;
            this.channeledit = [];
            for (var i = 0; i < itme.length; i++) {
              this.channeledit.push(itme[i].nCh);
            }

            this.$nextTick(() => {
              var numcolor = this.$refs.nowchnumber;
              for (let m = 0; m < numcolor.length; m++) {
                for (let s = 0; s < this.channeledit.length; s++) {
                  this.chedit = this.channeledit[s];
                  if (this.chedit == numcolor[m].innerText) {
                    numcolor[m].style.color = "#FFFFFF";
                    numcolor[m].style.opacity = "0.2";
                    numcolor[m].style.pointerEvents = "none";
                  }
                }
              }
            });
            this.$nextTick(() => {
              var numcolor = this.$refs.nowchnumber;
              for (let m = 0; m < numcolor.length; m++) {
                if (this.chNowNumber == numcolor[m].innerText) {
                  numcolor[m].style.color = "#FF0000";
                  numcolor[m].style.opacity = "1";
                } else {
                  numcolor[m].style.color = "";
                }
              }
            });
          }
        }
      });
    },
    // Close the edit channel number popup
    downeditdialog() {
      this.$nextTick(() => {
        var numcolor = this.$refs.nowchnumber;
        for (let m = 0; m < numcolor.length; m++) {
          numcolor[m].style.color = "";
          numcolor[m].style.opacity = "1";
          numcolor[m].style.pointerEvents = "auto";
        }
      });
      this.valuech = 64;
    },
    // Edit OK button
    async editchaumbersure() {
      var num = this.chNowNumber;
      if (this.num1 == num) {
        return;
      }
      for (let m = 0; m < this.channeledit.length; m++) {
        const element = this.channeledit[m];
        if (element == num) {
          return;
        }
      }

      var root = this.$store.state.IPPORT;
      var url =
        root +
        "/api/v1/DelCamera?token=" +
        this.deltetoken +
        "&session=" +
        (await this.$store.state.token);
      // console.log("-****************", url);
      this.$http.get(url).then((result) => {
        if (result.data.bStatus == true) {
          var num = this.chNowNumber;
          if (this.addch2.gbid == "" || this.addch2.gbid == "invalid") {
            var addChUrl = `${root}/api/v1/AddCamera?token=${this.addch2.token}&enable=${this.addch2.open_close}&audio=${this.addch2.audio}&ch=${num}&session=${this.$store.state.token}`;
          } else {
            var addChUrl = `${root}/api/v1/AddCamera?token=${this.addch2.token}&enable=${this.addch2.open_close}&audio=${this.addch2.audio}&gbid=${this.addch2.gbid}&ch=${num}&session=${this.$store.state.token}`;
          }
          this.$http
            .get(addChUrl)
            .then((result) => {
              this.EditChannelSloganDialog = false;
              this.loadSrc(this.addch2, this.addch2.token);
            })
            .catch((error) => {
              console.log("cuowu", error);
            });
        }
      });
    },
    // Select select channel number mode
    onSelecteCh(value) {
      this.editchnumer = [];
      for (let j = 0; j < value / 64; j++) {
        this.editchnumer.push(
          `${this.arrchannel[j][0]}~${this.arrchannel[j].slice(-1)}`
        );
      }
    },
    // Edit channel number left click event
    handleClick(tab) {
      this.$nextTick(() => {
        var numcolor = this.$refs.nowchnumber;
        for (let m = 0; m < numcolor.length; m++) {
          numcolor[m].style.color = "";
          numcolor[m].style.opacity = "1";
          numcolor[m].style.pointerEvents = "auto";
        }
      });
      for (let k = 0; k < this.arrchannel.length; k++) {
        const element = this.arrchannel[tab.index];
        this.editchnumers = element;
      }
      this.$nextTick(() => {
        var numcolor = this.$refs.nowchnumber;
        for (let m = 0; m < numcolor.length; m++) {
          for (let t = 0; t < this.channeledit.length; t++) {
            this.chedit = this.channeledit[t];
            if (this.chedit == numcolor[m].innerText) {
              numcolor[m].style.color = "#FFFFFF";
              numcolor[m].style.opacity = "0.2";
              numcolor[m].style.pointerEvents = "none";
            }
          }
        }
      });
      this.$nextTick(() => {
        var numcolor = this.$refs.nowchnumber;
        for (let m = 0; m < numcolor.length; m++) {
          if (this.chNowNumber == numcolor[m].innerText) {
            numcolor[m].style.color = "#FF0000";
            numcolor[m].style.opacity = "1";
          } else {
            if (this.chedit == numcolor[m].innerText) {
              numcolor[m].style.color = "#FFFFFF";
              numcolor[m].style.opacity = "0.2";
              numcolor[m].style.pointerEvents = "none";
            }
          }
        }
      });
    },
    // Edit right click event
    editchanumber(value) {
      this.chNowNumber = value;
      var numcolor = this.$refs.nowchnumber;
      for (let m = 0; m < numcolor.length; m++) {
        if (this.chNowNumber == numcolor[m].innerText) {
          numcolor[m].style.color = "#FF0000";
        } else {
          numcolor[m].style.color = "";
        }
      }
      for (let s = 0; s < this.channeledit.length; s++) {
        this.chedit = this.channeledit[s];
        $("#" + this.chedit).css("pointerEvents", "none");
        $("#" + this.chedit).css("color", "#FFFFFF");
        $("#" + this.chedit).css("opacity", "0.2");
      }
      this.$nextTick(() => {
        var numcolor = this.$refs.nowchnumber;
        for (let m = 0; m < numcolor.length; m++) {
          if (this.chNowNumber == numcolor[m].innerText) {
            numcolor[m].style.color = "#FF0000";
            numcolor[m].style.opacity = "1";
          } else {
            for (let s = 0; s < this.channeledit.length; s++) {
              this.chedit = this.channeledit[s];
              if (this.chedit == numcolor[m].innerText) {
                numcolor[m].style.color = "#FFFFFF";
                numcolor[m].style.opacity = "0.2";
                numcolor[m].style.pointerEvents = "none";
              }
            }
          }
        }
      });
    },
    //Disable offline
    async Allpart(off) {
      // return
      var root = this.$store.state.IPPORT;
      var tableData = this.tableData;
      console.log("Close all", tableData);

      var open_close = off;
      for (var i = 0; i < tableData.length; i++) {
        console.log(tableData[i].bOnline);
        var numindex = tableData.length - 1;
        this.tokenflag = tableData[numindex].token;
        // return
        if (tableData[i].disabled) {
          return;
        }
        if (!tableData[i].bOnline) {
          var url1 =
            root +
            "/api/v1/DelCamera?token=" +
            tableData[i].token +
            "&session=" +
            this.$store.state.token;
          await this.$http.get(url1).then((result) => {
            console.log(result);
            if (result.status == 200) {
              if (result.data.bStatus == true) {
                var tabledata = {
                  name: tableData[i].name,
                  token: tableData[i].token,
                  open_close: open_close,
                  gbid: tableData[i].gbid,
                  audio: tableData[i].audio,
                  disabled: tableData[i].disabled,
                  ChannelNumber: tableData[i].ChannelNumber,
                };
                this.tableData.splice(i, 1, tabledata);
                this.Allpublic(
                  root,
                  tableData[i].token,
                  open_close,
                  tableData[i].gbid,
                  tableData[i].audio,
                  tableData[i].ChannelNumber
                );
              }
            }
          });
        }
      }
    },
    //All on
    async Allon(on) {
      console.log("All on", this.tableData);
      // return
      var root = this.$store.state.IPPORT;
      var tableData = this.tableData;
      var open_close = on;
      for (var i = 0; i < tableData.length; i++) {
        console.log(tableData[i].token);
        var numindex = tableData.length - 1;
        this.tokenflag = tableData[numindex].token;
        if (tableData[i].disabled) {
          return;
        }
        var url1 =
          root +
          "/api/v1/DelCamera?token=" +
          tableData[i].token +
          "&session=" +
          this.$store.state.token;
        await this.$http.get(url1).then((result) => {
          console.log(result);
          if (result.status == 200) {
            if (result.data.bStatus == true) {
              var tabledata = {
                name: tableData[i].name,
                token: tableData[i].token,
                bOnline: tableData[i].bOnline,
                open_close: open_close,
                gbid: tableData[i].gbid,
                audio: tableData[i].audio,
                disabled: tableData[i].disabled,
                ChannelNumber: tableData[i].ChannelNumber,
              };
              console.log(tabledata);
              this.tableData.splice(i, 1, tabledata);
              this.Allpublic(
                root,
                tableData[i].token,
                open_close,
                tableData[i].gbid,
                tableData[i].audio,
                tableData[i].ChannelNumber
              );
            }
          }
        });
      }
    },
    //Close all
    async Alloff(off) {
      console.log("Close all");
      // return
      var root = this.$store.state.IPPORT;
      var tableData = this.tableData;
      var open_close = off;
      for (var i = 0; i < tableData.length; i++) {
        console.log(tableData[i].token);
        var numindex = tableData.length - 1;
        this.tokenflag = tableData[numindex].token;
        if (tableData[i].disabled) {
          return;
        }
        var url1 =
          root +
          "/api/v1/DelCamera?token=" +
          tableData[i].token +
          "&session=" +
          this.$store.state.token;
        await this.$http.get(url1).then((result) => {
          console.log(result);
          if (result.status == 200) {
            if (result.data.bStatus == true) {
              var tabledata = {
                name: tableData[i].name,
                token: tableData[i].token,
                bOnline: tableData[i].bOnline,
                open_close: open_close,
                gbid: tableData[i].gbid,
                audio: tableData[i].audio,
                disabled: tableData[i].disabled,
                ChannelNumber: tableData[i].ChannelNumber,
              };
              this.tableData.splice(i, 1, tabledata);
              this.Allpublic(
                root,
                tableData[i].token,
                open_close,
                tableData[i].gbid,
                tableData[i].audio,
                tableData[i].ChannelNumber
              );
            }
          }
        });
      }
    },
    //Save all
    async Allsave() {
      console.log("Save all");
      // return
      var root = this.$store.state.IPPORT;
      var tableData = this.tableData;
      this.loading = true;
      if (tableData.length==0) {
        this.loading = false;
        this.$message({
                message: 'Please select a device',
                type: "warning",
              });
      }
      for (var i = 0; i < tableData.length; i++) {
        console.log(tableData[i].token);
        var numindex = tableData.length - 1;
        this.tokenflag = tableData[numindex].token;
        var url1 =
          root +
          "/api/v1/DelCamera?token=" +
          tableData[i].token +
          "&session=" +
          this.$store.state.token;
        await this.$http.get(url1).then((result) => {
          console.log(result);
          if (result.status == 200) {
            if (result.data.bStatus == true) {
              var tabledata = {
                name: tableData[i].name,
                token: tableData[i].token,
                bOnline: tableData[i].bOnline,
                open_close: tableData[i].open_close,
                gbid: tableData[i].gbid,
                audio: tableData[i].audio,
                disabled: tableData[i].disabled,
                ChannelNumber: tableData[i].ChannelNumber,
              };
              this.tableData.splice(i, 1, tabledata);
              console.log(tabledata);
              this.Allpublic(
                root,
                tableData[i].token,
                tableData[i].open_close,
                tableData[i].gbid,
                tableData[i].audio,
                tableData[i].ChannelNumber
              );
            }
          }
        });
      }
    },
    Allpublic(root, token, open_close, gbid, audio, ChannelNumber) {
      var url = "";
      if (gbid == "") {
        url =
          root +
          "/api/v1/AddCamera?token=" +
          token +
          "&enable=" +
          open_close +
          "&audio=" +
          audio +
          "&ch=" +
          ChannelNumber +
          "&session=" +
          this.$store.state.token;
      } else if (gbid.length == 20) {
        url =
          root +
          "/api/v1/AddCamera?token=" +
          token +
          "&enable=" +
          open_close +
          "&audio=" +
          audio +
          "&ch=" +
          ChannelNumber +
          "&gbid=" +
          gbid +
          "&session=" +
          this.$store.state.token;
      } else if (gbid.length == 7) {
        url =
          root +
          "/api/v1/AddCamera?token=" +
          token +
          "&enable=" +
          open_close +
          "&audio=" +
          audio +
          "&ch=" +
          ChannelNumber +
          "&session=" +
          this.$store.state.token;
      }
      this.$nextTick(() => {
        console.log("----------------", url);
        this.$http
          .get(url)
          .then((result) => {
            console.log(result);
            if (result.status == 200 && token == this.tokenflag) {
              this.loading = false;
              this.$message({
                message: this.$t("message.camera.Save_successfully"),
                type: "success",
              });
            }
          })
          .catch((err) => {
            console.log("111111111111111111111", err);
          });
      });

      this.editform["name"] = name;
      this.editform["token"] = token;
      this.editform["open_close"] = open_close;
      this.editform["audio"] = audio;
      this.editform["gbid"] = gbid;
    },
    //Tips after modification
    openchange() {
      this.$message({
        message: "Please save after modification",
        type: "warning",
        center: true,
      });
    },
    //The data of the first table
    GetSrc() {
      var root = this.$store.state.IPPORT;
      //url
      var url = root + "/api/v1/GetSrc?session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        console.log("a", result);
        if (result.status == 200) {
          var itme = result.data.src;
          var tabledata = {};
          for (var i = 0; i < itme.length; i++) {
            tabledata = {
              name: itme[i].strName,
              token: itme[i].strToken,
              open_close: true,
              gbid: "",
              audio: false,
              disabled: false,
              ChannelNumber: itme[i].nChannelNumber,
            };
            if (itme[i].nType != "H5_CH_DEV") {
              // console.log(itme[i].nType)
              tabledata["disabled"] = true;
            }
            this.tableData.push(tabledata);
            console.log(this.tableData, 2222);
            this.loadSrc(tabledata, itme[i].strToken);
          }

          this.total = this.tableData.length;
        }
      });
    },
    loadSrc(tabledata, token) {
      // console.log(token);
      var data = tabledata;
      // console.log(data);
      var root = this.$store.state.IPPORT;
      var url =
        root +
        "/api/v1/GetCamera?token=" +
        token +
        "&session=" +
        this.$store.state.token;
      // console.log(url);
      this.$http.get(url).then((result) => {
        if (result.status == 200) {
          // console.log(result);
          if (result.data.bStatus == false) {
          } else {
            var itme = result.data.cam;
            for (var i = 0; i < itme.length; i++) {
              //console.log(itme,itme[i].bEnableAudio)
              data["open_close"] = itme[i].bEnable;
              data["gbid"] = itme[i].strGbID;
              data["audio"] = itme[i].bEnableAudio;
              data["ChannelNumber"] = itme[i].nCh;
              this.tableData.push(data);
              // console.log(this.tableData);
              for (var i = 0; i < this.tableData.length; i++) {
                for (var j = i + 1; j < this.tableData.length; j++) {
                  if (this.tableData[i] == this.tableData[j]) {
                    //The first is equivalent to the second, the splice method removes the second
                    this.tableData.splice(j, 1);
                    j--;
                    // console.log(this.tableData, 11111);
                  }
                }
              }
            }
          }
        }
      });
    },
    //Tree node click
    handleNodeClick(data, checked, indeterminate) {
      console.log(data);
      this.currentPage = 1;
      this.tableData = [];
      var root = this.$store.state.IPPORT;
      var data1 =data;
      var data = data.children;
      var tabledata = {};
      if (data) {
        for (var i = 0; i < data.length; i++) {
          // return false;
          tabledata = {
            name: data[i].label,
            token: data[i].token,
            bOnline: data[i].bOnline,
            open_close: true,
            gbid: "",
            audio: false,
            disabled: false,
            ChannelNumber: data[i].ChannelNumber,
            iconclass3:data[i].iconclass3
          };
          if (data[i].nType != "H5_CH_DEV") {
            // console.log(itme[i].nType)
            tabledata["disabled"] = true;
          }
          this.tableData.push(tabledata);
          // console.log(this.tableData);
          this.loadSrc(tabledata, data[i].token);
        }
      }else{
        tabledata = {
          name: data1['label'],
          token: data1['token'],
          bOnline: data1['bOnline'],
          open_close: true,
          gbid: "",
          audio: false,
          disabled: false,
          ChannelNumber: data1['ChannelNumber'],
          iconclass3:data1['iconclass3']
        };
        if (data1['nType'] != "H5_CH_DEV") {
            // console.log(itme[i].nType)
            tabledata["disabled"] = true;
          }
          this.tableData.push(tabledata);
          // console.log(this.tableData);
          this.loadSrc(tabledata, data1['token']);
      }
      this.total = this.tableData.length;
    },
    //Edit
    handleEdit(index, row) {
      console.log(index, row, row.gbid.length, row.gbid);
      var index_xlh = (this.currentPage - 1) * 10 + index;
      //return false;
      var root = this.$store.state.IPPORT;

      var url1 =
        root +
        "/api/v1/DelCamera?token=" +
        row.token +
        "&session=" +
        this.$store.state.token;
      console.log("-****************", url1);
      this.$http.get(url1).then((result) => {
        console.log(result);
        if (result.status == 200) {
          if (result.data.bStatus == true) {
            var tabledata = {
              name: row.name,
              token: row.token,
              open_close: row.open_close,
              gbid: row.gbid,
              audio: row.audio,
              disabled: row.disabled,
              ChannelNumber: row.ChannelNumber,
            };
            this.tableData.splice(index_xlh, 1, tabledata);
            console.log(row.gbid.length);
            var url = "";
            if (row.gbid == "") {
              console.log("&&row.open_close==true||row.audio==true");
              url =
                root +
                "/api/v1/AddCamera?token=" +
                row.token +
                "&enable=" +
                row.open_close +
                "&audio=" +
                row.audio +
                "&ch=" +
                row.ChannelNumber +
                "&session=" +
                this.$store.state.token;
            } else if (row.gbid.length == 20) {
              console.log("222");
              url =
                root +
                "/api/v1/AddCamera?token=" +
                row.token +
                "&enable=" +
                row.open_close +
                "&audio=" +
                row.audio +
                "&gbid=" +
                row.gbid +
                "&ch=" +
                row.ChannelNumber +
                "&session=" +
                this.$store.state.token;
            } else if (row.gbid.length == 7) {
              console.log("222");
              url =
                root +
                "/api/v1/AddCamera?token=" +
                row.token +
                "&enable=" +
                row.open_close +
                "&audio=" +
                row.audio +
                "&ch=" +
                row.ChannelNumber +
                "&session=" +
                this.$store.state.token;
            }

            console.log("----------------", url);
            this.$http.get(url).then((result) => {
              console.log(result);
              if (result.status == 200) {
                this.$message({
                  message: this.$t("message.camera.Save_successfully"),
                  type: "success",
                });
              }
            });

            this.editform["name"] = row.name;
            this.editform["token"] = row.token;
            this.editform["open_close"] = row.open_close;
            this.editform["audio"] = row.audio;
            this.editform["gbid"] = row.gbid;
          } else {
            console.log("Failed to save");
            // return false;
          }
        }
      });
    },
    //Delete
    handledel(index, row) {
      console.log(index, row);
      console.log("Serial number", (this.currentPage - 1) * 10 + index);
      var index_xlh = (this.currentPage - 1) * 10 + index;

      var root = this.$store.state.IPPORT;
      var url =
        root +
        "/api/v1/DelCamera?token=" +
        row.token +
        "&session=" +
        this.$store.state.token;
      console.log("-****************", url);
      this.$http.get(url).then((result) => {
        console.log(result);
        if (result.status == 200) {
          if (result.data.bStatus == true) {
            var tabledata = {
              name: row.name,
              token: row.token,
              open_close: true,
              gbid: "",
              audio: false,
              ChannelNumber: 0,
            };
            console.log(this.tableData);
            this.tableData.splice(index_xlh, 1, tabledata);
            this.$message({
              message: this.$t("message.camera.Delete_successful"),
              type: "success",
            });
          } else {
            this.$message({
              message: this.$t("message.camera.Delete_failed"),
              type: "warning",
            });
            return false;
          }
        }
      });
    },
    //Test cabin
    loadtest() {
      let _this = this;
      var root = this.$store.state.IPPORT;
      //url
      var url =
        root + "/api/v1//GetSrcCamera?session=" + this.$store.state.token;
      console.log(url);
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
            console.log(item, "00000000000000");
            if (item["nOriginalType"] == "H5_CH_GB") {
              continue;
            } else {
              var newItem = {
                token: item["strToken"],
                label: item["strName"],
                nType: item["nType"],
                bOnline: item["bOnline"],
                ChannelNumber: item["nChannelNumber"],
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
      console.log("*******", url);
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
                nType: item["nType"],
                bOnline: item["bOnline"],
                ChannelNumber: item["nChannelNumber"],
              };
              // console.log(newItem);
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
                nType: item["nType"],
                bOnline: item["bOnline"],

                ChannelNumber: item["nChannelNumber"],
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
      console.log(`Per page ${val} Strip`);
      this.currentPage2 = 1;
      this.pageSize2 = val;
    },
    async handleCurrentChange2(val) {
      await this.$nextTick();
      console.log(`Current page: ${val}`);
      this.currentPage2 = val;
    },
    Searchtable(data,filterNode) {
      // console.log(data);
      // this.tableData = [];
      var data1 =data
      var tabledata = {};
      if (filterNode) {
        tabledata = {
          name: data1['label'],
          token: data1['token'],
          bOnline: data1['bOnline'],
          open_close: true,
          gbid: "",
          audio: false,
          disabled: false,
          ChannelNumber: data1['ChannelNumber'],
          iconclass3:data1['iconclass3']
        };
        if (data1['nType'] != "H5_CH_DEV") {
          // console.log(itme[i].nType)
          tabledata["disabled"] = true;
        }
        this.tableData.push(tabledata);
        // console.log(this.tableData);
        this.loadSrc(tabledata, data1['token']);
        this.total = this.tableData.length;
      }
    },
    //Search
    handlechange() {},
    //Fuzzy query
    filterNode(value, data) {
      if (!value) return true;
      var data1= data.label.indexOf(value) !== -1;
      // console.log(data1);
      if (data1==true) {
        this.Searchtable(JSON.parse(JSON.stringify(data)),'filterNode')
      }
      return data.label.indexOf(value) !== -1;
    },
  },
  //fuzzy query
  watch: {
    filterText(val) {
      console.log("filter", val);
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
};
</script>

<style scoped lang="scss">
.c-app:not(.c-legacy-theme) .btn:not([class*="ghost"]):not([class*="link"]):not([class*="outline"]):not([class*="transparent"]) {
  border: 1px solid #FFFFFF;
}
.liveview {
  width: 100%;
  height: 100%;
  padding-top: 0px !important;
  display: flex;
  // flex-wrap: wrap;
  justify-content: space-between;
  /* watermark */
  #watermarktoggle {
    position: fixed;
    z-index: 100;
    width: 100%;
    height: 100%;
    left: 0;
    top: 0;
    display: block;
    pointer-events: none;
    margin-top: 40px;
  }
  .liveview_left {
    background: none !important;
    width: 16%;
    min-width: 290px;
    margin: 0 5px;
    &::-webkit-scrollbar {
      display: none;
    }
    .liveview_left_input {
      margin: 5px 0;
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
  .liveview_right {
    width: 84%;
    .liveview_right_but {
      height: 50px;
      display: flex;
      align-items: center;
      justify-content:space-between; 
      .form_butt {
        font-size: 12px;
        background: none;
        border-radius: 5px;
        margin-right: 10px;
        opacity: 0.9;
        &:hover {
          border: 1px solid #3dabff !important;
          color: #3dabff !important;
        }
      }
      .camera_top_button_right{
        display: flex;
        align-items: center;
        justify-content:space-between; 
        .addChannelSloganBtn {
          margin-left: auto;
          background: #3abbfe;
          border-radius: 2px;
          padding: 2px 16px;
          font-size: 14px;
          font-family: PingFang SC;
          font-weight: 600;
          color: rgba(255, 255, 255, 1);
          margin-left: 10px;
          border: none !important;
        }
      }
    }
    .button_edi {
      margin-bottom: 0px;
      line-height: 27px;
      position: relative;
      .el-button {
        border:1px solid #3ABBFE !important;
        padding: 4px 10px;
        font-size:12px;
        font-family:PingFang SC;
        font-weight:600;
        color:#32B3FE;
        box-sizing: border-box;
      }
      .el-button:nth-child(2) {
        background:#3ABBFE;
        border-radius:2px;
        padding: 4px 10px;
        font-size:12px;
        font-family:PingFang SC;
        font-weight:600;
        color:#FFFFFF;
        margin-right: 20px;
        border: none !important;
        margin-left: 5px;
      }
    }
  }
}
.addChannelSlogan {
  /deep/ .el-dialog {
    .el-dialog__body {
      max-height: 500px !important;
      min-height: 700px;
      overflow-y: hidden;
      padding-top: 0px !important;
    }
  }
}

.EditChannelSloganBtn {
  margin-left: auto;
  background: #3abbfe;
  border-radius: 2px;
  padding: 2px 16px;
  font-size: 14px;
  font-family: PingFang SC;
  font-weight: 600;
  color: rgba(255, 255, 255, 1);
  margin-right: 0px;
  border: none !important;
}
.EditChannelSloganBtn1 {
  margin-right: 20px;
  background: rgba(55, 62, 72, 1);
  border: 1px solid #3abbfe !important;
  padding: 2px 16px;
  font-size: 14px;
  font-family: PingFang SC;
  font-weight: 600;
  color: rgba(255, 255, 255, 1);
  box-sizing: border-box;
}

.EditChannelSloganDialogTabs .el-tabs__nav-wrap::after {
  background-color: transparent !important;
}
.EditChannelSloganDialogTabs .el-tabs__active-bar {
  background-color: transparent !important;
}
  .el-table {
  /deep/ .has-gutter th{
    text-align: center;
  }
  /deep/ .el-table__body tr td{
    text-align: center;
  }
}

.liveview_right {
  .el-table  /deep/ .el-input{
    width: 70%;
    .el-input__inner{
      border-radius: 0px !important;
    }
  } 
  /deep/ .el-input .el-input__inner{
    border-radius: 50px !important;
  }
}
/deep/ .setclass{
  background-color: rgba(255,73,73,0.5) !important;
}
.liveview_right /deep/ .el-input--suffix{
  line-height: 28px;
  .el-input__inner {
    padding-right: 0px;
  }
}
.el-pagination{
  /deep/ .el-input .el-input__inner{
    border-radius: 0px !important;
  }
}
</style>