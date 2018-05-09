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
    user: {},
    loginError: '',
    registerError: '',
    token: '',
    currentRound: 1,
    currentGame: '',
    roundOption: '',
    matrix: [],
    coachChatMsgs: [],
    playerChatMsgs: [],
  },
  getters: {
    inGameStatus: state => state.inGameStatus,
    currentRound: state=> state.currentRound,
    currentGame: state=> state.currentGame,
    user: state=>state.user,
    playerID: state => state.user.id,
    name: state => state.user.name,
    matrix: state => state.matrix,
    coachChatMsgs: state => state.coachChatMsgs,
    playerChatMsgs: state => state.playerChatMsgs,

    //added for registration
    user: state => state.user,
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
    setCoachID (state, coachID){
      state.coachID = coachID
    },
    setRoundOption (state, roundOption){
      state.roundOption = roundOption;
    },
    setMatrix (state, matrix){
      state.matrix = matrix;
    },
    setCoachChatMsgs (state, coachChatMsgs){
      state.coachChatMsgs = coachChatMsgs;
    },
    setPlayerChatMsgs (state, playerChatMsgs){
      state.playerChatMsgs = playerChatMsgs;
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
        //console.log("in updateData in store: " + context.state.userID.userID);
        axios.get('/api/inGameStatus/' + context.state.user.id).then(response => {
          console.log(response.data.gameID + " - " + response.data.inGameStatus);
          if(response.data.inGameStatus == true){
            console.log("one small step for man");
          }
          context.commit('setInGameStatus',response.data.inGameStatus);

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
      console.log(user);
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

    addChatMsg(context,post) {
      axios.post("/api/users/" + context.state.user.user_id + "/posts",post).then(response => {
      	return context.dispatch('getPosts');
      }).catch(err => {
      	console.log("addPost failed:",err);
      });
    },


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
