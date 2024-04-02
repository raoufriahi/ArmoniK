<template>
    <div class="ai_redio">
        <div class="ai_redio_left">
            <div class="ai_redio_left_right">
                <canvas id="aiclass_canvas" width="900" height="500"></canvas>
                <div id="bzelect" style="position:absolute;display:none;">
                    <el-select v-model="value1" placeholder="请选择" @change="bzclass">
                        <el-option
                        v-for="item in options1"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                        </el-option>
                    </el-select>
                </div>
                <div id="btn" style="width:900px;background:#202020;padding:10px; display:none;">
                    <el-button style="margin:0 10px;" id="bianji" type="success" icon="iconfont icon-biaozhu1" size="mini" circle @click="edit()"></el-button>
                    <el-button style="margin:0 10px;" type="danger" icon="el-icon-delete" size="mini" circle @click="clears"></el-button>
                    <el-button style="margin:0 10px;" type="success" icon="el-icon-back" size="mini" circle @click="last"></el-button>
                    <el-button style="margin:0 10px;" type="success" icon="el-icon-right" size="mini" circle @click="next"></el-button>
                    <el-button type="primary" style="float:right;" size="mini" @click="send()"> 发送</el-button>
                </div>
            </div>
            <div @click="picture" class="picture">
                <i class="el-icon-picture"></i>
            </div>
        </div>
        <div class="ai_redio_right">
            <div id="tree">
                <div style="background: #2D2D2D;padding:8px 0 8px 22px;">标注库</div>
                <el-tree
                    :data="data"
                    node-key="id"
                    ref="tree"
                    @node-click="handleNodeClick"
                    :props="defaultProps">
                    <span slot-scope="{ data }">
                        <i class="iconfont icon-ku1"></i>
                        <span style="padding-left: 4px;">{{data.label}}</span>
                    </span>
                </el-tree>
            </div>
            <div style="background: #2D2D2D;padding:8px 0 8px 20px;">图片库</div>
            <div id="img">
                <div style="height:50vh;">
                    <div style="cursor:pointer;padding-left:10px;" class="img_list" v-for="(data,i) in options.slice((currentPage - 1) * pageSize, currentPage * pageSize)" :key="i" :id="'img'+i" @click="dianji(data,$event.target,i)">{{data.name}}</div>
                </div>
                <hr style="margin-bottom:0;">
                <div style="position: relative;">
                    <span class="pagspan">{{pageSize}}个/页</span>
                    <el-pagination
                    style="text-align: center"
                    layout=" prev, next,total,jumper"
                    @size-change="handleSizeChange"
                    @current-change="handleCurrentChange"
                    :current-page="currentPage"
                    :page-size="pageSize"
                    :total="total"
                    >
                    <!-- :pager-count="1" -->
                    </el-pagination>
                </div>
            </div>
        </div>
    </div>
</template>
<script>
import DataClass from '../public/class';
export default {
    data(){
        return{         
            currentPage: 1, // 当前页码
            total: 0, // 总条数
            pageSize: 20, //一页数量
            data:[],
            options1: [],
            options: [],
            defaultProps: {     
                children: "children",
                label: "label",
            },
            dataclass:null,
            value1:'',
            xmlurl:'',
            width:'',
            height:'',
            index:0,
            start:true
        }
    },
    mounted(){
        this.class();
    },
    methods:{
        picture(){
            if (this.start) {
                $(".ai_redio_left").css("width","100%");
                $(".ai_redio_right").css("width","0");
                $(".ai_redio_right").css("display","none");
                $(".picture").css("background","#409EFF");
                this.start = !this.start;
            }else{
                $(".ai_redio_left").css("width","80%");
                $(".ai_redio_right").css("width","20%");
                $(".ai_redio_right").css("display","block");
                $(".picture").css("background","#353535");
                this.start = !this.start;
            }
        },
        last(){
            if (this.index>0) {
                let i = this.index-1;
                let e = $(".img_list").get(i);
                let data = this.options[i+(this.currentPage-1)*this.pageSize];
                this.dianji(data,e,i);
            }
        },

        next(){
            console.log(this.index);
            if (this.index < this.pageSize-1&&this.index < this.total-(this.currentPage-1)*this.pageSize-1) {
                let i = this.index+1;
                let e = $(".img_list").get(i);
                let data = this.options[i+(this.currentPage-1)*this.pageSize];
                this.dianji(data,e,i);
            }
        },
        class(){
            var url = this.$store.state.IPPORT + "/api/v2/label/GetRepository?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
            //   return false;
              if(result.status == 200){
                result.data.forEach(item => {
                    this.data.push({
                        value:item,
                        label:item
                    })
                });
             }
            }).catch((err)=>{
                console.log(err);
            })
        },
        handleNodeClick(data){
            $("#aiclass_img").attr("src",'');
            $("#aiclass_img").width('0')
            $("#aiclass_img").height('0')
            var url = this.$store.state.IPPORT + "/api/v2/label/GetClassFlies?repository="+data.label+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                this.options=[]
                if(result.status == 200){
                    if (result.data.bStatus!=false) {
                        this.total = result.data.length;
                        this.options=result.data;
                        this.$nextTick(()=>{
                            this.dianji(this.options[0],$(".img_list").get(0),0)
                        })
                    }
                }
            }).catch((err)=>{
                console.log(err);
            })
        },
        dianji(data,e,i){
            this.index = i
            console.log(this.index);
            $('#bzelect').hide();
            let url = this.$store.state.IPPORT+'/'+data.url.split("\\").join("/")+"?session="+ this.$store.state.token;
            let img = new Image();
            let ctx;
            img.src = url;
            img.onload = function () {
                let canvas = $("#aiclass_canvas").get(0)
                ctx = canvas.getContext('2d');
                ctx.clearRect(0, 0, 900, 500);
                ctx.drawImage(img, 0, 0, 900, 500);
            }
            $("#btn").css("display","block")
            $(".img_list").css("color","#FFF")
            $(".img_list").css("background","#202020")
            e.style.color = "red";
            e.style.background = "black";
            this.xmlurl = data.xmlurl;
            let xmlurl = this.$store.state.IPPORT + "/api/v2/label/GetClassXml?url="+data.xmlurl+"&session="+ this.$store.state.token;
            this.$http.get(xmlurl).then(result=>{
                let layers =[];
                let width = result.data.width;
                let height = result.data.height;
                this.width = result.data.width;
                this.height = result.data.height;
                result.data.object.forEach((item)=>{
                    let object ={};
                    object.fillStyle = '';
                    object.height=item.bndbox.height*500/height;
                    object.strokeStyle="red";
                    object.type=item.repository;
                    object.width=item.bndbox.width*900/width;
                    object.x1=item.bndbox.xmin*900/width;
                    object.x2=item.bndbox.xmax*900/width;
                    object.y1=item.bndbox.ymin*500/height;
                    object.y2=item.bndbox.ymax*500/height;
                    layers.push(object)
                })
                let conf = {
                    width:result.data.width,
                    height:result.data.height,
                    img : img,
                    bzelect : 'bzelect',
                    canvasid : 'aiclass_canvas',
                    value : result.data.depth,
                    layers: layers,
                    mover:true,
                    getclass:this.getclass,
                    reshow:this.reshow,
                }
                setTimeout(()=>{
                    let Dataclass = new DataClass(conf);
                    this.dataclass = Dataclass;
                },100)
                // this.canvasfile(ctx,result.data)
                this.getclass(result.data.depth)
            }).catch((err)=>{
                console.log(err);
            })
        },
        edit(){
            if (this.dataclass) {
                this.dataclass.edit();
            }
        },
        clears(){
            if (this.dataclass) {
                $('#bzelect').hide();
                this.dataclass.clears();
            }
        },
        reshow(item){
            $('#bzelect').show();
            $('#bzelect').css({top:item.y2+'px',left:item.x2+"px"});
            this.value1 = item.type;
        },
        getclass(repository,y,x){
            if (y) {
                $('#bzelect').show();
                $('#bzelect').css({top:y+'px',left:x+"px"});
            }
            this.options1=[];
            var root = this.$store.state.IPPORT;
            var url = root + "/api/v2/label/GetClass?repository="+repository+"&session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                if(result.status == 200){
                    result.data.forEach(item => {
                        this.options1.push({
                            value:item,
                            label:item
                        })
                    });
                }
            })
        },
        bzclass(){
            if (this.dataclass) {
                this.dataclass.bzclass(this.value1)
            }
        },
        canvasfile(ctx,data){
            let width = data.width
            let height = data.height
            data.object.forEach(item => {
                let data = item.bndbox;
                ctx.beginPath();
                ctx.rect(data.xmin*900/width, data.ymin*500/height, data.width*900/width, data.height*500/height);
                ctx.strokeStyle = "red";
                ctx.stroke();
            });
        },
        send(){
            if (this.xmlurl!='') {
                $('#bzelect').hide();
                let layersclass = this.dataclass.layers;
                let layers = []
                layersclass.forEach((item,i) => {
                    let data = {
                        width:item.width*this.width/900,
                        height:item.height*this.height/500,
                        type:item.type,
                        x1:item.x1*this.width/900,
                        y1:item.y1*this.height/500,
                        x2:item.x2*this.width/900,
                        y2:item.y2*this.height/500
                    }
                    layers.push(data)
                })
                let data = {
                    url:this.xmlurl,
                    layers:layers
                }
                var url = this.$store.state.IPPORT + "/api/v2/UpdataLabelImg?session="+ this.$store.state.token;
                this.$http.post(url,JSON.stringify(data),{headers: {'Content-Type': 'application/json'}}).then(result=>{
                    if (result.status === 200) {
                        this.$message({
                            message: "发送成功",
                            type: 'success'
                        });
                    }
                })
            }
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
            // let i = 0 + (val-1)*this.pageSize;
            // this.index = 0;
            // let e = $(".img_list").get(0);
            // let data = this.options[i];
            // this.dianji(data,e,i)
        },
    }
}
</script>
<style lang="scss" scoped>
    .ai_redio{
        display: flex;
        height: 100%;
        .ai_redio_left{
            width: 75%;
            display: flex;
            height: 100%;
            .ai_redio_left_right{
                padding: 10px;
                width: 100%;
                position: relative;
            }
            .picture{
                padding:10px 14px;
                background:#353535;
                height:40px;
                cursor:pointer;
                border-radius: 4px 0px 0px 4px;
            }
        }
        .ai_redio_right{
            width: 25%;
            background: #202020;
        }
        #tree{
            height: 30%;
            font-size:16px;
        }
        .img_list{
            width: 400px;
        }
        #bianji{
            padding: 5px;
        }
        #img{
            height: 65%;
            font-size:16px;
            padding: 10px 0;
        }
        #img,#tree{
            border-bottom: none;
            overflow: auto;
            &::-webkit-scrollbar {/*滚动条整体样式*/
                width: 8px;     /*高宽分别对应横竖滚动条的尺寸*/
                height: 8px;
                scrollbar-arrow-color:red;
            }
            &::-webkit-scrollbar-thumb {/*滚动条里面小方块*/
                border-radius: 5px;
                -webkit-box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                background: rgba(218, 218, 218,0.2);
                scrollbar-arrow-color:red;
            }
            &::-webkit-scrollbar-track {/*滚动条里面轨道*/
                -webkit-box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                box-shadow: inset 0 0 5px rgba(218, 218, 218,0.2);
                border-radius: 0;
                background: rgba(218, 218, 218,0.1);
            }
        }
        .el-pagination{
            position: absolute;
            top:10px;
            left: 0;
        }
        .pagspan{
            position: absolute;
            top:16px;
            right: 15px;
            color: #606266;
            font-size: 14px;
        }
    }
</style>