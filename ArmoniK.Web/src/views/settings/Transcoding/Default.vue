<template>
    <div>
        <!-- Edit popup -->
        
        <el-dialog width="25%" :title="label.Edit" :visible.sync="editPopup">
            <el-form class="el_form" ref="form" label-position='left' label-width="100px" size="small " :model="editform">
                <el-form-item :label="label.Name">
                    <el-input disabled v-model="editform.strName" @input="change($event)"></el-input>
                </el-form-item>
                <el-form-item :label="label.Token">
                    <el-input disabled v-model="editform.strToken" @input="change($event)"></el-input>
                </el-form-item>
                <el-form-item :label="label.Codec">
                    <!-- <el-input disabled v-model="editform.nCodec"></el-input> -->
                    <el-select v-model="editform.nCodec" placeholder="Please choose" @input="change($event)">
                        <el-option
                            v-for="item in codec"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.Engine">
                    <el-select v-model="editform.nEngine" placeholder="Please choose" @input="change($event)">
                        <el-option
                            v-for="item in engine"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.Bitrate">
                    <el-input type="number" v-model="editform.nBitrate" @input="change($event)"></el-input>
                </el-form-item>
                <el-form-item :label="label.FPSType">
                    <el-select v-model="editform.nFPSType" placeholder="Please choose" @input="change($event)">
                        <el-option
                            v-for="item in fpstype"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                    <!-- <input class="editinput" v-model="form.nFPSType"/> -->
                </el-form-item>
                <el-form-item :label="label.FPS">
                    <el-input type="number" v-model="editform.nFPS" @input="change($event)"></el-input>
                </el-form-item>
                <el-form-item :label="label.ScaleType">
                    <el-select v-model="editform.nScaleType" placeholder="Please choose" @input="change($event)">
                        <el-option
                            v-for="item in scaletype"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                    <!-- <input class="editinput" v-model="form.nScaleType"/> -->
                </el-form-item>
                <!-- v-if="form.Type!='H5_ONVIF'" -->
                <el-form-item :label="label.WH">
                    <el-select v-model="editform.wihe" placeholder="Please choose" @input="change($event)">
                        <el-option
                            v-for="item in options"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer button_table">
                <el-button class="form_butt1" @click="editPopup = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="defedityes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>

        <el-table
            :data="tableData.filter(data => !search || data.Name.toLowerCase().includes(search.toLowerCase())).slice((currentPage1-1)*pageSize,currentPage1*pageSize)"
            style="width: 100%">
            <el-table-column
                prop="index"
                :label="label.Index"
                width="100">
            </el-table-column>
            <el-table-column
            prop="strName"
            :label="label.Name">
            </el-table-column>
            <el-table-column
            prop="strToken"
            :label="label.Token"
            min-width="80">
            </el-table-column>
            <el-table-column
            prop="nCodec"
            :label="label.Codec"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="nEngine"
            :label="label.Engine"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="nFPSType"
            :label="label.FPSType"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="nScaleType"
            :label="label.ScaleType"
            min-width="140">
            </el-table-column>
            <el-table-column
                fixed="right"
                min-width="140">
                <template slot="header" slot-scope="scope">
                    <el-input
                    v-model="search"
                    @change="handlechange(scope.$index,scope.row)"
                    size="mini"
                    placeholder="Enter keywords"/>
                </template>
                <template slot-scope="scope">
                    <!-- <el-button @click="handleClick(scope.$index,scope.row)" type="text" size="small">{{$t("message.setting.Detail")}}</el-button> -->
                    <el-button @click="handleEdit(scope.$index,scope.row)" type="text" size="small">{{$t("message.setting.edit")}}</el-button>
                </template>
            </el-table-column>
        </el-table>
        <!-- pagination -->
        <el-pagination
            style="text-align: center;"
            layout="prev, pager, next"
            @size-change="handleSizeChange1" 
            @current-change="handleCurrentChange1"
            :current-page="currentPage1"
            :total="total1">
        </el-pagination>
    </div>
</template>
<script>
import uuid from '../../../assets/js/uuid'

import '../../../assets/js/jQuery.md5.js'
  export default {
    data() {
        return {
            eltitle:this.$t("message.setting.Configuration"),
            search: '',//search
            activeName: 'Default',
            tabPosition: 'button',//Tab position
            inputedit:[],
            label:{
                label:this.$t("message.setting.Default"),//Pick 1
                label1:this.$t("message.setting.Profile"),//Pick 2
                Edit:this.$t("message.table.Edit"),
                Index:this.$t("message.table.Index"),
                Name:this.$t("message.table.Name"),
                Token:this.$t("message.table.Token"),
                Codec:this.$t("message.table.Codec"),
                Engine:this.$t("message.table.Engine"),
                FPSType:this.$t("message.table.FPSType"),
                ScaleType:this.$t("message.table.ScaleType"),

                Bitrate:this.$t("message.table.Bitrate"),
                FPS:this.$t("message.table.FPS"),
                WH:this.$t("message.table.WH")
            },
            //pagination
            form: {
                Type:"Default",
                strName:"Profile1",
                strToken:"",
                nCodec:"H5_CODEC_H264",
                nEngine:"H5_SW_ONLY",
                nBitrate:"1024",
                nFPSType:"H5_FPS_AUTO",
                nFPS:"25",
                nScaleType:"H5_SCALE_AUTO",
                wihe:"1920*1080",
            },
            editform: {
                Type:"",
                strName:"",
                strToken:"",
                nCodec:"",
                nEngine:"",
                nBitrate:"",
                nFPSType:"",
                nFPS:"",
                nScaleType:"",
                wihe:"",
            },
            codec: [{
                    value: 'H5_CODEC_H264',
                    label: 'H5_CODEC_H264'
                },{
                    value: 'H5_CODEC_AV1',
                    label: 'H5_CODEC_AV1'
                }
            ],
            engine: [{
                    value: 'H5_SW_ONLY',
                    label: 'H5_SW_ONLY'
                },{
                    value: 'H5_GPU_AUTO',
                    label: 'H5_GPU_AUTO'
                },{
                    value: 'H5_GPU_ONLY',
                    label: 'H5_GPU_ONLY'
                }
            ],
            fpstype: [{
                    value: 'H5_FPS_AUTO',
                    label: 'H5_FPS_AUTO'
                }, {
                    value: 'H5_FPS_FIXED',
                    label: 'H5_FPS_FIXED'
                }
                // , {
                //     value: 'H5_FPS_DYNAMIC',
                //     label: 'H5_FPS_DYNAMIC'
                // }
            ],
            scaletype: [{
                    value: 'H5_SCALE_AUTO',
                    label: 'H5_SCALE_AUTO'
                }, {
                    value: 'H5_SCALE_FIXED',
                    label: 'H5_SCALE_FIXED'
                }
                // , {
                //     value: 'H5_SCALE_DYNAMIC',
                //     label: 'H5_SCALE_DYNAMIC'
                // }
            ],
            options: [{
                    value: '1920*1080',
                    label: '1920*1080'
                }, {
                    value: '1280*720',
                    label: '1280*720'
                }, {
                    value: '704*576',
                    label: '704*576'
                }, {
                    value: '640*480',
                    label: '640*480'
                }, {
                    value: '320*240',
                    label: '320*240'
                }
                
            ],
            
            pageSize: 10,//Number of pages
            dialogFormVisible: false,//Add popup
            editPopup:false,//Edit popup
            edittoken:"",//The token to delete when editing
            editindex:"",//index when editing
            tableData: [],//1
            currentPage1: 1, // Current page number
            total1: 0, // Total number 1

        };
    },
    mounted(){
        this.loadstream();
        this.Gpu();
    },
    methods:{
        Gpu(){
            var url =this.$store.state.IPPORT + "/api/v1/GetGPUInfo?session=" + this.$store.state.token;
                // console.log("------------",url)
            this.$http.get(url).then(result => {
                if (result.status == 200) {
                    var data = result.data;
                    if(data.intel.length){
                        var eng={
                            value: 'H5_GPU_INTEL',
                            label: 'H5_GPU_INTEL'
                        }
                        this.engine.push(eng);
                        console.log(!data.intel.length,"145",this.engine)
                    }else if(data.nvidia.length){
                        var eng={
                            value: 'H5_GPU_NVIDIA',
                            label: 'H5_GPU_NVIDIA'
                        }
                        this.engine.push(eng);
                        console.log(!data.nvidia.length,"1454",this.engine)
                    }
                }
            })
        },

        loadstream(){
            this.editPopup = false;
            var url = this.$store.state.IPPORT + "/api/v1/GetTransProfileDefault?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
            //   return false;
              if(result.status == 200){
                  var itme=result.data.profile;
                  for(var i=0;i<itme.length;i++){
                      var tabledata={
                          index:i+1,
                          strName:itme[i].strName,
                          strToken:itme[i].strToken,
                          nCodec:itme[i].nCodec,
                          nEngine:itme[i].nEngine,
                          nBitrate:itme[i].nBitrate,
                          nFPSType:itme[i].nFPSType,
                          nFPS :itme[i].nFPS,
                          nScaleType:itme[i].nScaleType,
                          nWidth:itme[i].nWidth,
                          nHeight:itme[i].nHeight
                      };
                      this.tableData.push(tabledata);
                      
                  }
                  this.total1=this.tableData.length;
              }
            })
        },
        // Edit
        
        handleEdit(index,row){
            var index_xlh=((this.currentPage1-1)*10)+index;
            //return false;
            this.editPopup = true;
            this.edittoken=row.strToken;
            this.editindex=index_xlh;
            this.editform["Type"]=this.form.Type;
            this.editform["index"]=row.index;
            this.editform["strName"]=row.strName;
            this.editform["strToken"]=row.strToken;
            this.editform["nCodec"]=row.nCodec;
            this.editform["nEngine"]=row.nEngine;
            this.editform["nBitrate"]=row.nBitrate;
            this.editform["nFPSType"]=row.nFPSType;
            this.editform["nFPS"]=row.nFPS;
            this.editform["nScaleType"]=row.nScaleType;
            this.editform["wihe"]=row.nWidth+"*"+row.nHeight;
            console.log(this.editform);
            // console.log(this.tableData[index])
        },
        //  Edit Add OK button
        
        defedityes(){
            console.log("default",this.editindex,this.editform);
            var form=this.editform;
            var nWidhei= form.wihe.split("*");
            if(form.nBitrate<64||form.nBitrate>10240){
                console.log(form.nBitrate)
                this.$message({
                    message: 'Bitrate value must be in 64',
                    type: 'warning'
                });
                return false;
            }else if(form.nFPS<1||form.nFPS>30){
                console.log(form.nBitrate)
                this.$message({
                    message: 'FPS value must be at 1',
                    type: 'warning'
                });
                return false;
            }
            //url
            var url = this.$store.state.IPPORT + "/api/v1/SetTransProfileDefault?fpstype="+encodeURIComponent(form.nFPSType)+
            "&fps="+encodeURIComponent(form.nFPS)+
            "&scaletype="+encodeURIComponent(form.nScaleType)+
            "&width="+encodeURIComponent(nWidhei[0])+
            "&height="+encodeURIComponent(nWidhei[1])+
            "&bitrate="+encodeURIComponent(form.nBitrate)+
            "&engine="+encodeURIComponent(form.nEngine)+
            "&codec="+encodeURIComponent(form.nCodec)+
            "&session="+ this.$store.state.token;
            console.log(url);
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.tableData=[];
                        this.loadstream();
                    }else{
                        this.$message({
                            message: 'Edit failed',
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        //Randomly get token data when you click Add
        addto(){
            this.dialogFormVisible=true;
            this.form["strToken"] = uuid(4, 16).toLowerCase();
        },
        selectCall(row){
            console.log("INDEX",row);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    token:row[i].strToken,
                    index:row[i].index,
                    type:row[i].nFPSType,
                };
                this.selectop.push(selectop);
            }
            
        },
        select_Call(row){
            console.log(row);
            this.selectop=[];
            for(var i=0;i<row.length;i++){
                var selectop={
                    token:row[i].strToken,
                };
                this.selectop.push(selectop);
            }
        },
        //pagination
        change (e) {
            this.$forceUpdate()
        },
        handleSizeChange1(val) {
            console.log(`per page ${val} strip`);
            this.currentPage1 = 1;
            this.pageSize = val;
        },
        handleCurrentChange1(val) {
            console.log(`Current page: ${val}`);
            this.currentPage1 = val;
        },
    },
  };
</script>
