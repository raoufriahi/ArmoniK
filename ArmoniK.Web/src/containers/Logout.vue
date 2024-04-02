<template>
<div>
</div>
</template>

<script>
export default {
    name: 'logout',
    data(){
        return {
            e1:undefined
        }
    },
    mounted() {
        console.log("logout");
        // this.$store.commit(types.LOGOUT);
        this.open()
        // location.reload();
    },
    methods:{
        open(){
            sessionStorage.removeItem('mcutoken')
            this.$store.state.token = null
            sessionStorage.removeItem('mcuuser')
            this.$store.state.user = null
            sessionStorage.removeItem('mcuroot')
            this.$store.state.root = null
            this.$router.push({
                path: 'login'
            })
            sessionStorage.removeItem('subscriptType')
            if (this.$store.state.e1) {
                this.$store.state.e1.keepaliveTimer("unsubscribe");
            }
        }
    }
}
</script>

