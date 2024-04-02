<template>
    <div class="storage"> 
        <div class="storage_rights">
           <iframe width="100%" height="100%" id="terminal" frameborder="0" class="ifram"></iframe>
           <div class="divifram"></div>
       </div>
    </div>
</template>
<script>
//终端
export default {
    data(){
        return{}
    },
    methods:{
        url(){
           let iframe = document.getElementById("terminal");
           var configport=this.$store.state.strNodeName
           var configip=this.$store.state.IPPORT
           var configipnew=configip.split(":")
           iframe.setAttribute("src", configip+"/cockpit/@localhost/system/terminal.html");
       } 
	},
	mounted(){
        this.url()
	},
}
</script>
<style scoped>
.storage{
    padding: 10px 10px;
}
.storage_rights{
    height: 87vh;
}
</style>