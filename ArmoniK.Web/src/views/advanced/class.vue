<template>
    <div class="aiclass"> 
        <div class="aiclass_left">
            <div class="aiclass_left_top">
                <span style="padding-left:10px;padding-right:150px; "> 标注库</span>
                <span class="iconfont icon-shuaxin" @click="refresh" style="cursor:pointer;"> </span>
                <span class="iconfont icon-liebiao" style="padding-left:10px;"> </span>
            </div>
            <div class="aiclass_left_search">
                <el-input
                    class="aiclass_left_input"
                    placeholder="快速查找"
                    v-model="filterText">
                    <span slot="suffix" style="line-height:35px;" ><i style="font-size:18px;">|</i>&nbsp;<i class="iconfont icon-jia" style="cursor:pointer" @click="qadd()"></i></span>
                </el-input>
            </div>
            <el-tree
                :data="data"
                node-key="id"
                ref="tree"
                :filter-node-method="filterNode"
                @node-click="handleNodeClick"
                :props="defaultProps"
            >
            <span slot-scope="{ data }">
              <i :class="data.classname" style="font-size:14px;"></i>
              <span style="padding-left: 4px;">{{data.name}}</span>
              <el-popover placement="bottom" trigger="manual" v-if="data.classname == 'iconfont icon-ku1'" class="Status" :popper-class="$store.state.darkMode==false?'bai':$store.state.darkMode" style="flex: 1; text-align: right;height: 36px;line-height: 36px;" :ref="'popover-'+data.name">
                <div class="StatusPrompt" >
                    <el-input
                    class="aiclass_left_input"
                    size="mini"
                    v-model="classname">
                    </el-input>
                    <el-button type="primary" size="mini" @click="AddClass(data.name)">创建</el-button>
                </div>
                <el-button  slot="reference" type="text"  class="iconfont icon-jia2 statusBtn" style="font-size:14px;position:absolute;right:25px;" @click="StatusPrompt(data.name,$event)"></el-button>
                <el-button  slot="reference" type="text"  class="iconfont icon-shanchu statusBtn" style="font-size:14px;position:absolute;right:5px;;" @click="cancel1(data.name,$event)"></el-button>
              </el-popover>
              <el-popover placement="bottom" trigger="manual" v-else class="Status" popper-class="popperOptions" style="flex: 1; text-align: right;height: 36px;line-height: 36px;" :ref="'popover-'">
                <el-button  slot="reference" type="text"  class="iconfont icon-shanchu statusBtn" style="font-size:14px;position:absolute;right:5px;" @click="cancel(data.name,data.repository)"></el-button>
              </el-popover>
            </span>
          </el-tree>
        </div>
        <div class="aiclass_right">
            <div class="aiclass_right_title" v-if="aiclassku">
                <div class="aiclass_right_ku" >
                    <h3 class='title'>创建库</h3> <br>
                    <div>
                        名称
                    </div>
                    <el-input v-model="repository"></el-input>
                </div>
                <div class="LowerPart">
                    <el-button  class="saveBtn" @click="stop">取消</el-button>
                    <el-button class="save" @click="save"> 保存</el-button>
                </div>
            </div>
            <div class="aiclass_right_title" v-if="remove">
                <div class="aiclass_right_ku" >
                    <h3 class='title'>确定要删除吗？</h3> <br>
                    <div>
                        请输入名称
                    </div>
                    <el-input v-model="inputclass"></el-input>
                </div>
                <div class="LowerPart">
                    <el-button  class="saveBtn" @click="stop">取消</el-button>
                    <el-button class="save" v-if="deleteRepository" @click="DeleteRepository"> 确定</el-button>
                    <el-button class="save" v-else @click="ensure"> 确定</el-button>
                </div>
            </div>
        </div>
    </div>
</template>
<script>
//操作信息
export default {
    data(){
        return{
            filterText:"",
            data: [],
            defaultProps: {
                children: "children",
                name: "label",
            },
            timer:undefined,
            aiclassku:false,
            repository:'',//库文件名
            classname:'',
            inputclass:'',
            remove:false,
            deleteRepository:true,
            removename:'',
            removerepository:''
        }
    },
    methods:{
        stop(){
            this.aiclassku=false;
            this.remove=false;
            this.repository='';
            this.inputclass='';
        },
        save(){
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v2/label/AddRepository?name="+this.repository+"&session="+ this.$store.state.token;
            if (this.repository) {
                this.$http.get(url).then(resule=>{
                    console.log(resule);
                    this.data = [];
                    this.catalogue();
                })
            }else{
                this.$message('名称不能为空');
            }
        },
        ensure(){
            // /api/v2/label/DeleteClass
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v2/label/DeleteClass?repository="+this.repository+"&class="+this.inputclass+"&session="+ this.$store.state.token;
            console.log(url);
            if (this.removename == this.inputclass) {
                this.$http.get(url).then(resule=>{
                    console.log(resule);
                    this.data = [];
                    this.stop();
                    this.catalogue();
                })
            }else{
                this.$message('名称不一致');
            }
        },
        refresh(){
            if(this.timer) {
                clearTimeout(this.timer)
                this.timer = null
            }
            this.data = [];
            this.timer = setTimeout(()=>{
                this.catalogue();
            },500)
        },
        qadd(){
            this.aiclassku=true;
            this.remove=false;
        },
        cancel1(){
            this.remove=true;
            this.aiclassku=false;
            this.deleteRepository = true;
        },
        DeleteRepository(){
            // DeleteRepository
            // /api/v2/label/DeleteClass
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v2/label/DeleteRepository?repository="+this.inputclass+"&session="+ this.$store.state.token;
            console.log(url);
            if (this.removerepository == this.inputclass) {
                this.$http.get(url).then(resule=>{
                    console.log(resule);
                    this.data = [];
                    this.stop();
                    this.catalogue();
                })
            }else{
                this.$message('名称不一致');
            }
        },
        AddClass(repository){
            if (this.classname) {
                // /api/v2/label/AddClass
                var root = this.$store.state.IPPORT;
                var url = root + "/api/v2/label/AddClass?repository="+repository+"&class="+this.classname+"&session="+ this.$store.state.token;
                this.$http.get(url).then(result=>{
                    console.log(result);
                    if (result.status == 200) {
                        this.data = [];
                        this.catalogue();
                    }
                })
            }else{
                this.$message('名称不能为空');
            }
        },
        StatusPrompt(id,e){
            let refName = 'popover-' + id;
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
        cancel(data,repository){
            this.remove=true;
            this.aiclassku=false;
            this.deleteRepository = false;
            // this.removename=data;
            // this.repository = repository
        },
        catalogue(){
            // /api/v2/label/GetRepository
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v2/label/GetRepository?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if (result.status == 200) {
                    for (let i = 0; i < result.data.length; i++) {
                        this.getclass(result.data[i])
                    }
                }
            })
        },
        getclass(repository){
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v2/label/GetClass?repository="+repository+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if (result.data.bStatus==false) {
                    let data = {
                        name : repository,
                        classname : 'iconfont icon-ku1',
                        children:[]
                    }
                    this.data.push(data);
                }else{
                    let children = []
                    for (let i = 0; i < result.data.length; i++) {
                        let namechild = {
                            name : result.data[i],
                            repository:repository,
                            classname : 'iconfont icon-kuyuansu',
                        }
                        children.push(namechild)
                    }
                    let data = {
                        name : repository,
                        classname : 'iconfont icon-ku1',
                        children:children
                    }
                    this.data.push(data);
                }
            })
        },
        handleNodeClick(data,e){
            console.log(data);
            if (data.repository) {
                this.removename=data.name;
                this.repository = data.repository
            }else{
                this.removerepository = data.name
            }
        },
        //模糊查询
		filterNode(value, data) {
			if (!value) return true;
			var data1= data.name.indexOf(value) !== -1;
			return data.name.indexOf(value) !== -1;
		},
	},
	mounted(){
        this.catalogue();
	},
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
}
</script>
<style lang="scss" scoped>
.aiclass{
    display: flex;
    height: 100%;
    .aiclass_left{
        height: 100%;
        width: 260px;
    }
    .aiclass_left_top{
        width: 260px;
        height: 51px;
        line-height: 51px;   
        font-size: 14px;
        font-family: Adobe Heiti Std;
        font-weight: normal;
    }
    .aiclass_left_search{
        padding: 10px;
    }
    .aiclass_right{
        padding: 20px;
        .aiclass_right_title{
            width: 556px;
            height: 403px;
            padding: 30px;
        }
        .LowerPart{
            padding-left: 348px;
            padding-top: 100px;
            .saveBtn{
                    font-size: 14px;
                    padding: 6px 20px;
                    background: none;
                    border: 1px solid rgb(58, 188, 254);
                }
                .save{
                    font-size: 14px;
                    padding: 7px 20px;
                    background: rgb(58, 188, 254);
                    border: 0px none;
                }
        }
    }
    .statusBtn:hover{
        color: #409eff;
    }
    .el-tree-node__content{
        padding:2px 0;
    }
}
</style>