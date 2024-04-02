
   
import {H5sEvent} from '../../assets/js/h5sevent'
import store from '../../store'
console.log(store.state.token,store.state.root)
let gEvent=[];
let mapEvent = [];
let mapEventarr = [];
var e1=undefined;
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
    if (msgevent.type=='H5S_EVENT_MOBILE_POSITION') {
        if (mapEventarr.indexOf(msgevent.strDevToken)=='-1') {
            mapEventarr.push(msgevent.strDevToken)
            let object={
                Token: msgevent.strDevToken,
                Detail:msgevent
            };
            mapEvent.push(object);
        }else{
            mapEvent.map((item)=>{
                if (item.Token = msgevent.strDevToken) {
                    item.Detail = msgevent 
                }
            })
        }
    }
    gEvent.unshift(timeitem);
    //console.log(gEvent.length)
}
function events(){
    if(store.state.token==null){
        return false;
    }
    var conf1 = {
        protocol: window.location.protocol, //http: or https:
        host:store.state.WSROOT, //localhost:8080
        rootpath:'/', // '/'
        callback: EventCB, 
        userdata: null, // user data
        session: store.state.token //session got from login
    };
    e1 = new H5sEvent(conf1);
    if (sessionStorage.getItem('mcutoken')) {
        e1.connect();
        store.state.e2=e1
    }
}
events();
export default{gEvent,mapEvent}