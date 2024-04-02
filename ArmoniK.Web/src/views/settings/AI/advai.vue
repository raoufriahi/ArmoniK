<template>
  <div class="vqd">
    <div class="vqd_left">
		<el-input
            class="vqd_left_input"
            placeholder="Quick search"
            v-model="filterText">
            <span slot="suffix" style="line-height:35px;" ><i style="font-size:18px;">|</i>&nbsp;<i class="iconfont icon-jia" style="cursor:pointer" @click="qadd()"></i></span>
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
                <div class="liveview_titleicon1" @click.stop="Refresh()"></div>
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
        <div></div>
        <div class="camera_top_button_right">
          <div class="concurrency">
            <span>Turn on data storage</span>&nbsp;
            <el-switch
              v-model="storage"
              @change="concurrency"
              active-color="#13ce66"
              inactive-color="#ff4949">
            </el-switch>
          </div>
          <div><el-input v-model="search" @change="handlechange()" placeholder="Quick search">
            <span slot="suffix" style="line-height:35px;" ><i style="font-size:18px;">|</i><i class="iconfont icon-sousuo"></i></span>
          </el-input></div>
        </div>
		</div>
        <el-table
          class="eltable"
          v-loading="loading"
          element-loading-text="Saving"
          element-loading-background="rgba(0, 0, 0, 0)"
          stripe
          style="width: 100%;"
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
                active-color="#13ce66"
                inactive-color="#ff4949"
              >
              </el-switch>
            </template>
          </el-table-column>
          <el-table-column prop="retriggertime" label="Trigger interval">
            <template slot-scope="scope">
                <el-select v-model="scope.row.retriggertime" size="mini" placeholder="Please choose" style="width: 100px;">
                    <el-option
                    v-for="item in options_time"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value">
                    </el-option>
                </el-select>
            </template>
          </el-table-column>
		  <el-table-column  label="Operate">
			<template slot-scope="scope">
				<div class="button_edi" style="color:#35a4da;cursor:pointer;">
					<span @click="handledel(scope.$index, scope.row)">{{$t('message.setting.DeleteAll')}}</span> &nbsp;&nbsp;
					<span @click="edit(scope.$index, scope.row)">{{$t('message.setting.edit')}}</span>
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
	<el-dialog
        width="34%"
        title="Add"
        :visible.sync="AddChannelSloganDialog"
        class="addChannelSlogan"
    >
		<div class="addtop">
			<div class="aidia">
				<div style="width:350px;padding:20px 0 20px;"><div>编号</div><br>
					<div>{{ai_date.token}}</div>
				</div>
          <div style="padding:20px 0 20px;"><div>Trigger interval</div><br>
					<el-select v-model="ai_date.retriggertime" size="mini" placeholder="Please choose" style="width: 100px;">
						<el-option
						v-for="item in options_time"
						:key="item.value"
						:label="item.label"
						:value="item.value">
						</el-option>
					</el-select>
				</div>
			</div>
			<div class="aidia">
				<div style="width:350px;padding:20px 0 20px;"><div>Name</div><br><div>{{ai_date.name}}</div></div>
				<div style="padding-top:20px;">
                    <div>Open/Close</div><br>
                    <el-switch
                        v-model="ai_date.enable"
                        active-color="#13ce66"
                        inactive-color="#ff4949"
                    >
                    </el-switch>
                </div>
			</div>
			
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
import {listdatagload,listdatag,AllMethods} from '../../public/regional'
export default {
  data() {
    return {
      storage:true,
      options_time: [{
          value: '3000',
          label: '3s'
      }, {
          value: '5000',
          label: '5s'
      }, {
          value: '10000',
          label: '10s'
      }],
      value: '',
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
      Aidatalist:{},
      Aitype:{},
      ai_time:{},// Trigger interval time
	  videoname:{},
	  filterText: "", // Search bar
	  aipeizhi:{},
	  AddChannelSloganDialog:false,
	  ai_date:{
		  retriggertime:'3s',
		  token:'',
		  name:'',
		  enable:true
	  },
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
    this.viewlist();
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
      // /api/v2/UpAdvStorage
      let url = this.$store.state.IPPORT+ "/api/v2/UpAdvStorage?storage="+this.storage+"&session=" + this.$store.state.token;
      this.$http.get(url).then((result)=>{
        console.log(result);
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
	  editchaumbersure(){
			let time = {'3s':'3000','5s':'5000','10s':'10000'};
			this.AddChannelSloganDialog = false;
			let retriggertime  = time[this.ai_date.retriggertime]||this.ai_date.retriggertime
			let url = this.$store.state.IPPORT+ "/api/v2/AddAdvTargetdetect?token="+this.ai_date.token+"&enable="+this.ai_date.enable+"&retriggertime="+retriggertime+"&session=" + this.$store.state.token;
			this.$http.get(url).then((result) => {
				this.loading = true;
				console.log(result);
				if (result.data.bStatus) {
				this.AIlist();
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
	  //Add
        qadd(){
			if (this.aipeizhi.token) {
				this.AddChannelSloganDialog=true;
				console.log(this.aipeizhi);
				this.ai_date.token=this.aipeizhi.token
				this.ai_date.name=this.aipeizhi.label
			}
        },
	  Refresh(){
		this.data=[];
		AllMethods().then(res=>{
			this.data = res.listdatag;
		}).catch(err=>{
			console.log(err);
		})
	},
	  viewlist(){
            var url = this.$store.state.IPPORT + "/api/v1/GetSrc"+"?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.status === 200) {
                    for (let i = 0; i < result.data.src.length; i++) {
                        this.videoname[result.data.src[i].strToken]=result.data.src[i].strName
                    }
                    this.AIlist();
                }
            })
        },
      AIlist(){
      this.Aidatalist={}
      let time = {'3000':'3s','5000':'5s','10000':"10s"}
      let url = this.$store.state.IPPORT+ "/api/v2/GetAdvList?session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
		  this.tableData=[];
		  var tabledata = {}
      this.storage = result.data.data.storage;
		  let data = result.data.data.cam;
			if (data.length!=0) {
				for (let i = 0; i < data.length; i++) {
					tabledata = {
						name: this.videoname[data[i].token],
						token: data[i].token,
						open_close: data[i].enable=='true'?true:false,
						retriggertime:data[i].retriggertime
					};
					this.tableData.push(tabledata);
				}
			}
        this.total = this.tableData.length;
      })
    },
    edit(index,date){
      this.ai_date.retriggertime = date.retriggertime
      this.ai_date.token = date.token
      this.ai_date.name = date.name
      this.ai_date.enable = date.open_close
      this.AddChannelSloganDialog = true;
    },
    //Delete
    handledel(index, row) {
      this.loading = true;
      var index_xlh = (this.currentPage - 1) * 10 + index;
      this.tableData[index_xlh].open_close=false;
      let url = this.$store.state.IPPORT+ "/api/v2/DeleteAdvTargetDetect?token="+row.token+"&session=" + this.$store.state.token;
      this.$http.get(url).then((result) => {
        if (result.data.bStatus) {
          this.loading = false;
          this.AIlist();
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
    //Search
    handlechange() {},
    //Tree node click
	handleNodeClick(data, checked, indeterminate) {
		if (data.token == undefined) {
			return
		}
		this.aipeizhi=data;
	},
    //Pagination
    handleSizeChange(val) {
      console.log(`Per page ${val} Strip`);
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
  /*width: 0; width is 0 hidden */
  width: 0px;
}
.el-select__caret.el-input__icon.el-icon-arrow-up{
    // display: none !important;
    line-height: 25px;
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
.eltable{
    overflow: auto;
    overflow-x: auto; 
    overflow-y: hidden;
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
.vqd {
  width: 100%;
  height: 100%;
  display: flex;
  justify-content: space-between;
  .vqd_left {
    width: 17%;
    min-width: 290px;
	.vqd_left_input{
		margin: 10px 0;
		border-radius: 0 !important;
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
        background: none;
        border: 1px solid #fff !important;
        border-radius: 5px;
        opacity: 0.9;
        &:hover {
          border: 1px solid #3dabff !important;
          color: #3dabff !important;
        }
      }
      .camera_top_button_right{
        display: flex;
        .concurrency{
          width: 300px;
          padding-top: 10px;
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
	.addtop{
		display: flex;
		flex-direction: column;
		font-size: 15px;
		padding-left: 10px;
		.el-input__inner{
            border-radius: 0;
            border:none !important;
            border-bottom: 1px solid #DCDFE6 !important;
        }
		.aidia{
			display: flex;
			flex-direction: row;
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
}
</style>