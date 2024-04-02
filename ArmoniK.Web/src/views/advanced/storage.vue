<template>
  <div class="storage">
    <div class="storage_rights">
      <iframe  
      src="http://192.168.100.150:8080/cockpit/@localhost/storage/index.html"
      width="100%"  height="100%"  id="terminal"  frameborder="0"  class="ifram"></iframe>
      <div class="divifram"></div>
    </div>
  </div>
</template>
<script>
//存储
export default {
  data() {
    return {};
  },
  methods: {
    url(){
      let iframe = document.getElementById("terminal");
      let configport=this.$store.state.strNodeName;
      let configip=this.$store.state.IPPORT;
      let configipnew=configip.split(":");
      iframe.setAttribute("src", configip+"/cockpit/@localhost/storage/index.html");
    },
  },
  mounted() {
    this.url()
  },
};
</script>
<style scoped>
.storage{
    padding: 10px 10px;
}
.storage_rights {
  height: 87vh;
}
</style>