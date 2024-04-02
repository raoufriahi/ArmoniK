<template>
    <div class="AiChannel">
        <div class="AI">
            Basic AI channel : {{aiBasic}} <br><br>
            Number of use : {{aiBasicindex}} <br><br>
            Remainder : {{aiBasic-aiBasicindex}}
        </div>
        <div class="AI">
            Advanced AI channel : {{aiAdv}} <br><br>
            Number of use : {{aiAdvindex}} <br><br>
            Remainder : {{aiAdv-aiAdvindex}}
        </div>
    </div>
</template>

<script>
export default {
	name: 'TheAside',
	data () {
		return {
            aiAdv:0,
            aiBasic:0,
            aiAdvindex:0,
            aiBasicindex:0,
		}
	},
	mounted(){
        this.AiChannel();
	},
	methods:{
        AiChannel(){
            let root = this.$store.state.IPPORT;
            let url = root + "/api/v2/GetAiChannel?session=" + this.$store.state.token;
            this.$http.get(url).then((result) => {
                console.log(result);
                // this.Channel = result.data.AiChannel
                this.aiAdv=result.data.aiAdv;
                this.aiAdvindex=result.data.aiAdvindex;
                this.aiBasic=result.data.aiBasic;
                this.aiBasicindex=result.data.aiBasicindex;
            });
        }
    },
}
</script>

<style lang="scss" scoped>
</style>
