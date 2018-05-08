<style scoped>

</style>


<template>
<div class="container">
  <h1>{{name}}, please wait while you are being paired. Thanks! </h1>
      {{playerID}}

</div>
</template>

<script>
export default{
  data() {
    return {

    }
  },
  computed: {
    playerID: function(){
      return this.$store.getters.playerID;
    },
    name: function(){
      return this.$store.getters.name;
    }
  },
  methods: {

  },


}
</script>
