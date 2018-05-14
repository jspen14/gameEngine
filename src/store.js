import Vue from 'vue';
import Vuex from 'vuex';
import axios from 'axios';

Vue.use(Vuex);

const getAuthHeader = () => {
   return { headers: {'Authorization': localStorage.getItem('token')}};
 }

export default new Vuex.Store({
  state: {
    inGameStatus: 'false',
    currentRound: 1,
    currentGame: '',
    user: {},
    coachChatID: '',
    partnerChatID: '',
    matrix: [],
    coachChatMsgs: [],
    partnerChatMsgs: [],

    token: '',
    loginError: '',
    registerError: '',
  },
  getters: {
    inGameStatus: state => state.inGameStatus,
    currentRound: state => state.currentRound,
    currentGame: state => state.currentGame,
    user: state => state.user,
    coachChatID: state => state.coachChatID,
    partnerChatID: state => state.partnerChatID,
    matrix: state => state.matrix,
    coachChatMsgs: state => state.coachChatMsgs,
    partnerChatMsgs: state => state.partnerChatMsgs,

    //added for registration
    getToken: state => state.token,
    loggedIn: state => {
      if (state.token === '')
       return false;
      return true;
    },
    loginError: state => state.loginError,
    registerError: state => state.registerError,
  },
  mutations: {
    setInGameStatus (state, inGameStatus){
      state.inGameStatus = inGameStatus;
    },
    setCurrentGame (state, currentGame){
      state.currentGame = currentGame;
    },
    setMatrix (state, matrix){
      state.matrix = matrix;
    },
    setCoachChatID (state, coachChatID){
      state.coachChatID = coachChatID;
    },
    setCoachChatMsgs (state, coachChatMsgs){
      state.coachChatMsgs = coachChatMsgs;
    },
    setPartnerChatMsgs (state, partnerChatMsgs){
      state.partnerChatMsgs = partnerChatMsgs;
    },
    //added for login and registration
    setUser (state, user) {
      state.user = user;
    },
    setToken (state, token) {
      state.token = token;
      if (token === '')
          localStorage.removeItem('token');
      else
          localStorage.setItem('token', token)
    },
    setLoginError (state, message) {
      state.loginError = message;
    },
    setRegisterError (state, message) {
      state.registerError = message;
    },

  },
  actions: {
    updateData(context){
      let timerID = setInterval(() => {
        // We will eventually need to set conditionals on these functions to conserve power
        axios.get('/api/inGameStatus/' + context.state.user.id).then(response => {
          context.commit('setInGameStatus',response.data.inGameStatus);
          context.commit('setCurrentGame',response.data.gameID);
        }).catch(err => {
          console.log("GET in updataData store side Failed: " + err);
        });




      }, 3000);
    },
  // Initialize //
    initialize(context) {
      let token = localStorage.getItem('token');
      if (token) {
  // see if we can use the token to get my user account
        axios.get("/api/me",getAuthHeader()).then(response => {
          context.commit('setToken',token);
          context.commit('setUser',response.data.user);
        }).catch(err => {
          // remove token and user from state
          context.commit('setUser',{});
          context.commit('setToken','');
        });
      }
    },
  // Registration, Login //

    login(context,user) {
      return axios.post("/api/login",user).then(response => {
        context.commit('setUser', response.data.user);
        context.commit('setToken',response.data.token);
        context.commit('setRegisterError',"");
        context.commit('setLoginError',"");
      }).catch(error => {
        context.commit('setUser',{});
        context.commit('setToken','');
        context.commit('setRegisterError',"");
        if (error.response) {
          if (error.response.status === 403 || error.response.status === 400)
          context.commit('setLoginError',"Invalid login.");
          context.commit('setRegisterError',"");
          return;
        }
        context.commit('setLoginError',"Sorry, your request failed. We will look into it.");
      });
    },

    logout(context,user) {
      context.commit('setUser', {});
      context.commit('setToken','');
    },

    register(context,user) {
      return axios.post("/api/users",user).then(response => {
      context.commit('setUser', response.data.user);
      context.commit('setToken',response.data.token);
      context.commit('setRegisterError',"");
      context.commit('setLoginError',"");
          }).catch(error => {
      context.commit('setLoginError',"");
      context.commit('setUser',{});
      context.commit('setToken','');
      if (error.response) {
        if (error.response.status === 409)
          context.commit('setRegisterError',"That user name is already taken.");
        return;
      }
  context.commit('setRegisterError',"Sorry, your request failed. We will look into it.");
      });

    },

//START JSPENCER CHAT STUFF
    getCoachChatID(context){
      axios.get('/api/coachChatID/'+ context.state.user.id +'/'+ context.state.currentGame).then(response => { // context.state.user.id/context.state.currentGame
        context.commit('setCoachChatID', response.data.id);
        console.log("setCoachChatID: " + context.state.coachChatID);
      }).catch(err => {
        console.log("getCoachChatID Failed: " + err);

      });
    },

    addChatMsg(context, msgInfo){
      axios.post('/api/coachChatMsgs', {
        text: msgInfo.text,
        chatID: context.state.coachChatID,
        userID: context.state.user.id, //msgInfo.userID
      }).then(response => {
        //possibly call getChatMsgs from
        return true;
      }).catch(err => {
        console.log("error from addChatMsg: " + err);
      });
    },

    getCoachChatMsgs(context){
      axios.get('/api/coachChatMsgs/' + context.state.coachChatID).then(response => {
        context.commit('setCoachChatMsgs',response.data.messages);
        console.log('STORE getCoachChatMsgs: ' + response.data.messages)
        return true;
      }).catch(err => {
        console.log("STORE: getCoachChatMsgs: " + err);
      });
    },

    getCoachChatMsgsSize(context){
      axios.get('/api/coachChatMsgs/' + context.state.coachChatID).then(response => {
        //Check the format of this.
        return response.messages.length;

      }).catch(err => {
        console.log("STORE: getCoachChatMsgsSize: " + err);
      });
    },


//END JSPENCER CHAT STUFF

    getMatrix(context, matrixID){
      axios.get("/api/matrix/" + matrixID).then(response => {
        let data= response.data.matrix[0];
        let mx=data.matrix;

        let type= data.type;
        let dimensions= type.split('x');
        for(let i=0; i<dimensions.length;i++)
        {
          dimensions[i]=parseInt(dimensions[i]);
        }
        let rows=dimensions[0];
        let cols=dimensions[1];
                //extract values
        let index=0;
        let temparray=[]
        while(index<mx.length)
        {

          if(isNaN(mx[index]))
          {
            index++;
          }
          else{

            let temp=index;

            while(!isNaN(mx[temp]))
            {
              temp++;
            }
            let k=parseInt(mx.substring(index,temp));

            index=temp;
            temparray.push(k);

          }
        }
        let matrix=[]
        //Initialize matrix
        let arrayIndex=0;
        for(let y =0; y<rows;y++)
        {
          let row=[]
          for(let x=0; x<cols;x++)
          {
            let option=[]
            for(let i=0;i<2;i++)
            {
              option.push(temparray[arrayIndex]);
              arrayIndex++;
            }
            row.push(option);
          }
          matrix.push(row);
        }
        context.commit('setMatrix', matrix);
      }).catch(err => {
        console.log("getMatrix Failed:", err);

      });
    },

  }
});
