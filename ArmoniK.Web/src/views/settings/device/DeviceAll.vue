<template>
    <div>
        <!-- two tables -->
        <!-- sheet -->
        
        <div class="All_button_edi">
            <CButton class="form_butt1" @click="exportExcel" type="submit">Export all</CButton>
        </div>
        <el-table
            :data="tableData3.filter(data => !search || data.Name.toLowerCase().includes(search.toLowerCase())).slice((currentPage3-1)*pageSize,currentPage3*pageSize)"
            stripe
            style="width: 100%">
            <el-table-column
                prop="index"
                :label="label.Index"
                width="100">
            </el-table-column>
            <el-table-column
            prop="Name"
            :label="label.Name">
            </el-table-column>
            <el-table-column
            prop="IP"
            :label="label.IP"
            min-width="140">
            </el-table-column>
            <el-table-column
            prop="Port"
            :label="label.Port">
            </el-table-column>
            <el-table-column
            prop="User"
            :label="label.User">
            </el-table-column>
            <el-table-column
            prop="Online"
            :label="label.Online">
            </el-table-column>
            <el-table-column
            prop="Token"
            :label="label.Token"
            min-width="140">
            </el-table-column>
            <el-table-column
            fixed="right"
            prop="Type"
            :label="label.Type"
            min-width="140">
                <template slot="header" slot-scope="scope">
                    <el-input
                    v-model="search"
                    @change="handlechange(scope.$index,scope.row)"
                    size="mini"
                    placeholder="Enter keywords"/>
                </template>
            </el-table-column>
        </el-table>
        <!-- pagination -->
        <el-pagination
            style="text-align: center;"
            layout=" prev, pager, next,total, jumper"
            @size-change="handleSizeChange4" 
            @current-change="handleCurrentChange4"
            :current-page="currentPage3"
            :total="total3">
        </el-pagination>
        
    </div>
</template>
<script>
import uuid from '../../../assets/js/uuid'
import XLSX from 'xlsx'
import '../../public/xlsx.core.min'
  export default {
    data() {
      return {
        eltitle:this.$t("message.setting.Configuration"),
        search: '',//search
        activeName: 'H5_STREAM',//Show tabs first
        inputedit:[],
        label:{
            label:"H5_STREAM",//Pick 1
            label1:"H5_ONVIF",//Pick 2
            label2:"H5_FILE",//Pick 3
            label3:this.$t("message.setting.all"),//Pick 4
            
            Index:this.$t("message.table.Index"),
            Name:this.$t("message.table.Name"),
            IP:this.$t("message.table.IP"),
            Port:this.$t("message.table.Port"),
            User:this.$t("message.table.User"),
            Online:this.$t("message.table.Online"),
            Type:this.$t("message.table.Type"),
            Token:this.$t("message.table.Token")
        },
        options: [{
                value: 'H5_STREAM',
                label: 'H5_STREAM'
            }, {
                value: 'H5_ONVIF',
                label: 'H5_ONVIF'
            }, {
                value: 'H5_FILE',
                label: 'H5_FILE'
            }
        ],
        //pagination
        pageSize: 10,//Number of pages
        dialogFormVisible: false,//Add popup
        editPopup:false,//Edit popup
        form: {
            Type:"H5_STREAM",
            Name:"Cam1",
            Token:"",
            Username:"admin",
            Password:"12345",
            URL:"rtsp://192.168.1.1/stream",
            Audio:"false",
            IP:"192.168.1.1",
            Port:"80"
        },
        editform: {
            Audio:"false"
        },
        edittoken:"",//The token to delete when editing
        editindex:"",//index when editing
        
        tableData3: [],//4
        total3: 0, // Total number 4
        currentPage3: 1, // Current page 4
        selectop:[]//Choose those
      };
    },
    mounted(){
        this.loadqb();
    },
    methods:{
        openDownloadDialog(url, saveName){
            if(typeof url == 'object' && url instanceof Blob)
            {
                url = URL.createObjectURL(url); // Create blob address
            }
            var aLink = document.createElement('a');
            aLink.href = url;
            aLink.download = saveName || ''; // HTML5 new attribute, specify the save file name, you can do not need a suffix, note that it will not take effect in file:/// mode
            var event;
            if(window.MouseEvent) event = new MouseEvent('click');
            else
            {
                event = document.createEvent('MouseEvents');
                event.initMouseEvent('click', true, false, window, 0, 0, 0, 0, 0, false, false, false, false, 0, null);
            }
            aLink.dispatchEvent(event);
        },
        sheet2blob(sheet, sheetName) {
            sheetName = sheetName || 'sheet1';
            var workbook = {
                SheetNames: [sheetName],
                Sheets: {}
            };
            workbook.Sheets[sheetName] = sheet;
            // Generate configuration items for excel
            var wopts = {
                bookType: 'xlsx', // file type to generate
                bookSST: false, // Whether to generate Shared String Table, the official explanation is that if the generation speed is turned on, it will decrease, but it has better compatibility on low-version IOS devices
                type: 'binary'
            };
            var wbout = XLSX.write(workbook, wopts);
            var blob = new Blob([s2ab(wbout)], {type:"application/octet-stream"});
            // String to ArrayBuffer
            function s2ab(s) {
                var buf = new ArrayBuffer(s.length);
                var view = new Uint8Array(buf);
                for (var i=0; i!=s.length; ++i) view[i] = s.charCodeAt(i) & 0xFF;
                return buf;
            }
            return blob;
        },
        exportExcel() {
            // console.log(this.tableData3)
            var aoa =  [
                ['Index', 'Name', 'IP', 'Port','User','Online','Token','Type']
            ];//Assemble the array
            for (var i=0; i<this.tableData3.length; i++) {
                // console.log(this.tableData3[1])
                var data=this.tableData3[i]
                var aoadata=[data.index,data.Name,data.IP,data.Port,data.User,data.Online,data.Token,data.Type]
                aoa.push(aoadata)
            }
            console.log(aoa)
            // return
            var sheet = XLSX.utils.aoa_to_sheet(aoa);
            // sheet['!merges'] = [
            //     // 设置A1-C1的单元格合并
            //     {s: {r: 0, c: 0}, e: {r: 0, c: 2}}
            // ];
            this.openDownloadDialog(this.sheet2blob(sheet), 'Device_Information.xlsx');
        },
        //Data in the fourth table
        loadqb(){
          //url
          var url = this.$store.state.IPPORT + "/api/v1/GetSrc?session="+ this.$store.state.token;
          //console.log("444",url);
            this.$http.get(url).then(result=>{
              //console.log("a",result);
              if(result.status == 200){
                  var itme=result.data.src;
                  
                  for(var i=0;i<itme.length;i++){
                      var tabledata={
                          index:i+1,
                          Type:itme[i].nType,
                          Name:itme[i].strName,
                          Token:itme[i].strToken,
                          User:itme[i].strUser,
                          Password:itme[i].strPasswd,
                          IP:itme[i].strSrcIpAddress,
                          Port:itme[i].strSrcPort,
                          Audio :itme[i].bEnableAudio,
                          Online:itme[i].bOnline+"",
                          bPasswdEncrypt:itme[i].bPasswdEncrypt
                      };
                      this.tableData3.push(tabledata);
                      
                      //console.log(tabledata);
                  }
                  this.total3=this.tableData3.length;
              }
            })
        },
        
        //search
        handlechange(){},
        //pagination
        
        handleSizeChange4(val) {
            console.log(`per page ${val} strip`);
            this.currentPage3 = 1;
            this.pageSize = val;
        },
        handleCurrentChange4(val) {
            console.log(`Current page: ${val}`);
            this.currentPage3 = val;
        },
        
    },
  };
</script>

<style lang="scss" scoped>
.All_button_edi{
    .form_butt1{
        background: #3ABBFE;
        border-radius: 4px;
        line-height: 15px;
        color: #FFFFFF;
    }
    margin-bottom: 10px;
}
</style>