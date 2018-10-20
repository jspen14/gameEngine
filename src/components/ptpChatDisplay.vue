<style>
  ul {
    list-style: none;
  }
  li {
    background: #fff;
    width: 500px;
    min-height: 30px;
    padding: 10px;
    margin-bottom: 10px;
    font-size: 1.5vh;
    display: flex;
    align-items: center;
  }
  .noBorder{
    border: 0;
    box-shadow: none;
  }
  .msgsBox{
    overflow-y: scroll;
    height: 65vh;
    word-wrap: break-word;
  }
  .partnerMsgWrap{
    float: left;
    clear:left;
    max-width: 75%;
    min-width: 75%;
  }
  .partnerMsgDisplay{
    float: left;
    clear: left;
    text-align: left;
    border-radius: 6px;
    border: 2px solid #FE7223;
    background-color: #E5E5EA;
    color: black;
    padding-top: 7px;
    padding-bottom: 7px;
    padding-left: 10px;
    padding-right: 10px;
  }
  .playerMsgWrap{
    float: right;
    clear:right;
    max-width: 75%;
    min-width: 75%;
  }
  .playerMsgDisplay{
    text-align:left;
    float: right;
    clear:right;
    border-radius:6px;
    background-color:#31A3F9;
    color: white;
    padding-top: 7px;
    padding-bottom: 7px;
    padding-left: 10px;
    padding-right: 10px;
  }

  #chatbox {
    overflow:   none;
    position:   relative;
    /* width:      100%;
    height:     65vh;  Inhere*/

  }
  #chatmessages
  {
      overflow:   auto;
      position:   absolute;
      bottom:     0;
      width: 100%;
      max-height: 65vh;
  }

</style>

<template>
  <div>
    <br>
    <h4> Send Message to:</h4>
    <h4 style="color:#FE7223"> Your Partner</h4>
    <hr>

    <div class = "msgsBox" id = "chatbox">
      <div id="chatmessages">
        <div v-for="msg in chatMsgs" >


          <div v-if="!isMe(msg.userID)">
            <div v-if="isCheapTalk(msg.message)">
              <h6 class="partnerMsgDisplay">  {{formatCheapTalk(msg.message)}} </h6>
              <br><br>
            </div>
            <div v-else class="partnerMsgWrap">
              <h6 class="partnerMsgDisplay">  {{msg.message}} </h6>
              <br><br>
            </div>

          </div>

          <div v-else>
            <div class="playerMsgWrap">
              <h6 class="playerMsgDisplay"> {{msg.message}} </h6>
              <br><br>
            </div>

          </div>

        </div>
      </div>
    </div>

    <form v-on:submit.prevent="addChatMsg" class="form-control noBorder">

      <div class="input-group mb-3">
        <input type="text" v-model="msgText" class="form-control" placeholder="Your Message" aria-describedby="basic-addon1">
        <div class="input-group-prepend">
          <button class="btn btn-success" style="border-bottom-right-radius: 5px; border-top-right-radius: 5px; background-color: #31A3F9; " type="submit">Send</button>
        </div>
      </div>

    </form>

  </div>
</template>

<script>
import axios from 'axios'
export default{
  data() {
    return {
      msgText: '',
      messages: [],
      pastLength: 0,
      currentLength: 0,

    }
  },
  created: function(){
    // Put a conditional here that only calls to updateData if there is player to talk to
    this.$store.dispatch('getPartnerChatID');
    this.updateData();
  },
  computed: {
    name: function(){
      return this.$store.getters.user.name;
    },
    chatMsgs: function(){
      // this might be expensive
      return this.messages;
    },


  },
  methods: {
    updateData: function(){
      let timerID = setInterval(() => {

        //this.$store.dispatch('getpartnerChatID'); // See if this will still work if call moved to line 160 of store

        this.$store.dispatch('getPartnerChatMsgs');
        this.messages = this.$store.getters.partnerChatMsgs.slice();
        this.currentLength = this.messages.length;

        $('#chatmessages').scrollTop($('#chatmessages')[0].scrollHeight);

      }, 2000);

    },

    test: function(){
      this.$store.dispatch('getPartnerChatID');
    },

    isMe: function(msgUserID){
      if(msgUserID == this.$store.getters.user.id){
        return true;
      }
      else {
        return false;
      }
    },

    isCheapTalk: function(message){
      if((message[0] == 0 || message[0] == 1) && (message[1] == 0 || message[1] == 1)){
        return true;
      }
      else{
        return false;
      }
    },

    formatCheapTalk: function(message){
      var retString;
      var aiOption;
      var myOption;

      if (this.$store.getters.whichPlayer == "0"){
        if (parseInt(message.charAt(1)) == 0){
          aiOption = "X";
        }
        else{
          aiOption = "Y";
        }

        if (parseInt(message.charAt(0)) == 0){
          myOption = "A";
        }
        else{
          myOption = "B";
        }


        retString = "You play " + myOption + ", and I'll play " + aiOption;
      }
      else {
        if (parseInt(message.charAt(0)) == 0){
          aiOption = "A";
        }
        else{
          aiOption = "B";
        }

        if (parseInt(message.charAt(1)) == 0){
          myOption = "X";
        }
        else{
          myOption = "Y";
        }


        retString = "You play " + myOption + ", and I'll play " + aiOption;
      }

      return retString;
    },

    addChatMsg: function(){
      if(this.msgText.length > 254){
        swal("Error","Message is too long to send.","warning");
      }
      else{
        this.$store.dispatch('addChatMsgToPartner', {
          text: this.msgText,
        }).then(func => {
          this.msgText = '';
          this.$store.dispatch('getPartnerChatMsgs');
        }).then(func2 => {
          $('#chatmessages').scrollTop($('#chatmessages')[0].scrollHeight);
        });
      }
      return;
    },
  },
}
</script>
