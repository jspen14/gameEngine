import Vue from 'vue';
import Vuex from 'vuex';
import axios from 'axios';

Vue.use(Vuex);

const getAuthHeader = () => {
   return { headers: {'Authorization': localStorage.getItem('token')}};
 }

export default new Vuex.Store({
  state: {

    inGameStatus: false,
    user: {},
    loginError: '',
    registerError: '',
    token: '',
    currentRound: 1,
    currentGame: '',
    p1Choice: '',
    p2Choice: '',
    gameState: 'unsubmitted',
    matrix: [],
    coachChatMsgs: [],
    playerChatMsgs: [],
    whichPlayer: null,
    numberOfRounds: '',
    roundEarnings: '-',
    totalEarnings: 0,
    coachChatID: '',
    partnerChatID: '',
    partnerChatMsgs: [],
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
    playerChatMsgs: state => state.playerChatMsgs,
    gameState: state =>state.gameState,
    p1Choice: state =>state.p1Choice,
    p2Choice: state =>state.p2Choice,
    numberOfRounds: state =>state.numberOfRounds,
    partnerChatMsgs: state => state.partnerChatMsgs,


    //added for registration
    getToken: state => state.token,
    loggedIn: state => {
      if (state.token === '' || state.user==={})
       return false;
      return true;
    },
    loginError: state => state.loginError,
    registerError: state => state.registerError,

    whichPlayer: state => state.whichPlayer,
    roundEarnings: state => state.roundEarnings,
    totalEarnings: state => state.totalEarnings,

  },
  mutations: {

    setCurrentGame (state, gameID){
      state.currentGame=gameID;
    },
    setCurrentRound(state, nextRound){
      state.currentRound=nextRound;
    },

    setInGameStatus (state, inGameStatus){
      state.inGameStatus = inGameStatus;
    },
    
    setGameState(state, gameState){
      state.gameState =gameState;
    },
    setCoachID (state, coachID){
      state.coachID = coachID
    },
    setCoachChatID (state, coachChatID){
      state.coachChatID = coachChatID;
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
    setWhichPlayer(state, which){
      state.whichPlayer=which;
    },
    setP1Choice(state, choice){
      state.p1Choice=choice;
    },    
    setP2Choice(state, choice){
      state.p2Choice=choice;
    },
    setNumberOfRounds(state,number){
      state.numberOfRounds=number;
    },
    setRoundEarnings(state, money){
      state.roundEarnings=money;
    },
    setTotalEarnings(state,money){
      state.totalEarnings=money;
    }

  },
  actions: {

    updateData(context){
      var updateDataTimer = setInterval(() => {
        //console.log("in updateData in store: " + context.state.userID.userID);
        axios.post('/api/inGameStatus/', context.state.user).then(response => {
        context.commit('setInGameStatus',response.data.inGameStatus);

        context.commit('setCurrentGame', response.data.gameID);
        context.commit('setWhichPlayer', response.data.which);
        if(context.state.inGameStatus===true)
        {
          context.dispatch('getNumberOfRounds');
          context.dispatch('getMatrix',context.state.currentRound);
          context.commit('setGameState','unsubmitted');
          clearInterval(updateDataTimer);
          context.dispatch('updateGame');
        }
      });

      }, 1500);
    },
    //needs to update game
    updateGame(context){
      let timerID = setInterval(() => {
      axios.get('/api/gameState/'+context.state.currentGame+'/'+context.state.whichPlayer).then(response => {
      context.commit('setGameState',response.data.gameState);
      if(context.state.gameState==='done')
      {
        clearInterval(timerID);
        context.commit('setP1Choice', response.data.p1Choice);
        context.commit('setP2Choice', response.data.p2Choice);
        if(context.state.whichPlayer===0)
            context.commit('setRoundEarnings', context.state.matrix[context.state.p1Choice][context.state.p2Choice][0]); 
        else
          context.commit('setRoundEarnings', context.state.matrix[context.state.p1Choice][context.state.p2Choice][1]);
        axios.get('/api/totalEarnings/'+context.state.currentGame+'/'+context.state.whichPlayer).then(response =>{

          console.log("RESPONSE: ", response.data);
          context.commit('setTotalEarnings', response.data); 
        }).catch(error=>{
          console.log(error);
        });
         
        console.log("setTotalEarnings to: ", context.state.totalEarnings);
        let roundInfo={
          gameID: context.state.currentGame,
          currentRound: context.state.currentRound,
          p1Choice: context.state.p1Choice,
          p2Choice: context.state.p2Choice,
          p1Earnings:context.state.matrix[context.state.p1Choice][context.state.p2Choice][0],
          p2Earnings:context.state.matrix[context.state.p1Choice][context.state.p2Choice][1],
        }
        axios.post('/api/round',roundInfo).then(response =>{
          console.log(response.data);
        });

      }
      });
      }, 1000);
    },
  // Initialize //
    initialize(context) {
      let token = localStorage.getItem('token');
      if (token) {
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

  whichPlayer(context)
  {
    console.log("api/game/"+context.state.currentGame+'/'+context.state.user.id);
    let whichplayer=axios.get("/api/game/"+context.state.currentGame+'/'+context.state.user.id);
    if (whichplayer!==null)
    {
      context.commit('setWhichPlayer',whichPlayer-1);
    }
  },
  // Registration, Login //


  login(context,user) {
    return axios.post("/api/login",user).then(response => {
      context.commit('setUser', response.data.user);
      context.commit('setToken',response.data.token);
      context.commit('setRegisterError',"");
      context.commit('setLoginError',"");
      })
    .catch(error => {
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

    logout(context) {
        context.commit('setInGameStatus', false);
        context.commit('setUser', {});//originial logout
        context.commit('setToken','');//*
        
        context.commit('setCurrentRound',1);
        context.commit('setCurrentGame','');
        context.commit('setP1Choice','');
        context.commit('setP2Choice','');
        context.commit('setGameState','unsubmitted');
        context.commit('setMatrix',[]);
        context.commit('setCoachChatMsgs',[]);
        context.commit('setPlayerChatMsgs',[]);
        context.commit('setWhichPlayer', null);
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
        endGame(context){
      axios.delete('/api/game/'+context.state.currentGame).then(response =>{
        context.dispatch('logout')}).catch(err=>{console.log("endGame error")});
    },

    
    submitChoice(context, choice){
      
      let choiceInfo = {round: context.state.currentRound,
        which: context.state.whichPlayer,
        choice: choice,
        game: context.state.currentGame,
      }
      axios.post("/api/game",choiceInfo).then(response =>
      {
        console.log(response.data);
        context.commit('setGameState','submitted');

      }).catch(err =>{
        console.log("submitChoice failed: ", err);
      });
      
    },
    readyForNextRound(context){
      let readyInfo={gameID:context.state.currentGame, which: context.state.whichPlayer}
      console.log(readyInfo);

      axios.post("/api/ready", readyInfo).then(response =>{
        context.commit('setGameState','isReady');
        context.dispatch('nextRound');
      }).catch(err=>console.log(err));
    },
    nextRound(context)
    {
      let timerID = setInterval(() => {
        axios.get("/api/ready/"+context.state.currentGame)
        .then(response => {
          let newRound=response.data.currentRound;

          if(newRound>context.state.numberOfRounds)
          {
            context.commit('setGameState','endGame');
            clearInterval(timerID);
          }
          else if(newRound>context.state.currentRound)
          {
            clearInterval(timerID);
            console.log("got to nextRound");
            context.commit('setCurrentRound', newRound);
            context.commit('setP1Choice', null);
            context.commit('setP2Choice',null);
            context.commit('setGameState','unsubmitted');
            context.dispatch('getMatrix',newRound);
            context.commit('setRoundEarnings','-');
            context.dispatch('updateGame');
          }

        }).catch(err =>{
          console.log("nextRound Error: ",err);
        })
      },1000);
    },
    getNumberOfRounds(context){
      axios.get("/api/numberOfRounds/"+context.state.currentGame)
      .then(response =>{
        context.commit('setNumberOfRounds', response.data.numberOfRounds)})
      .catch(err =>{
        console.log("getNumberOfRounds failed: ",err);
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
