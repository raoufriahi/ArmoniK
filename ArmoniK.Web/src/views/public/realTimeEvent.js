import {IotEventExt} from '../../assets/js/ioteventext.js'
import store from '../../store'
import axios from 'axios'
import uuid from '../.././assets/js/uuid'
console.log(store.state.token,store.state.root)
let ExtSupport= false;
store.state.gEvent=[];
var e1=undefined;
var flag =true;
let obj = {}
var Eventuuid=uuid();
function EventCB(event, userdata)
{   
   
    var msgevent = JSON.parse(event);
    let timeitem={
            Token: msgevent.strDevToken,
            Type: msgevent.type,
            UUID:msgevent.strUUID,
            Time:msgevent.strTime,                      
            Detail:event
        };
    // gEvent.unshift(timeitem);
    // store.state.gEvent=gEvent;
    store.state.gEvent.unshift(timeitem);
    //console.log(gEvent.length)
}
function realEvent () {
    if(store.state.token==null){
        return false;
    }
    let url = store.state.IPPORT + "/api/v1/GetAdvSystemInfo?session="+ store.state.token;
    // let obj = {types:["H5S_EVENT_ONLINE","H5S_EVENT_OFFLINE","H5S_EVENT_MOTION"]};
    if (sessionStorage.getItem('subscriptType')) {
        obj={type:JSON.parse(sessionStorage.getItem('subscriptType'))}
    }else{
        obj = {type:{types:[],uuid:Eventuuid}};
        sessionStorage.setItem('subscriptType',JSON.stringify(obj));
    }
    if(!flag) { return }

     axios.get(url).then(result=>{
        if (result.status == 200){
            console.log(result.data.bExtSupport);
            if (result.data.bExtSupport) {
                flag = false;
                // let eventhost=window.location.host.split(':');
                var conf1 = {
                    protocol: window.location.protocol, //http: or https:
                    host:store.state.WSROOT, //localhost:8080
                    rootpath:'/', // '/'
                    callback: EventCB, 
                    userdata: null, // user data
                    session: store.state.token, //session got from login
                    Subscription: obj,
                };
                e1 = new IotEventExt(conf1);
                e1.connect();
                store.state.e1=e1
                console.log(e1.wsSocket);
            }
        } 
    }).catch((err) =>{
        console.log('GetSrc failed', err);
    });
}
// var url =store.state.IPPORT + "/api/v1/GetSrc?session=" +  store.state.token;

// fetch(url).then((result) => {
//     console.log("a", result);
//     if (result.status == 200) {
//       var itme = result.data.src;
//       for (var i = 0; i < itme.length; i++) {
//           obj.token=itme[i].strToken;
//           obj.types = item[i].type;
//       }
//     }
// })

// function events(){
    // if(store.state.token==null){
    //     return false;
    // }
    // if (e1.wsSocket.readyState===1) {
    // e1.wsSocket.send(JSON.stringify(store.state.subscribeEvent));
    // }
// }
// events();
realEvent();
export default {realEvent};