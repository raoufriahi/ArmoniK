<template>
  <div class="liveview RTMP">
    <div class="liveview_left">
        <div class="titlevideo">
          video source
            <div style="position: absolute;top:2px; right:5px;">
                <span class="iconfont icon-refresh" @click="Refresh" style="cursor:pointer;"></span> &nbsp;
                <span class="iconfont icon-hanbaobao" style="font-size:20px;"></span>
            </div>
        </div>
        <el-input
            class="liveview_left_input"
            placeholder="Quick search"
            v-model="filterText">
            <span slot="suffix" ><i style="font-size:18px;">|</i>&nbsp;<i class="iconfont icon-jia" @click="qadd"></i></span>
        </el-input>
        <el-tree
            :data="data"
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
    <!-- Right video bar -->
    <div class="liveview_right" id="videoPanel">
      <div class="liveview_right_but">
          <div class="camera_top_button_right">
              <el-input v-model="search" @change="handlechange()" placeholder="Quick search">
                  <span slot="suffix" ><i style="font-size:18px;">|</i><i class="iconfont icon-sousuo"></i></span>
              </el-input>
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
                data.index.toLowerCase().includes(search.toLowerCase()) ||
                data.token.toLowerCase().includes(search.toLowerCase())
            )
            .slice((currentPage - 1) * pageSize, currentPage * pageSize)
        "
      >
		<el-table-column prop="i" :label="label.i" width="80"></el-table-column>
        <el-table-column prop="index" :label="label.index" width="120"></el-table-column>
        <el-table-column prop="token" :label="label.token" width="180"></el-table-column>
        <el-table-column prop="state" :label="label.state" width="180"></el-table-column>
        <el-table-column prop="name" :label="label.Name1" width="180"></el-table-column>
        <el-table-column prop="pushflow" :label="label.pushflow"></el-table-column>
        <el-table-column  label="Operate" width="180">
          <template slot-scope="scope">
            <div class="button_edi" style="color:#35a4da;">
              <span @click="handleEdit(scope.$index, scope.row)">Edit</span> &nbsp;&nbsp;
              <span @click="handledel(scope.$index, scope.row)">Delete</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column width="80" >
			<template slot="header">
				<div @click="refresh" style="cursor:pointer;background:#3abbfd;color:white">Refresh</div>
			</template>
        </el-table-column>
      </el-table>
      <!-- pagination -->
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
    <!-- Quickly add popups -->
    <el-dialog
        width="36%"
        title="Edit"
        :visible.sync="eddChannelSloganDialog"
        class="addChannelSlogan"
    >
		<div class="addtop">
			<div style="padding:10px 0;">Serial number : {{ index}}</div>
			<div style="padding:10px 0;">Device ID : {{ token}}</div>
			<div style="padding:10px 0;">Push URL</div>
			<el-input v-model="eddRTMP" placeholder="Url"></el-input>
		</div>
		<div slot="footer" class="dialog-footer">
			<CButton
			@click="eddChannelSloganDialog = false"
			class="EditChannelSloganBtn1"
			type="submit"
			>Cancel</CButton
			>
			<CButton
			@click="edditchaumbersure()"
			class="EditChannelSloganBtn"
			type="submit"
			>Confirm</CButton
			>
		</div>
    </el-dialog>
    <el-dialog
        width="36%"
        title="Push stream configuration"
        :visible.sync="AddChannelSloganDialog"
        class="addChannelSlogan"
    >
		<div class="addtop">
			<div style="padding:10px 0;">Push URL</div>
			<el-input v-model="RTMPINN" placeholder="Url"></el-input>
		</div>
		<div slot="footer" class="dialog-footer">
			<CButton
			@click="AddChannelSloganDialog = false"
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
import {listdatag,AllMethods} from '../public/regional'
import uuid from '../../assets/js/uuid'
export default {
    name: "Camera",
    data() {
        return {
            index:'',
            token:'',
            RTMPINN:"rtmp://192.168.0.111:8935/live/b1c3",
            eddRTMP:"",
            filterText: "", //Search bar
            data: [],
            defaultProps: {
                children: "children",
                label: "label",
                token: "token",
                iconclass: "iconclass",
            },
            search: "", //Search
            loading: false,
            //Pagination
            currentPage: 1, // Current page number
            total: 0, // Total number
            pageSize: 10, //Number of pages
            tableData: [],
			// state:'',
            label: {
                Name1: this.$t("message.camera.name"),
				// audio: this.$t("message.camera.audio"),
				// save: this.$t("message.camera.save"),
				pushflow:'Push URL',
				i:'Serial number',
				index:'Serial number',
				token:'Device ID',
				state:"State"
            },
            AddChannelSloganDialog: false,
            eddChannelSloganDialog: false,
            videoname: {},
			pfpeizhi:{},
			// pftoken:{},//Added token
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
		console.log(this.data);
		this.viewlist();
    },
    methods: {
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
		//Refresh
		refresh(){
			this.tableData=[];
			this.getpflist();
		},
		//Edit
		handleEdit(i,data){
			console.log(data);
			this.index = data.index;
			this.token = data.token;
			this.eddChannelSloganDialog=true;
			this.eddRTMP = data.pushflow;
		},
		edditchaumbersure(){
			let root = this.$store.state.IPPORT;
            let url = root + "/api/v2/UpdataRTMPPush?index="+this.index+"&token="+this.token+"&url="+this.eddRTMP+"&session=" + this.$store.state.token;
			console.log(url);
            this.$http.get(url).then((result) => {
				console.log(result);
				if (result.data.bStatus) {
					this.getpflist();
					this.eddChannelSloganDialog=false;
				}
            });
		},
		viewlist(){
            var url = this.$store.state.IPPORT + "/api/v1/GetSrc"+"?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.status === 200) {
                    console.log(result.data.src);
                    for (let i = 0; i < result.data.src.length; i++) {
                        this.videoname[result.data.src[i].strToken]=result.data.src[i].strName
                    }
                    this.getpflist();
                }
            })
        },
		getpflist(){
            let root = this.$store.state.IPPORT;
            let url = root + "/api/v2/GetRTMPPushList?session=" + this.$store.state.token;
            this.$http.get(url).then((result) => {
				this.tableData=[];
				let data = result.data.data.cam;
                console.log(data);
				for (let i = 0; i < data.length; i++) {
					var tabledata ={
						i:i+1,
						name: this.videoname[data[i].token],
						token: data[i].token,
						pushflow:data[i].url,
						state:data[i].state||'offline',
						index:data[i].index
					};
					this.tableData.push(tabledata);
				}
				
            });
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
		//Delete
		handledel(i,data){
			let root = this.$store.state.IPPORT;
            let url = root + "/api/v2/DeleteRTMPPush?index="+data.index+"&token="+data.token+"&session=" + this.$store.state.token;
			console.log(url);
            this.$http.get(url).then((result) => {
				console.log(result);
				if (result.data.bStatus) {
					this.getpflist();
				}
            });
		},
		//Add
        qadd(){
			if (this.pfpeizhi.token) {
				this.AddChannelSloganDialog=true;
				console.log(this.pfpeizhi);
				
			}
        },
		editchaumbersure(){
			let index = uuid(4, 16).toLowerCase(); 
			let root = this.$store.state.IPPORT;
            let url = root + "/api/v2/AddRTMPPush?index="+index+"&token="+this.pfpeizhi.token+"&url="+this.RTMPINN+"&session=" + this.$store.state.token;
			console.log(url);
            this.$http.get(url).then((result) => {
				console.log(result);
				if (result.data.bStatus) {
					this.getpflist();
					this.AddChannelSloganDialog=false;
				}
            });
			
			// this.pftoken[this.pfpeizhi.token]=this.pfpeizhi.token;
		// }
		},
        //tree node click
        handleNodeClick(data, checked, indeterminate) {
			if (data.token == undefined) {
				return
			}
			this.pfpeizhi=data;
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
					data["open_close"] = itme[i].bEnable;
					data["gbid"] = itme[i].strGbID;
					data["audio"] = itme[i].bEnableAudio;
					data["ChannelNumber"] = itme[i].nCh;
					this.tableData.push(data);
					for (var i = 0; i < this.tableData.length; i++) {
						for (var j = i + 1; j < this.tableData.length; j++) {
							if (this.tableData[i] == this.tableData[j]) {
								this.tableData.splice(j, 1);
								j--;
							}
						}
					}
					}
				}
				}
			});
		},

        //Pagination
        handleSizeChange(val) {
        console.log(`per page ${val} strip`);
        this.currentPage = 1;
        this.pageSize = val;
        },
        handleCurrentChange(val) {
        console.log(`Current page: ${val}`);
        this.currentPage = val;
        },
        //Fuzzy query
        filterNode(value, data) {
        if (!value) return true;
        return data.label.indexOf(value) !== -1;
        },
  },
  //Fuzzy query
  watch: {
    filterText(val) {
      console.log("filter", val);
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
            //This method is case sensitive! Therefore, the toLowerCase() method was required to convert all the queried content to lowercase.
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
.liveview {
  width: 100%;
  height: 100%;
  padding-top: 0px !important;
  display: flex;
  // flex-wrap: wrap;
  justify-content: space-between;
  .liveview_left {
    width: 12% !important;
    max-width: 220px;
    height: 100%;
    margin: 0;
    overflow: auto;
    &::-webkit-scrollbar {
      display: none;
    }
	
    .titlevideo{
        font-size: 16px;
        width: 100%;
        height: 50px;
        line-height: 50px;
        position: relative;
        padding-left: 15px;
    }
    .liveview_left_input {
      margin: 10px 0;
      border-radius: 0 !important;
    }
  }
  .liveview_right {
    width: 88% !important;
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
      }
    }
    .button_edi {
      margin-bottom: 0px;
      line-height: 27px;
      position: relative;
	  cursor:pointer;
      .el-button {
        // font-size: 20px;
        // padding: 0;
        background:rgba(55,62,72,1);
        border:1px solid #3ABBFE !important;
        padding: 4px 10px;
        font-size:12px;
        font-family:PingFang SC;
        font-weight:600;
        color:#32B3FE;
        box-sizing: border-box;
      }
      .el-button:nth-child(2) {
        // position: absolute;
        // top: 2px;
        // left: 15px;
        // font-size: 23px;
        // line-height: 25px;
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
    .addChannelSlogan {
        .addtop{
            padding: 40px 0;
            font-size: 16px;
        }
        /deep/.el-input__inner{
            border-radius: 0;
            border:none !important;
            border-bottom: 1px solid #DCDFE6 !important;
        }
        /deep/ .el-dialog {
            .el-dialog__body {
                //border-top: 1px solid #dcdfe6;
                //border-bottom: 1px solid #dcdfe6;
                min-height: 300px !important;
                overflow-y: hidden;
                padding-top: 0px !important;
            }
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
      // background: #3c3c3c !important;
      // border: none;
      border-radius: 0px !important;
    }
  } 
  /deep/ .el-input .el-input__inner{
    // background: #3c3c3c !important;
    // border: none;
    border-radius: 50px !important;
  }
}
.liveview_right /deep/ .el-input--suffix{
  line-height: 28px;
  .el-input__inner {
    padding-right: 0px;
  }
}
</style>