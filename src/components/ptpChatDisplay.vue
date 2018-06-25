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


  /* Attempt to get chat looking like an actual chat (iMessage)*/
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
    <h4> {{title}} </h4>

    <hr>

    <div class = "msgsBox" id = "chatbox">
      <div id="chatmessages">
        <div v-for="msg in chatMsgs" >


          <div v-if="!isMe(msg.userID)">
            <div class="partnerMsgWrap">
              <h6 class="partnerMsgDisplay">  {{msg.message}}</h6>
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
      title: 'Send Message to: \n Your Partner',
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

    addChatMsg: function(){
      console.log("Length: ", this.msgText.length);
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
    },
  },
}
</script>
