function triangleTime(ms, pxM, Time,id) {
    // console.log(ms, pxMs,  intervalTime,id);
    let data = new Date(Time)
    let pageShowStartTime = data.getHours()*60*60*1000+data.getMinutes()*60*1000+data.getSeconds()*1000
    let canvasId = document.getElementById(id)
    let width = canvasId.width;
    let pxMs = width/pxM
    let canvansH = canvasId.height;
    let intervalTime = 10*ms;
    /*
    let strtime = new Date().getTime();
    let endtime = new Date().getTime()+25*60*1000;
    let pxs = canvansH/(endtime-strtime)*1000
    let pxg = px/s*6
    let pxdg = px/s*60
    canvansW/(pxdg) 
    */
    let ctx = canvasId.getContext('2d')
    ctx.clearRect(0, 0, width, canvansH)
    ctx.fillStyle = '#999999'
    // 为防止苹果屏幕2X显示不正常
    var getPixelRatio = function (context) {
        var backingStore = context.backingStorePixelRatio ||
            context.webkitBackingStorePixelRatio ||
            context.mozBackingStorePixelRatio ||
            context.msBackingStorePixelRatio ||
            context.oBackingStorePixelRatio ||
            context.backingStorePixelRatio || 1
        return (window.devicePixelRatio || 1) / backingStore
    }
    let ratio = getPixelRatio(ctx)
    // ------------
    let msOffset = startOffsetTime(pageShowStartTime,ms * 1000 ) // 开始的偏移时间 ms
    let initCnavasImgLeft = pageShowStartTime % (intervalTime * 1000)
    let pxOffset = msOffset / 1000 * pxMs // 开始的偏移距离 px
    let graduationLeft = 0
    let graduationTime = 0
    let beginX = 0
    let beginY = 0
    for (let i = 0; i < width / (ms * pxMs); i++) {
        // console.log(width / (ms * pxMs))
         graduationLeft = pxOffset + i * (ms * pxMs) // 距离=开始的偏移距离+格数*px/格
        // graduationTime = pageShowStartTime + msOffset + i * ms  // 时间=左侧开始时间+偏移时间+格数*ms/格
        // graduationTime = pageShowStartTime + graduationLeft * 1000 / pxMs  // 时间=左侧开始时间+偏移时间+格数*ms/格
        beginX = pxOffset + i * (ms * pxMs)
        let canvasColor
        let showTime = pageShowStartTime + (graduationLeft * 1000) / pxMs
        if (showTime % (intervalTime * 1000) === 0) {
            // console.log(showTime);
            beginY = 0
            ctx.font = '9px Arial'
            ctx.fillStyle = 'rgba(151,158,167,1)';
            ctx.fillText(changeTime(showTime, 1), beginX - 20, 20)
            canvasColor = '#fff'
            ctx.fillStyle = '#B1B1B1'
            drawLine(graduationLeft, beginY, graduationLeft, 10, canvasColor, 1 * ratio)
        } else /*if (showTime % intervalTime === 0)*/ {
            beginY = 0
            canvasColor = '#fff'
            drawLine(graduationLeft, beginY, graduationLeft, 5, canvasColor, 1 * ratio)
        }
    }
}

/**
 * 左侧开始时间的偏移，返回单位ms
 */
function startOffsetTime(timestamp, step) {
    let remainder = timestamp % step
    return remainder ? step - remainder : 0
}

/**
 * 根据传入参数画线。。
 */
function drawLine(beginX, beginY, endX, endY, color, width) {
    let canvasId = document.getElementById('timeline')
    let ctx = canvasId.getContext('2d')
    ctx.beginPath()
    ctx.moveTo(beginX, beginY)
    ctx.lineTo(endX, endY)
    ctx.strokeStyle = color
    ctx.lineWidth = width
    ctx.stroke()
}

/**
 * 返回时间
 */
function changeTime(time, num) { // num 区分是传时间还是传日期
    let hour = 0
    let minute = 0
    let second = 0
    second = time / 1000
    if (second >= 3600) {
        minute = (second - (second % 60)) / 60
        hour = parseInt((minute / 60).toString())
        minute = minute % 60
        /* eslint-disable */
        hour >= 10 ? hour : hour = '0' + hour
        minute >= 10 ? minute : minute = '0' + minute
        second = second % 60
        second >= 10 ? second : second = '0' + second
        /* eslint-enable */
        return hour + ':' + minute + ':' + second
    }
    if (second < 3600 && second >= 60) {
        hour = '00'
        minute = parseInt((second / 60).toString())
        /* eslint-disable */
        minute >= 10 ? minute : minute = '0' + minute
        second = second % 60
        second >= 10 ? second : second = '0' + second
        /* eslint-enable */
        return hour + ':' + minute + ':' + second
    }
    if (second < 60) {
        hour = '00'
        minute = '00'
        second = parseInt(second)
        /* eslint-disable */
        second >= 10 ? second : second = '0' + second
        /* eslint-enable */
        return hour + ':' + minute + ':' + second
    }
}
// canvas宽度  一个小刻度的秒数 10像素一个小刻度   初始时间（毫秒）  一大段间隔时长(秒）

// triangleTime(530, 12, 530/600, 0, 120,'timeline')
let initTime = 0
// setInterval(() => {
//     initTime += 1000
//     triangleTime(530, 6, 10/6, initTime, 60)
//     draw_cell()
// }, 1000)
// draw_cell()
// function draw_cell(time,endtime) {
//     let canvasId = document.getElementById('time_line')
//     let ctx = canvasId.getContext('2d')
//     ctx.fillStyle = '#4e6ef2'
//     ctx.fillRect(0,43,530,49);
// }
export {triangleTime}