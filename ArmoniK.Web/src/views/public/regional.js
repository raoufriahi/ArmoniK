import Vue from 'vue'
import axios from 'axios'
import store from '../../store'
import VueI18n from 'vue-i18n'
import LangEn from '../../../static/lang/en'
import LangZhCHS from '../../../static/lang/zhchs'
import LangZhCHT from '../../../static/lang/zhcht'
Vue.use(VueI18n)
const i18n = new VueI18n({
    locale: 'en',
      fallbackLocale: 'en',
      localeDir: 'locales',
      enableInSFC: true,
    messages: {
      'en': LangEn,
    }
})
i18n.locale = store.state.lang

let timer = null;
let root=store.state.IPPORT;
//测试机仓
var listdatag=[];//全部
var listdatagload=[];//部分
var listdatag1=[];//一个
var listdataChilde=[];//有主辅码流节点
function loadtest(){
    if(store.state.root=="Operator"){
        return false
    }
    var url = root + "/api/v1//GetSrcCamera?session="+ store.state.token;
    // return falsel;
    axios.get(url).then(result=>{
        if(result.status == 200){
            var data =  result.data;
            var srcGroup = {children: []};
            var srcGroup2 = {children: []};
            srcGroup.label=i18n.tc('message.live.camera');
            srcGroup.iconclass="iconfont  icon-shexiangji";
            srcGroup.length=data.src.length;
            var number = 0;
            var numberbDisable = 0;
            var offlineNumber = 0;
            var disableNumber = 0;
            var IdleNumber =0;
            for(var i=0; i< data.src.length; i++){
                var item = data.src[i];
                if (item.bOnline&&!item.bDisable) {
                    number=number+1;
                }
                if(item['nOriginalType'] == 'H5_CH_GB'){
                    numberbDisable=numberbDisable+1;
                    continue;
                }else{
                    var newItem ={
                        token : item['strToken'],
                        label : item['strName'],
                        iconclass : 'iconfont  icon-shexiangji',
                        iconclass1 : '',
                        iconclass2 : 'iconfont  icon-shexiangtou',
                        name:item['strName']+"--"+i18n.tc('message.live.mainstream'),
                        disabled_me:false,
                        ChannelNumber: item["nChannelNumber"],
                        nType: item["nType"],
                        bOnline: item["bOnline"],
                    };
                    if(!item['bOnline']){
                        // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                        newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                        newItem['iconclass1'] = 'el-tree-camera';
                        offlineNumber=offlineNumber+1;
                    }
                    if(item['nType'] == 'H5_CLOUD')
                        newItem['iconclass'] = 'mdi mdi-cloud-upload fa-fw';
                    
                    if(item['bRec'] == true)
                        // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                        newItem['iconclass2'] = 'none';

                    if(item['bDisable'] == true){
                        newItem['disabled_me'] =true;
                        // newItem['iconclass1'] = 'camera';
                        newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                        newItem['iconclass1'] = 'el-tree-camera';
                        newItem['iconclass3'] = 'heightlight';
                    }
                    if(item['bDisable'] == true&&store.state.devicemarktoggle=='none'){
                        numberbDisable = numberbDisable+1;
                        continue;
                    }
                    if(item['bDisable'] == true){
                        if (store.state.devicemarktoggle==undefined||store.state.devicemarktoggle=='block') {
                            disableNumber=disableNumber+1;
                        }
                    }
                    if (item['bIdle'] == true) {
                        newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                        newItem['iconclass1'] = 'el-tree-camera';
                        IdleNumber=IdleNumber+1;
                    }
                    if(!item['bOnline']){
                        srcGroup.children.push(newItem);
                    }else{
                        srcGroup.children.unshift(newItem);
                    }
                    // srcGroup.children.push(newItem);
                }
            }
            srcGroup.firstLevel=true,
            srcGroup.length=data.src.length-numberbDisable;
            srcGroup.online=number;
            // srcGroup.offline=srcGroup.length-number;
            srcGroup.offline=offlineNumber;
            srcGroup.disable=disableNumber;
            srcGroup.idle = IdleNumber;
            srcGroup.childrenShow=false;
            srcGroup.statusID=srcGroup.label,
            srcGroup2 = JSON.parse(JSON.stringify(srcGroup))
            listdatag.push(srcGroup);
            for(var i=0; i< srcGroup2.children.length; i++){
                // 主副流
                var node=[{
                    token : srcGroup2.children[i].token,
                    streamprofile : "main",
                    label :i18n.tc('message.live.mainstream'),
                    name:srcGroup2.children[i].label+"--"+i18n.tc('message.live.mainstream'),
                    iconclass : 'mdi mdi-playlist-play fa-fw',
                    disabled_me:false
                    },{
                    token : srcGroup2.children[i].token,
                    streamprofile : "sub",
                    label :i18n.tc('message.live.substream'),
                    name:srcGroup2.children[i].label+"--"+i18n.tc('message.live.substream'),
                    iconclass : 'mdi mdi-playlist-play fa-fw',
                    disabled_me:false
                }]
                srcGroup2.children[i].children=node;
            }
            listdataChilde.push(srcGroup2);
        } 
    })

}
// loadtest();
//写作业
function loadDevice() {
    if(store.state.root=="Operator"){
        return false
    }
    var url = root + "/api/v1/GetDevice?session="+ store.state.token;
    //重组
    axios.get(url).then(result=>{
        if(result.status == 200){
            // var srcData = [];
            var data=result.data.dev;
            if(Array.isArray(data)){
            data.sort((a,b)=>{
                if(a.strName===b.strName) return 0;
                return a.strName > b.strName? 1:-1 
            })
            for(var i = 0; i < data.length; i++){
                var item=data[i];
                // var srclevel=[];
                // srclevel["strToken"]=item.strToken;
                // srclevel["strName"]=item.strName;
                listdatag.push({
                    token:item.strToken,
                    label:item.strName,
                    length:'',
                    online:0,
                    offline:0,
                    iconclass:"iconfont icon-shexiangji",
                    children:[],
                    firstLevel:true,
                    statusID:data[i].strToken,
                })
                listdatagload.push({
                    token:item.strToken,
                    label:item.strName,
                    length:'',
                    online:0,
                    offline:0,
                    iconclass:"iconfont icon-shexiangji",
                    children:[],
                    firstLevel:true,
                    statusID:data[i].strToken,
                })
                listdataChilde.push({
                    token:item.strToken,
                    label:item.strName,
                    length:'',
                    online:0,
                    offline:0,
                    iconclass:"iconfont icon-shexiangji",
                    children:[],
                    firstLevel:true,
                    statusID:data[i].strToken,
                })
                loadSrc(item.strToken);
              }
            }
        }
    })
}
function loadSrc(strToken) {
    let _this =this;
    var url = root + "/api/v1/GetDeviceSrc?token="+ strToken + "&session=" + store.state.token;
    axios.get(url).then(result => {
        if (result.status == 200){
            var number = 0;
            var numberbDisable = 0;
            var offlineNumber = 0;
            var disableNumber = 0;
            var IdleNumber =0;
            var data =  result.data;
            var srcGroup = [];
            var srcGroup2 = [];
            // srcGroup.label=srclevel.strName;
            // srcGroup.iconclass="iconfont  icon-kaiqishexiangtou1";
            for(var i=0; i< data.src.length; i++){
                var item = data.src[i];
                if (item.bOnline&&!item.bDisable) {
                    number=number+1;
                }
                var newItem ={
                    token : item['strToken'],
                    label : item['strName'],
                    iconclass : 'iconfont  icon-shexiangji',
                    iconclass1 : '',
                    name:item['strName']+"--"+i18n.tc('message.live.mainstream'),
                    disabled_me:false,
                    ChannelNumber: item["nChannelNumber"],
                    nType: item["nType"],
                    bOnline: item["bOnline"]
                };

                if(!item['bOnline']){
                    // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                    newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                }
                if(!item['bOnline']&&item['bIdle'] == false){ 
                    offlineNumber=offlineNumber+1;
                }
                if(item['nType'] == 'H5_CLOUD')
                    newItem['iconclass'] = 'mdi mdi-cloud-upload fa-fw';

                if(item['bRec'] == true)
                    // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                    newItem['iconclass2'] = 'none';

                if(item['bDisable'] == true){
                    newItem['disabled_me'] =true;
                    // newItem['iconclass1'] = 'camera';
                    newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                    newItem['iconclass3'] = 'heightlight';
                }
                if(item['bDisable'] == true&&store.state.devicemarktoggle=='none'){
                    numberbDisable = numberbDisable+1;
                    continue;
                }
                if(item['bDisable'] == true){
                    if (store.state.devicemarktoggle==undefined||store.state.devicemarktoggle=='block') {
                        disableNumber=disableNumber+1;
                    }
                }
                if (item['bIdle'] == true) {
                    newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                    IdleNumber=IdleNumber+1;
                }
                //    srcGroup.children.push(newItem);
                if(!item['bOnline']||item.bDisable){
                    srcGroup.push(newItem);
                }else{
                    srcGroup.unshift(newItem);
                }
            }
            var srcData = listdatag.find(item => item.token === strToken);
            if(srcData){
                srcData.children=srcGroup;
                srcData.length=result.data.src.length-numberbDisable;
                srcData.online=number;
                // srcData.offline=srcData.length-srcData.online;
                srcData.offline=offlineNumber;
                srcData.disable=disableNumber;
                srcData.idle = IdleNumber;
            }
            var srcData1 = listdatagload.find(item => item.token === strToken);
            if(srcData1){
                srcData1.children=srcGroup;
                srcData1.length=result.data.src.length-numberbDisable;
                srcData1.online=number;
                // srcData1.offline=srcData1.length-srcData1.online;
                srcData1.offline=offlineNumber;
                srcData1.disable=disableNumber;
                srcData1.idle = IdleNumber;
            }
            var srcData2 = listdataChilde.find(item => item.token === strToken);
            if(srcData2){
                srcGroup2 = JSON.parse(JSON.stringify(srcGroup))
                srcData2.children=srcGroup2;
                srcData2.length=result.data.src.length-numberbDisable;
                srcData2.online=number;
                // srcData2.offline=srcData2.length-srcData2.online;
                srcData2.offline=offlineNumber;
                srcData2.disable=disableNumber;
                srcData2.idle = IdleNumber;
                for(var i=0; i< srcData2.children.length; i++){
                    // 主副流
                    var node=[{
                        token : srcData2.children[i].token,
                        streamprofile : "main",
                        label :i18n.tc('message.live.mainstream'),
                        name:srcData2.children[i].label+"--"+i18n.tc('message.live.mainstream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        },{
                        token : srcData2.children[i].token,
                        streamprofile : "sub",
                        label :i18n.tc('message.live.substream'),
                        name:srcData2.children[i].label+"--"+i18n.tc('message.live.substream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                    }]
                    srcData2.children[i].children=node;
                }
            }
            // listdatag.push(srcGroup);
            // listdatagload.push(srcGroup);
        }
    }).catch(error => {
        console.log('GetSrc failed', error);
    });
}
// loadDevice();
//数字仓机
function NumberDevice() {
    if(store.state.root=="Operator"){
        return false
    }
   //url
   var url = root + "/api/v1/GetGbDevice?session="+ store.state.token;
    //重组
    axios.get(url).then(result=>{
        if(result.status == 200){
            // var srcData = [];
            var data=result.data.dev;
            if(Array.isArray(data)){
                data.sort((a,b)=>{
                    if(a.strName===b.strName) return 0;
                    return a.strName > b.strName? 1:-1 
                })
                for(var i = 0; i < data.length; i++){
                    var item=data[i];
                    // var srclevel=[];
                    //   srclevel["strToken"]=item.strToken;
                    //   srclevel["strName"]=item.strName;
                    listdatag.push({
                        token:item.strToken,
                        label:item.strName,
                        iconclass:"iconfont icon-shexiangji",
                        children:[],
                        firstLevel:true,
                        length:'',
                        online:0,
                        offline:0,
                        statusID:data[i].strToken,
                    })
                    listdatagload.push({
                        token:item.strToken,
                        label:item.strName,
                        iconclass:"iconfont icon-shexiangji",
                        children:[],
                        firstLevel:true,
                        length:'',
                        online:0,
                        offline:0,
                        statusID:data[i].strToken,
                    })
                    listdataChilde.push({
                        token:item.strToken,
                        label:item.strName,
                        length:'',
                        online:0,
                        offline:0,
                        iconclass:"iconfont icon-shexiangji",
                        children:[],
                        firstLevel:true,
                        statusID:data[i].strToken,
                    })
                    NumberSrc(item.strToken);
                }
            }
        }
    })
}
// NumberDevice();
function NumberSrc(strToken) {
    let _this =this;
    var url = root + "/api/v1/GetGbDeviceSrc?token="+ strToken + "&session=" + store.state.token;
    axios.get(url).then(result => {
        if (result.status == 200){
            var number = 0;
            var numberbDisable = 0;
            var offlineNumber = 0;
            var disableNumber = 0;
            var IdleNumber =0;
            var data =  result.data;
            var srcGroup = [];
            var srcGroup2 = [];
            // srcGroup.label=srclevel.strName;
            // srcGroup.iconclass="iconfont  icon-kaiqishexiangtou1";
            for(var i=0; i< data.src.length; i++){
                var item = data.src[i];
                if (item.bOnline&&!item.bDisable) {
                    number=number+1;
                }
                var newItem ={
                    token : item['strToken'],
                    label : item['strName'],
                    iconclass : 'iconfont  icon-shexiangji',
                    iconclass1:'',
                    name:item['strName']+"--"+i18n.tc('message.live.mainstream'),
                    disabled_me:false,
                    ChannelNumber: item["nChannelNumber"],
                    nType: item["nType"],
                    bOnline: item["bOnline"],
                };
                if(!item['bOnline']){
                    // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                    newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                }
                if(!item['bOnline']&&item['bIdle'] == false){ 
                    offlineNumber=offlineNumber+1;
                }
                if(item['nType'] == 'H5_CLOUD')
                    newItem['iconclass'] = 'mdi mdi-cloud-upload fa-fw';

                if(item['bRec'] == true)
                    // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                    newItem['iconclass2'] = 'none';
                        
                if(item['bDisable'] == true){
                    newItem['disabled_me'] =true;
                    // newItem['iconclass1'] = 'camera';
                    newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                    newItem['iconclass3'] = 'heightlight';
                }
                if(item['bDisable'] == true){
                    if (store.state.devicemarktoggle==undefined||store.state.devicemarktoggle=='block') {
                        disableNumber=disableNumber+1;
                    }
                }
                 
                 if(item['bDisable'] == true&&store.state.devicemarktoggle=='none'){
                     numberbDisable = numberbDisable+1;
                     continue;
                 }
                if (item['bIdle'] == true) {
                    newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                    IdleNumber=IdleNumber+1;
                }
                srcGroup.push(newItem);
            }
            var srcData = listdatag.find(item => item.token === strToken)
            // console.log(srcData)
            if(srcData){
                srcData.children=srcGroup
                srcData.length=result.data.src.length-numberbDisable;
                srcData.online=number;
                // srcData.offline=srcData.length-srcData.online;
                srcData.offline=offlineNumber;
                srcData.disable=disableNumber;
                srcData.idle = IdleNumber;
            }
            var srcData1 = listdatagload.find(item => item.token === strToken)
            if(srcData1){
                srcData1.children=srcGroup
                srcData1.length=result.data.src.length-numberbDisable;
                srcData1.online=number;
                // srcData.offline=srcData.length-srcData.online;
                srcData1.offline=offlineNumber;
                srcData1.disable=disableNumber;
                srcData1.idle = IdleNumber;
            }
            var srcData2 = listdataChilde.find(item => item.token === strToken);
            if(srcData2){
                srcGroup2 = JSON.parse(JSON.stringify(srcGroup))
                srcData2.children=srcGroup2;
                srcData2.length=result.data.src.length-numberbDisable;
                srcData2.online=number;
                // srcData2.offline=srcData2.length-srcData2.online;
                srcData2.offline=offlineNumber;
                srcData2.disable=disableNumber;
                srcData2.idle = IdleNumber;
                for(var i=0; i< srcData2.children.length; i++){
                    // 主副流
                    var node=[{
                        token : srcData2.children[i].token,
                        streamprofile : "main",
                        label :i18n.tc('message.live.mainstream'),
                        name:srcData2.children[i].label+"--"+i18n.tc('message.live.mainstream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        },{
                        token : srcData2.children[i].token,
                        streamprofile : "sub",
                        label :i18n.tc('message.live.substream'),
                        name:srcData2.children[i].label+"--"+i18n.tc('message.live.substream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                    }]
                    srcData2.children[i].children=node;
                }
            }
            // listdatag.push(srcGroup);;
            // listdatagload.push(srcGroup);
        }
    }).catch(error => {
        console.log('GetSrc failed', error);
    });
}
//级联
function cloudDevice() {
    if(store.state.root=="Operator"){
        return false
    }
    var url = root + "/api/v1/GetCloudDevice?session="+ store.state.token;
    //重组
    axios.get(url).then(result=>{
        if(result.status == 200){
            // var srcData = [];
            var data=result.data.dev;
            if(Array.isArray(data)){
                data.sort((a,b)=>{
                    if(a.strName===b.strName) return 0;
                    return a.strName > b.strName? 1:-1 
                })
                for(var i = 0; i < data.length; i++){
                    var item=data[i];
                    // var srclevel=[];
                    //   srclevel["strToken"]=item.strToken;
                    //   srclevel["strName"]=item.strName;
                    listdatag.push({
                        token:item.strToken,
                        label:item.strName,
                        iconclass:"iconfont icon-shexiangji",
                        children:[],
                        firstLevel:true,
                        length:'',
                        online:0,
                        offline:0,
                        statusID:data[i].strToken,
                    })
                    listdatagload.push({
                        token:item.strToken,
                        label:item.strName,
                        iconclass:"iconfont icon-shexiangji",
                        children:[],
                        firstLevel:true,
                        length:'',
                        online:0,
                        offline:0,
                        statusID:data[i].strToken,
                    })
                    listdataChilde.push({
                        token:item.strToken,
                        label:item.strName,
                        length:'',
                        online:0,
                        offline:0,
                        iconclass:"iconfont icon-shexiangji",
                        children:[],
                        firstLevel:true,
                        statusID:data[i].strToken,
                    })
                    cloudSrc(item.strToken);
                }
            }
        }
    })
}
// cloudDevice();
function cloudSrc(strToken) {
    var url = root + "/api/v1/GetCloudDeviceSrc?token="+ strToken + "&session=" + store.state.token;
    axios.get(url).then(result => {
        if (result.status == 200){
            var number = 0;
            var numberbDisable = 0;
            var offlineNumber = 0;
            var disableNumber = 0;
            var IdleNumber =0;
            var data =  result.data;
            var srcGroup = [];
            var srcGroup2 = [];
            // srcGroup.label=srclevel.strName;
            // srcGroup.iconclass="iconfont icon-kaiqishexiangtou1";
            for(var i=0; i< data.src.length; i++){
                var item = data.src[i];
                if (item.bOnline&&!item.bDisable) {
                    number=number+1;
                }
                var node=[{
                    token : item['strToken'],
                    streamprofile : "main",
                    label :i18n.tc('message.live.mainstream'),
                    name:item['strName']+"--"+i18n.tc('message.live.mainstream'),
                    iconclass : 'mdi mdi-playlist-play fa-fw',
                    disabled_me:false
                    },{
                    token : item['strToken'],
                    streamprofile : "sub",
                    label :i18n.tc('message.live.substream'),
                    name:item['strName']+"--"+i18n.tc('message.live.substream'),
                    iconclass : 'mdi mdi-playlist-play fa-fw',
                    disabled_me:false
                }]
                var newItem ={
                    token : item['strToken'],
                    label : item['strName'],
                    iconclass : 'iconfont icon-shexiangji',
                    iconclass1:'',
                    name:item['strName']+"--"+i18n.tc('message.live.mainstream'),
                    disabled_me:false,
                    ChannelNumber: item["nChannelNumber"],
                    nType: item["nType"],
                    bOnline: item["bOnline"],
                };
                if(!item['bOnline']){
                    // newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou';
                    newItem['iconclass'] = 'iconfont icon-shexiangji el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                }
                if(!item['bOnline']&&item['bIdle'] == false){ 
                    offlineNumber=offlineNumber+1;
                }
                // if(item['nType'] == 'H5_CLOUD')
                //     newItem['iconclass'] = 'iconfont  icon-kaiqishexiangtou1';

                if(item['bRec'] == true){
                    // newItem['iconclass2'] = 'iconfont icon-radioboxfill none';
                    newItem['iconclass2'] = 'none';
                }

                if(item['bDisable'] == true){
                    newItem['disabled_me'] =true;
                    // newItem['iconclass1'] = 'camera';
                    newItem['iconclass'] = 'iconfont icon-jinyong1 el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                    newItem['iconclass3'] = 'heightlight';
                }
                if(item['bDisable'] == true){
                    if (store.state.devicemarktoggle==undefined||store.state.devicemarktoggle=='block') {
                        disableNumber=disableNumber+1;
                    }
                }
                 
                if(item['bDisable'] == true&&store.state.devicemarktoggle=='none'){
                    numberbDisable = numberbDisable+1;
                    continue;
                }
                if (item['bIdle'] == true) {
                    newItem['iconclass'] = 'iconfont icon-kaiqishexiangtou1 el-tree-camera';
                    newItem['iconclass1'] = 'el-tree-camera';
                    IdleNumber=IdleNumber+1;
                }
                srcGroup.push(newItem);
            }
            var srcData = listdatag.find(item => item.token === strToken)
            // console.log(srcData)
            if(srcData){
                srcData.children=srcGroup
                srcData.length=result.data.src.length-numberbDisable;
                srcData.online=number;
                // srcData.offline=srcData.length-srcData.online;
                srcData.offline=offlineNumber;
                srcData.disable=disableNumber;
                srcData.idle = IdleNumber;
            }
            var srcData1 = listdatagload.find(item => item.token === strToken)
            if(srcData1){
                srcData1.children=srcGroup
                srcData1.length=result.data.src.length-numberbDisable;
                srcData1.online=number;
                // srcData.offline=srcData.length-srcData.online;
                srcData1.offline=offlineNumber;
                srcData1.disable=disableNumber;
                srcData1.idle = IdleNumber;
            }
            var srcData2 = listdataChilde.find(item => item.token === strToken);
            if(srcData2){
                srcGroup2 = JSON.parse(JSON.stringify(srcGroup))
                srcData2.children=srcGroup2;
                srcData2.length=result.data.src.length-numberbDisable;
                srcData2.online=number;
                // srcData2.offline=srcData2.length-srcData2.online;
                srcData2.offline=offlineNumber;
                srcData2.disable=disableNumber;
                srcData2.idle = IdleNumber;
                for(var i=0; i< srcData2.children.length; i++){
                    // 主副流
                    var node=[{
                        token : srcData2.children[i].token,
                        streamprofile : "main",
                        label :i18n.tc('message.live.mainstream'),
                        name:srcData2.children[i].label+"--"+i18n.tc('message.live.mainstream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                        },{
                        token : srcData2.children[i].token,
                        streamprofile : "sub",
                        label :i18n.tc('message.live.substream'),
                        name:srcData2.children[i].label+"--"+i18n.tc('message.live.substream'),
                        iconclass : 'mdi mdi-playlist-play fa-fw',
                        disabled_me:false
                    }]
                    srcData2.children[i].children=node;
                }
            }
            // listdatag.push(srcGroup);;
            // listdatagload.push(srcGroup);
        }
    }).catch(error => {
        console.log('GetSrc failed', error);
    });
}

function Regional(){
    var url = root + "/api/v1/GetRegion?session="+ store.state.token;
    axios.get(url).then(result=>{
        // console.log(result);
        var oldarr=result.data.root;
        var oldarr1=result.data.src;
        // console.log(oldarr,oldarr1)
        var dataroot=getchild(oldarr,oldarr1);
        listdatag1.push(dataroot);
    })
}
function getchild(arr,arr1) {
    
    for(var i in arr.cam){
        if(!arr.cam[i].strName){
            for(var j in arr1){
                if(arr.cam[i].strToken == arr1[j].strToken){
                    arr.cam[i].strName = arr1[j].strName;
                    arr.cam[i].name=arr1[j].strName+"--"+i18n.tc('message.live.mainstream')
                    arr.cam[i].bOnline = arr1[j].bOnline;
                    arr.cam[i].iconclass="iconfont  icon-kaiqishexiangtou1"
                    arr.cam[i].disabled_me=false
                    if(!arr1[j].bOnline)
                        arr.cam[i].iconclass = 'iconfont icon-kaiqishexiangtou';

                    if(arr1[j].nConnectType == 'H5_CLOUD')
                        arr.cam[i].iconclass = 'iconfont  icon-kaiqishexiangtou1';

                    if(arr1[j].bRec == true)
                        arr.cam[i].iconclass2  = 'iconfont icon-radioboxfill none';

                    if(arr1[j].bDisable== true){
                        
                        arr.cam[i].node[0].disabled_me=true;
                        arr.cam[i].node[1].disabled_me=true;
                        arr.cam[i].disabled_me=true;
                        arr.cam[i].iconclass1= 'camera';
                    }
                }
            }
        }
    }
    
    var camarr=[]
    var camarr1=[]
    for(var i in arr.cam){
        camarr.push(arr.cam[i].strToken)
    }
    for(var i in arr1){
        camarr1.push(arr1[i].strToken)
    }
    let diff = camarr.filter(item => !new Set(camarr1).has(item))
    console.log(diff)
    for(var i in arr.cam){
        for(var j in diff){
            if(arr.cam[i].strToken == diff[j]){
                arr.cam[i].strName = '设备暂时不可用';
                arr.cam[i].iconclass="iconfont  icon-kaiqishexiangtou"
            }
        }
    }

    if(arr.node && arr.node.length>0){
        for (var i = 0; i < arr.node.length; i++) {
            arr.node[i] = getchild(arr.node[i],arr1);
        }
    }
    return arr;
}
Regional();
function AllMethods(){
    return new Promise(resolve => {
        if(timer) {
            clearTimeout(timer)
            timer = null
        }
        listdatag=[];
        listdataChilde=[];
        timer = setTimeout(()=> {
            loadtest();
            loadDevice();
            NumberDevice();
            cloudDevice();
            resolve({listdatag,listdataChilde});
        }, 1000)
    })
}
export{listdatag,listdatagload,listdatag1,listdataChilde,AllMethods}