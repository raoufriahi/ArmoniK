<template>
    <div class="intel">
        <!-- 名字 :  -->
        {{data}}
    </div>
</template>
<script>
export default {
    data(){
        return{
            data:''
        }
    },
    mounted(){
        this.TransGPU();
    },
    methods:{
        TransGPU(){
            var root =this.$store.state.IPPORT;
            var url = root + "/api/v2/GetIntelGPUList?session="+ this.$store.state.token;
            this.$http.get(url).then(result=>{
                console.log(result);
                if(result.status == 200){
                    this.data = result.data.data[0];
                }
            })
        },
    }
}
</script>
<style lang="scss" scoped>
    .intel{
        margin-left:10px;
        padding: 20px;
        background: #1b1b1b;
        width: 550px;
        height: 250px;
        position: relative;
    }
</style>