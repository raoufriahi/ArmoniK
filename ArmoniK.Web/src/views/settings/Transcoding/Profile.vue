<template>
    <div>
        <!-- Edit popup -->
        
        <el-dialog width="25%" :title="label.Edit" :visible.sync="editPopup">
            <el-form class="el_form" ref="form" label-position='left' label-width="100px" size="small " :model="editform">
                <el-form-item :label="label.Name">
                    <el-input v-model="editform.strName" @input="change($event)"></el-input>
                </el-form-item>
                <el-form-item :label="label.Token">
                    <el-input v-model="editform.strToken" @input="change($event)"></el-input>
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
                    <!-- <input class="editinput" v-model="form.nEngine"/> -->
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
                <el-button class="form_butt" type="primary" @click="proedityes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
         <!-- Added popup -->
        <el-dialog width="25%" :title="eltitle" :visible.sync="dialogFormVisible">
            <el-form class="el_form" ref="form" label-position='left' label-width="100px" size="small " :model="form">
                <el-form-item :label="label.Name">
                    <el-input v-model="form.strName"></el-input>
                </el-form-item>
                <el-form-item :label="label.Token">
                    <el-input v-model="form.strToken"></el-input>
                </el-form-item>
                <el-form-item :label="label.Codec">
                    <!-- <el-input disabled v-model="form.nCodec"></el-input> -->
                    <el-select v-model="form.nCodec" placeholder="Please choose" @input="change($event)">
                        <el-option
                            v-for="item in codec"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item :label="label.Engine">
                    <el-select v-model="form.nEngine" placeholder="Please choose">
                        <el-option
                            v-for="item in engine"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                    <!-- <input class="editinput" v-model="form.nEngine"/> -->
                </el-form-item>
                <el-form-item label="Bitrate">
                    <el-input type="number" v-model="form.nBitrate"></el-input>
                </el-form-item>
                <el-form-item :label="label.FPSType">
                    <el-select v-model="form.nFPSType" placeholder="Please choose">
                        <el-option
                            v-for="item in fpstype"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                        </el-option>
                    </el-select>
                    <!-- <input class="editinput" v-model="form.nFPSType"/> -->
                </el-form-item>
                <el-form-item label="FPS">
                    <el-input type="number" v-model="form.nFPS"></el-input>
                </el-form-item>
                <el-form-item :label="label.ScaleType">
                    <el-select v-model="form.nScaleType" placeholder="Please choose">
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
                <el-form-item label="Width*Height">
                    <el-select v-model="form.wihe" placeholder="Please choose">
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
                <el-button class="form_butt1" @click="dialogFormVisible = false">{{$t("message.setting.Cancel")}}</el-button>
                <el-button class="form_butt" type="primary" @click="platformyes">{{$t("message.setting.OK")}}</el-button>
            </div>
        </el-dialog>
        <!-- two tables -->
        <div class="button_edi">
            <CButton class="form_butt" @click="addto" type="submit">Add</CButton>
            <CButton class="form_butt1" @click="deleteselect" type="submit">Delete</CButton>
        </div>
        <!-- 表格 -->
        <el-table
            :data="tableData2.filter(data => !search || data.Name.toLowerCase().includes(search.toLowerCase())).slice((currentPage2-1)*pageSize,currentPage2*pageSize)"
            @select='selectCall'
            @select-all='select_Call'
            style="width: 100%">
            <el-table-column
                type="selection"
                width="55">
            </el-table-column>
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
                    <el-button @click="handleEdit(scope.$index,scope.row)" type="text" size="small">{{$t("message.setting.edit")}}</el-button>
                    <el-button @click.native.prevent="deleteRow(scope.$index,scope.row, tableData2)" type="text" size="small">{{$t("message.setting.DeleteAll")}}</el-button>
                </template>
            </el-table-column>
        </el-table>
        <!-- 分页 -->
        <el-pagination
            style="text-align: center;"
            layout="prev, pager, next"
            @size-change="handleSizeChange2" 
            @current-change="handleCurrentChange2"
            :current-page="currentPage2"
            :total="total2">
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
            search: '',//Search
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
            //Pagination
            form: {
                Type:"Default",
                strName:"Profile1",
                strToken:"",
                nCodec:"V1_CODEC_H264",
                nEngine:"V1_SW_ONLY",
                nBitrate:"1024",
                nFPSType:"V1_FPS_AUTO",
                nFPS:"25",
                nScaleType:"V1_SCALE_AUTO",
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
                    value: 'V1_CODEC_H264',
                    label: 'V1_CODEC_H264'
                },{
                    value: 'V1_CODEC_AV1',
                    label: 'V1_CODEC_AV1'
                }
            ],
            engine: [{
                    value: 'V1_SW_ONLY',
                    label: 'V1_SW_ONLY'
                },{
                    value: 'V1_GPU_AUTO',
                    label: 'V1_GPU_AUTO'
                },{
                    value: 'V1_GPU_ONLY',
                    label: 'V1_GPU_ONLY'
                }
            ],
            fpstype: [{
                    value: 'V1_FPS_AUTO',
                    label: 'V1_FPS_AUTO'
                }, {
                    value: 'V1_FPS_FIXED',
                    label: 'V1_FPS_FIXED'
                }
            ],
            scaletype: [{
                    value: 'V1_SCALE_AUTO',
                    label: 'V1_SCALE_AUTO'
                }, {
                    value: 'V1_SCALE_FIXED',
                    label: 'V1_SCALE_FIXED'
                }
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

            tableData2: [],//2
            total2: 0, // total number 2
            currentPage2: 1 // current page 2
        };
    },
    mounted(){
        this.loadstpro();
        this.Gpu();
    },
    methods:{
        Gpu(){
            let _this = this;
            var url = this.$store.state.IPPORT  + "/api/v1/GetGPUInfo?session=" + this.$store.state.token;
                // console.log("------------",url)
            this.$http.get(url).then(result => {
                if (result.status == 200) {
                    var data = result.data;
                    if(data.intel.length){
                        var eng={
                            value: 'V1_GPU_INTEL',
                            label: 'V1_GPU_INTEL'
                        }
                        this.engine.push(eng);
                        console.log(!data.intel.length,"145",this.engine)
                    }else if(data.nvidia.length){
                        var eng={
                            value: 'V1_GPU_NVIDIA',
                            label: 'V1_GPU_NVIDIA'
                        }
                        this.engine.push(eng);
                        console.log(!data.nvidia.length,"1454",this.engine)
                    }
                }
            })
        },
        loadstpro(){
            this.editPopup = false;
            var url =  this.$store.state.IPPORT  + "/api/v1/GetTransProfile?session="+ this.$store.state.token;
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
                      this.tableData2.push(tabledata);
                      
                  }
                  this.total2=this.tableData2.length;
              }
            })
        },
        handleEdit(index,row){
            var index_xlh="";
            //return false;
            index_xlh=((this.currentPage2-1)*10)+index;
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
        proedityes(){
 
            var form=this.editform;
            var nWidhei= form.wihe.split("*");
            // return false;
            console.log("1",form.strToken);
            var url =  this.$store.state.IPPORT + "/api/v1/DelTransProfile?token="+encodeURIComponent(this.edittoken)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                // console.log("1",result,url,form.strToken);
                // return false
                if(result.status==200){
                    if(result.data.bStatus==true){
                        this.Success(nWidhei);
                    }else{
                        this.$message({
                            message: this.$t("message.setting.Editorfailure"),
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        Success(nWidhei){
            var form=this.editform;
            if(form.nBitrate<64||form.nBitrate>10240){
                console.log(form.nBitrate)
                this.$message({
                    message: 'Bitrate value must be between 64-10240',
                    type: 'warning'
                });
                return false;
            }else if(form.nFPS<1||form.nFPS>30){
                console.log(form.nBitrate)
                this.$message({
                    message: 'FPS value must be between 1-between 30',
                    type: 'warning'
                });
                return false;
            }
            console.log("form1",nWidhei[0],nWidhei[1]);
            this.editPopup = false;
            var url =  this.$store.state.IPPORT + "/api/v1/AddTransProfile?name="+encodeURIComponent(form.strName)+
            "&token="+encodeURIComponent(form.strToken)+
            "&fpstype="+encodeURIComponent(form.nFPSType)+
            "&fps="+encodeURIComponent(form.nFPS)+
            "&scaletype="+encodeURIComponent(form.nScaleType)+
            "&width="+encodeURIComponent(nWidhei[0])+
            "&height="+encodeURIComponent(nWidhei[1])+
            "&bitrate="+encodeURIComponent(form.nBitrate)+
            "&engine="+encodeURIComponent(form.nEngine)+
             "&codec="+encodeURIComponent(form.nCodec)+
            "&session="+ this.$store.state.token;
            console.log(url)
            this.$http.get(url).then(result=>{
                if(result.status==200){
                  if(result.data.bStatus==true){
                        //this.reload();
                        this.tableData2=[];
                        this.loadstpro();
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
        platformyes(){
            var form=this.form;
            //return false;
            if(form.nBitrate<64||form.nBitrate>10240){
                console.log(form.nBitrate)
                this.$message({
                    message: 'Bitrate value must be between 64-10240',
                    type: 'warning'
                });
                return false;
            }else if(form.nFPS<1||form.nFPS>30){
                console.log(form.nBitrate)
                this.$message({
                    message: 'FPS value must be between 1-30',
                    type: 'warning'
                });
                return false;
            }
            var strs= []; //define an array
                strs=form.wihe.split("*");
            this.dialogFormVisible=false;
            var url =  this.$store.state.IPPORT + "/api/v1/AddTransProfile?name="+encodeURIComponent(form.strName)+
            "&token="+encodeURIComponent(form.strToken)+
            "&fpstype="+encodeURIComponent(form.nFPSType)+
            "&fps="+encodeURIComponent(form.nFPS)+
            "&scaletype="+encodeURIComponent(form.nScaleType)+
            "&width="+encodeURIComponent(strs[0])+
            "&height="+encodeURIComponent(strs[1])+
            "&bitrate="+encodeURIComponent(form.nBitrate)+
            "&engine="+encodeURIComponent(form.nEngine)+
            "&codec="+encodeURIComponent(form.nCodec)+
            "&session="+ this.$store.state.token;
            console.log(url)
            this.$http.get(url).then(result=>{
                if(result.status==200){
                  if(result.data.bStatus==true){
                        //this.reload();
                        this.tableData2=[];
                        this.loadstpro();
                    }else{
                        this.$message({
                            message: 'add failed',
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
        //Click to delete
        deleteRow(index, row,rows) {
            var index_xlh=((this.currentPage2-1)*10)+index;
            
            //url
            var url =  this.$store.state.IPPORT + "/api/v1/DelTransProfile?token="+encodeURIComponent(row.strToken)+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status==200){
                    if(result.data.bStatus==true){
                        rows.splice(index_xlh, 1);
                    }else{
                        this.$message({
                            message: 'Failed to delete',
                            type: 'warning'
                        });
                        return false;
                    }
                }
            })
        },
        //Select all delete
        deleteselect(){
            var selectop=this.selectop;
            console.log(selectop)
            // return false;
            //url
            for(var i=0;i<selectop.length;i++){
                var index=selectop[i].index;
                //return false;
                var url =  this.$store.state.IPPORT + "/api/v1/DelTransProfile?token="+encodeURIComponent(selectop[i].token)+"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    console.log(result);
                    console.log(this.tableData);
                    if(result.status==200){
                        if(result.data.bStatus==true){
                            // this.reload();
                            this.tableData2=[];
                            this.loadstpro();
                           //row.splice(index, 1);
                        }else{
                            this.$message({
                                message: 'Failed to delete',
                                type: 'warning'
                            });
                            return false;
                        }
                    }
                })
            }
            
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
        handleSizeChange2(val) {
            console.log(`Per page ${val} strip`);
            this.currentPage2 = 1;
            this.pageSize = val;
        },
        handleCurrentChange2(val) {
            console.log(`Current page: ${val}`);
            this.currentPage2 = val;
        }
    },
  };
</script>