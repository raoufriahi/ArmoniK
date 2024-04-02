import store from '../../store'
function IotEventExt(conf)
{
	console.log('conf',conf);
  	console.log(conf.host);
	this.wsSocket;
	this.keepaliveTimerId;
	this.bNeedReconnect = false;
	this.bDisConnected = false;
	
	this._debug = true;	
	if (conf.consolelog !== undefined)
	{
		if (conf.consolelog === 'false')
		{
			this._debug = false;	
		}
	}	
	

	this._conf = conf;	
}

// 再连接函数
IotEventExt.prototype.ReconnectFunction = function() 
{
	//if(this._debug === true) console.log('Try Reconnect...', this.bNeedReconnect);
	if (this.bNeedReconnect === true)
	{
		if(this._debug === true) console.log('Reconnect...');
		
		this.setupWebSocket(this._token);
		this.bNeedReconnect = false;
	}
	//if(this._debug === true) console.log('Try Reconnect...', this.bNeedReconnect);
}
	
	
IotEventExt.prototype.H5SWebSocketClient = function(h5spath) 
{
  console.log(h5spath);
	var socket;
	if(this._debug === true) console.log("H5SWebSocketClient");
	try {
		//alert(this._conf.protocol);
		if (this._conf.protocol == "http:") 
		{
			if (typeof MozWebSocket != "undefined")
			{
				socket = new MozWebSocket('ws://' + this._conf.host +h5spath);
        console.log(socket);
			}else
			{
				socket = new WebSocket('ws://' + this._conf.host +h5spath);
        console.log('ws://' + this._conf.host);
			}
		}
		if (this._conf.protocol == "https:")
		{	
			//alert(this._conf.host);
			if(this._debug === true) console.log(this._conf.host);
			if (typeof MozWebSocket != "undefined")
			{
				socket = new MozWebSocket('wss://' + this._conf.host+h5spath);
			}else
			{
				socket = new WebSocket('wss://' + this._conf.host+h5spath );
			}				
		}
		if(this._debug === true) console.log(this._conf.host);
	} catch (e) {
		alert('error');
		return;
	}
	return socket;
}

IotEventExt.prototype.keepaliveTimer = function(event)	
{
	try {
		var j = {};
		j.type = event||"keepalive";
		if (event) {
			this.wsSocket.send(JSON.stringify(j));
		}else{
			this.wsSocket.send(JSON.stringify(j));
		}
	} catch (e) {
	  if(this._debug === true) console.log(e);
	}
}


IotEventExt.prototype.onWebSocketData = function(msg)	
{	
	if (this._conf.callback != undefined)
	{
		this._conf.callback(msg.data, this._conf.userdata);
	}
} 

// IotEventExt.prototype.send = function(message) {
// 	console.log(11111111);
// 	console.log(this);
// 	// this.h5.wsSocket.send(JSON.stringify('message111111111111'));
		
// }


IotEventExt.prototype.setupWebSocket = function(token,type)	
{	
	if (type == 'ai') {
		var h5spath = "api/ws/v2/ioteventai";
	}else{
		var h5spath = "api/ws/v2/ioteventext";
	}

	h5spath = this._conf.rootpath + h5spath + '?session=' + this._conf.session;
	
	if(this._debug === true) console.log(h5spath);
	
	this.wsSocket = this.H5SWebSocketClient(h5spath);
  
	if(this._debug === true) console.log("setupWebSocket", this.wsSocket);
	this.wsSocket.binaryType = 'arraybuffer';
	this.wsSocket.h5 = this;
	console.log(this);
	this.wsSocket.onmessage = this.onWebSocketData.bind(this);
	// this.wsSocket.onmessage = (e)=> {
	// 	// console.log(store.state.subscribeEvent);
		
	// }
	// this.h5.wsSocket.send('1111111111')
	this.wsSocket.onopen = function()
	{
		if(this.h5._debug === true) console.log("wsSocket.onopen", this.h5);
    // this.wsSocket.send(JSON.stringify(this._conf.Subscription));
		// console.log(this.h5._conf.Subscription);
		// console.log(this.h5.wsSocket);
		this.h5.wsSocket.send(JSON.stringify(this.h5._conf.Subscription));
		
		this.h5.keepaliveTimerId = setInterval(this.h5.keepaliveTimer.bind(this.h5), 3000);
		// if (store.state.subscribeEvent) {
		// 	this.h5.wsSocket.send(JSON.stringify(store.state.subscribeEvent));
		// }
	}
	
	this.wsSocket.onclose = function (err) {
    console.log(err);
		if(this.h5._debug === true) console.log("wsSocket.onclose", this.h5);
		if (this.h5.bDisConnected === true)
		{
			if(this.h5._debug === true) console.log("wsSocket.onclose disconnect");
		}else
		{
			this.h5.bNeedReconnect = true;
		}
		this.h5.CleanupWebSocket(this.h5);
	}

}


IotEventExt.prototype.CleanupWebSocket = function(h5sPlayer)
{
	if(h5sPlayer._debug === true) console.log('CleanupWebSocket', h5sPlayer);
	clearInterval(h5sPlayer.keepaliveTimerId);
	h5sPlayer.emptyBuffCnt = 0;
	h5sPlayer.lastBuffTime = 0;
	h5sPlayer.buffTimeSameCnt = 0;
}


/** 
 * Connect a websocket Stream to videoElement 
*/
IotEventExt.prototype.connect = function() {
	/* start connect to server */
	this.setupWebSocket(this._token,this._conf.type);
	this.reconnectTimerId = setInterval(this.ReconnectFunction.bind(this), 3000);
}


/** 
 * Disconnect a websocket Stream and clear videoElement source
*/
IotEventExt.prototype.disconnect = function() {
	if(this._debug === true) console.log("disconnect", this);
	this.bDisConnected = true;
	clearInterval(this.reconnectTimerId);
	
	if (this.wsSocket != null)
	{
		this.wsSocket.close();
		this.wsSocket = null;
	}
	if(this._debug === true) console.log("disconnect", this);
} 
export {IotEventExt}
