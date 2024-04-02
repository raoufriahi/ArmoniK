import {IotEventExt} from '../../assets/js/ioteventext'
import store from '../../store'
import axios from 'axios'
let gEvent=[];
var e1=undefined;
function EventCB(event, userdata){
    var msgevent = JSON.parse(event);
    let timeitem={
            Token: msgevent.strDevToken,
            Type: msgevent.type,
            UUID:msgevent.strUUID,
            Time:msgevent.strTime,                      
            Detail:event
        };
    gEvent.unshift(timeitem);
}
let obj = {types:["person_vehicle"]};
var conf1 = {
    protocol: window.location.protocol, //http: or https:
    host:store.state.WSROOT, //localhost:8080
    rootpath:'/', // '/'
    callback: EventCB, 
    userdata: null, // user data
    session: store.state.token, //session got from login
    Subscription: obj,
    type:'ai'
};
e1 = new IotEventExt(conf1);
e1.connect();
export default{gEvent,e1}