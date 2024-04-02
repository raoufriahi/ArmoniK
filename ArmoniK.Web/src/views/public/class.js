import {message} from '../../assets/js/resetMessage';
function DataClass (conf) {
    this.conf =conf
    this.token =conf.token
    this.value = conf.value
    this.value1 = ''
    this.fillStyle = 'red'
    this.video = document.getElementById(conf.videoid);
    this.width= conf.width||document.getElementById(conf.videoid).videoWidth;
    this.height= conf.height||document.getElementById(conf.videoid).videoHeight;
    this.img = null;
    this.c = document.getElementById(conf.canvasid);
    this.ctx ;
    this.layers = conf.layers||[];
    this.currentR= undefined;//当前点击的矩形框
    this.startx = undefined;//起始x坐标
    this.starty = undefined;//起始y坐标
    this.flag = undefined;//是否点击鼠标的标志
    this.x = undefined;
    this.y = undefined;
    this.leftDistance= undefined;
    this.topDistance= undefined;
    this.op = 0;//op操作类型 0 无操作 1 画矩形框 2 拖动矩形框
    this.scale = 1;
    this.type = 0;
    this.scaleStep = 1.05;
    this.elementWidth = 900;
    this.elementHeight = 500;
    this.minWidth = 900;
    this.minHeight = 500;
    this.maxWidth = 1200;
    this.maxHeight = 800;
    this.canvasstart = true;
    this.mover = conf.mover||false;
    this.start = true
    this.init();
}

DataClass.prototype.init = function (){
    this.c.width = 900;
    this.c.height = 500;
    this.ctx = this.c.getContext('2d');
    this.ctx.clearRect(0, 0, 900, 500);
    if (this.conf.img) {
        this.ctx.drawImage(this.conf.img, 0, 0, 900, 500);
        this.img = this.conf.img;
        this.reshow()
    }else{
        this.ctx.drawImage(this.video, 0, 0, 900, 500);
        var MIME_TYPE = "image/jpeg";
        var imgURL = this.c.toDataURL(MIME_TYPE,1.0);
        this.img = new Image();
        this.img.src = imgURL;
    }
    this.c.onmouseleave = ()=> {
        this.c.onmousedown = null;
        this.c.onmousemove = null;
        this.c.onmouseup = null;
        this.c.onmousewheel = null;
    }
    this.c.onmouseenter = ()=> {
        let _this =this
        this.c.onmousedown = (e)=>
        {
            e.preventDefault();
            $('#'+this.conf.bzelect).hide();
            console.log($('#'+this.conf.bzelect).get(0));
            if (!this.mover) {
                if (!this.value) {
                    message({
                        message: "请选择库",
                        type: 'warning'
                    });
                    return
                }
            }
            this.startx =e.offsetX/ this.scale;
            this.starty =e.offsetY / this.scale;
            console.log(this.layers);
            this.layers.forEach((item,i)=> {
                item.fillStyle = ''
                if (this.startx>=item.x1&&this.startx<=item.x2&this.starty>=item.y1&&this.starty<=item.y2) {
                    item.fillStyle = this.fillStyle
                    
                }
                this.ctx.drawImage(this.img, 0, 0, 900, 500);
                this.reshow()
            })
            if (!this.mover) {
                this.currentR = this.isPointInRetc(this.startx, this.starty);
                if (this.currentR) {
                    this.leftDistance = this.startx - this.currentR.x1;
                    this.topDistance = this.starty - this.currentR.y1;
                }
                this.ctx.strokeRect(this.x, this.y, 0, 0);
                this.ctx.strokeStyle = this.fillStyle;
                this.flag = 1;
            }else{
                this.flag = 1;
            }
        };
        this.c.onmousemove = (e)=>{
            e.preventDefault();
            this.x = e.offsetX / this.scale;
            this.y = e.offsetY / this.scale;
            if (!this.mover) {
                this.ctx.save();
                this.ctx.setLineDash([5])
                this.c.style.cursor = "default";
                // ctx.clearRect(0, 0, elementWidth, elementHeight)
                this.ctx.drawImage(this.img, 0, 0, 900, 500);
                if (this.flag && this.op == 1) {
                    this.ctx.strokeRect(this.startx, this.starty, this.x - this.startx, this.y - this.starty);
                }
                this.ctx.restore();
                this.reshow(this.x, this.y);
            }else if(this.mover&&this.flag == 1){
                this.c.style.left = parseInt(this.c.style.left) + this.x-this.startx +'px';
                this.c.style.top = parseInt(this.c.style.top) + this.y-this.starty +'px';
            }
        };
        // c.onmousewheel = onmousewheel;
        this.c.onmouseup = (e)=>{
            e.preventDefault();
            $('#'+this.conf.bzelect).hide();
            if (!this.mover) {
                if (this.op == 1&&this.startx!=this.x&&this.starty!=this.y) {
                    this.layers.push(this.fixPosition({
                        x1: this.startx,
                        y1: this.starty,
                        x2: this.x,
                        y2: this.y,
                        strokeStyle: this.fillStyle,
                        type: this.value1
                    }))
                    this.conf.getclass(this.value , this.y, this.x);
                } else if (this.op >= 3) {
                    this.fixPosition(this.currentR)
                }
                this.currentR = null;
                this.flag = 0;
                this.reshow(this.x, this.y);
                this.op = 0;
            }else{
                this.flag = 0;
            }
        };
        this.c.onmousewheel = (e)=>{
            let canvas = this.c;
            if (e.wheelDeltaY>0&&this.mover) {
                if (canvas.width <= this.maxWidth && canvas.height <= this.maxHeight) {
                    canvas.width *= this.scaleStep;
                    canvas.height *= this.scaleStep;
                    this.scale = canvas.height / this.minHeight;
                    this.ctx.scale(this.scale, this.scale)
                    canvas.style.left = this.x-(canvas.width*this.x)/900 +'px';
                    canvas.style.top = this.y-(canvas.height*this.y)/506 +"px";
                    this.ctx.drawImage(this.img, 0, 0, 900, 500);
                    this.reshow(this.ctx,this.x,this.y,this.flag,this.op)
                }
            }else if (e.wheelDeltaY<0&&this.mover){
                if (canvas.width >= this.minWidth && canvas.height >= this.minHeight) {
                    canvas.width /= this.scaleStep;
                    canvas.height /= this.scaleStep;
                    this.scale = canvas.height / this.minHeight;
                    this.ctx.scale(this.scale, this.scale);
                    canvas.style.left = this.x-(canvas.width*this.x)/900 +'px';
                    canvas.style.top = this.y-canvas.height*this.y/506 +"px";
                    this.ctx.drawImage(this.img, 0, 0, 900, 500);
                    this.reshow(this.ctx,this.x, this.y,this.flag,this.op);
                }
            }
        }
    }
}
DataClass.prototype.edit = function (){
    this.mover = !this.mover;
}
DataClass.prototype.clears = function (data){
    this.layers.forEach((item,i)=> {
        if (this.startx>=item.x1&&this.startx<=item.x2&this.starty>=item.y1&&this.starty<=item.y2) {
            this.layers.splice(i,1)
            let canvas = document.getElementById(this.conf.canvasid);
            canvas.width = 900;
            canvas.height = 500;
            let ctx = canvas.getContext('2d');
            ctx.clearRect(0, 0, 900, 500);
            ctx.drawImage(this.img, 0, 0, 900, 500);
            this.reshow()
        }
    })
}
DataClass.prototype.movers = function (data){
    this.mover = !this.mover
    let canvas = document.getElementById(this.conf.canvasid);
    canvas.style.cursor = "pointer";
}
DataClass.prototype.refresh = function (data){
    this.layers = []
    let canvas = document.getElementById(this.conf.canvasid);
    canvas.width = 900;
    canvas.height = 500;
    let ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, 900, 500);
    ctx.drawImage(this.video, 0, 0, 900, 500);
    var MIME_TYPE = "image/jpeg";
    var imgURL = canvas.toDataURL(MIME_TYPE,1.0);
    this.img = new Image();
    this.img.src = imgURL;
    this.reshow()
}
DataClass.prototype.reclass = function (data){
    this.value = data;
}
DataClass.prototype.bzclass = function (data){
    console.log(data);
    this.layers.forEach((item,i)=> {
        if (this.startx>=item.x1&&this.startx<=item.x2&this.starty>=item.y1&&this.starty<=item.y2) {
            item.type = data;
        }
    })
    this.value1 = data;
}
DataClass.prototype.clear = function (){
    this.layers = []
    let canvas = document.getElementById(this.conf.canvasid);
    canvas.width = 900;
    canvas.height = 500;
    let ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, 900, 500);
    ctx.drawImage(this.img, 0, 0, 900, 500);
}
DataClass.prototype.resizeLeft = function (rect){
    this.c.style.cursor = "w-resize";
    if (this.flag && this.op == 0) { this.op = 3; }
    if (this.flag && this.op == 3) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.x1 =  this.x
        this.currentR.width = this.currentR.x2 - this.currentR.x1
    }
}
DataClass.prototype.resizeTop = function (rect){
    this.c.style.cursor = "s-resize";
    if (this.flag && this.op == 0) { this.op = 4; }
    if (this.flag && this.op == 4) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.y1 =  this.y
        this.currentR.height = this.currentR.y2 - this.currentR.y1
    }
}
DataClass.prototype.resizeWidth = function (rect){
    this.c.style.cursor = "w-resize";
    if (this.flag && this.op == 0) { this.op = 5; }
    if (this.flag && this.op == 5) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.x2 =  this.x
        this.currentR.width = this.currentR.x2 - this.currentR.x1
    }
}
DataClass.prototype.resizeHeight = function (rect){
    this.c.style.cursor = "s-resize";
    if (this.flag && this.op == 0) { this.op = 6; }
    if (this.flag && this.op == 6) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.y2 =  this.y
        this.currentR.height = this.currentR.y2 - this.currentR.y1
    }
}
DataClass.prototype.resizeLT = function (rect){
    this.c.style.cursor = "se-resize";
    if (this.flag && this.op == 0) { this.op = 7; }
    if (this.flag && this.op == 7) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.y1 =  this.y
        this.currentR.x1 =  this.x
        this.currentR.height = this.currentR.y2 - this.currentR.y1
        this.currentR.width = this.currentR.x2 - this.currentR.x1
    }
}
DataClass.prototype.resizeWH = function (rect){
    this.c.style.cursor = "se-resize";
    if (this.flag && this.op == 0) { this.op = 8; }
    if (this.flag && this.op == 8) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.y2 =  this.y
        this.currentR.x2 =  this.x
        this.currentR.height = this.currentR.y2 - this.currentR.y1
        this.currentR.width = this.currentR.x2 - this.currentR.x1
    }
}
DataClass.prototype.resizeLH = function (rect){
    this.c.style.cursor = "se-resize";
    if (this.flag && this.op == 0) { this.op = 9; }
    if (this.flag && this.op == 9) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.x1 =  this.x
        this.currentR.y2 =  this.y
        this.currentR.height = this.currentR.y2 - this.currentR.y1
        this.currentR.width = this.currentR.x2 - this.currentR.x1
    }
}
DataClass.prototype.resizeWT = function (rect){
    this.c.style.cursor = "se-resize";
    if (this.flag && this.op == 0) { this.op = 10; }
    if (this.flag && this.op == 10) {
        if (!this.currentR) { this.currentR = rect }
        this.currentR.x2 =  this.x
        this.currentR.y1 =  this.y
        this.currentR.height = this.currentR.y2 - this.currentR.y1
        this.currentR.width = this.currentR.x2 - this.currentR.x1
    }
}
DataClass.prototype.reshow = function (x,y){    
    let allNotIn = 1;
    let scale = this.scale;
    let ctx = this.ctx;
    this.layers.forEach(item => {
        ctx.beginPath();
        ctx.rect(item.x1, item.y1, item.width, item.height);
        if (item.fillStyle!='') {
            ctx.fillStyle = "rgba(255, 185, 0, 0.4000)";
            ctx.fill();
            this.conf.reshow(item);
        }
        ctx.strokeStyle = this.fillStyle;
        if(x>=(item.x1-10/scale)&&x<=(item.x1+10/scale)&&y<=(item.y2-10/scale)&&y>=(item.y1+10/scale)){
            this.resizeLeft(item);
        }else if(x>=(item.x2-10/scale)&&x<=(item.x2+10/scale)&&y<=(item.y2-10/scale)&&y>=(item.y1+10/scale)){
            this.resizeWidth(item);
        }else if(y>=(item.y1-10/scale)&&y<=(item.y1+10/scale)&&x<=(item.x2-10/scale)&&x>=(item.x1+10/scale)){
            this.resizeTop(item);
        }else if(y>=(item.y2-10/scale)&&y<=(item.y2+10/scale)&&x<=(item.x2-10/scale)&&x>=(item.x1+10/scale)){
            this.resizeHeight(item);
        }else if(x>=(item.x1-10/scale)&&x<=(item.x1+10/scale)&&y<=(item.y1+10/scale)&&y>=(item.y1-10/scale)){
            this.resizeLT(item);
        }else if(x>=(item.x2-10/scale)&&x<=(item.x2+10/scale)&&y<=(item.y2+10/scale)&&y>=(item.y2-10/scale)){
            this.resizeWH(item);
        }else if(x>=(item.x1-10/scale)&&x<=(item.x1+10/scale)&&y<=(item.y2+10/scale)&&y>=(item.y2-10/scale)){
            this.resizeLH(item);
        }else if(x>=(item.x2-10/scale)&&x<=(item.x2+10/scale)&&y<=(item.y1+10/scale)&&y>=(item.y1-10/scale)){
            this.resizeWT(item);
        }
        if (ctx.isPointInPath((x) * scale, (y) * scale)) {
            this.render(item);
            allNotIn = 0;
        }
        ctx.stroke();
    })
    if (this.flag && allNotIn && this.op < 3) {
        this.op = 1
    }
}
DataClass.prototype.render = function (rect){
    this.c.style.cursor = "move";
    if (this.flag && this.op == 0) { this.op = 2; }
    if (this.flag && this.op == 2) {
        if (!this.currentR) {  this.currentR = rect }
        this.currentR.x2 += this.x - this.leftDistance - this.currentR.x1
        this.currentR.x1 += this.x - this.leftDistance - this.currentR.x1
        this.currentR.y2 += this.y - this.topDistance - this.currentR.y1
        this.currentR.y1 += this.y - this.topDistance - this.currentR.y1
    }
}
DataClass.prototype.isPointInRetc = function (x, y){
    let len = this.layers.length;
    for (let i = 0; i < len; i++) {
        if (this.layers[i].x1 < x && x < this.layers[i].x2 && this.layers[i].y1 < y && y < this.layers[i].y2) {
            return this.layers[i];
        }
    }
}
DataClass.prototype.fixPosition = function (position){
    if (position.x1 > position.x2) {
        let x = position.x1;
        position.x1 = position.x2;
        position.x2 = x;
    }
    if (position.y1 > position.y2) {
        let y = position.y1;
        position.y1 = position.y2;
        position.y2 = y;
    }
    position.width = position.x2 - position.x1
    position.height = position.y2 - position.y1
    return position
}
export default DataClass