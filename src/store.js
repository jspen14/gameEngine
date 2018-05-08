import Vue from 'vue';
import Vuex from 'vuex';
import axios from 'axios';

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    roundID: 1,
    gameID: 1,
    userID: 1,
    name: '',
    role: '',
    inGame: 'false',
    coachID: 2,
    roundOption: '',
    submissionStatus: '',
    roundEarnings: '',
    averageEarnings: '',
    totalEarnings: '',
    //TODO think about how to show the winningBox
    matrix: [],
    coachChatMsgs: [],
    playerChatMsgs: [],
  },
  getters: {
    roundID: state => state.roundID,
    gameID: state => state.gameID,
    userID: state => state.userID,
    name: state => state.name,
    role: state => state.role,
    inGame: state => state.inGame,
    coachID: state => state.coachID,
    roundOption: state => state.roundOption,
    submissionStatus: state => state.submissionStatus,
    roundEarnings: state => state.roundEarnings,
    averageEarnings: state => state.averageEarnings,
    totalEarnings: state => state.totalEarnings,
    //TODO put in a getter for the winningBox variable
    matrix: state => state.matrix,
    coachChatMsgs: state => state.coachChatMsgs,
    playerChatMsgs: state => state.playerChatMsgs,
    rows: state => state.rows,
    cols: state => state.cols,

  },
  mutations: {
    setRoundID (state, roundID){
      state.roundID = roundID;
    },
    setGameID (state, gameID){
      state.gameID = gameID;
    },
    setUserID (state, userID){
      console.log("in setUserID " + userID.userID);
      state.userID = userID;
    },
    setName (state, name){
      state.name = name;
    },
    setRole (state, role){
      state.role = role;
    },
    setInGame (state, inGame){
      state.inGame = inGame;
    },
    setCoachID (state, coachID){
      state.coachID = coachID
    },
    setRoundOption (state, roundOption){
      state.roundOption = roundOption;
    },
    setSubmissionStatus (state, submissionStatus){
      state.submissionStatus = submissionStatus;
    },
    setRoundEarnings (state, roundEarnings){
      state.roundEarnings = roundEarnings;
    },
    setAverageEarnings (state, averageEarnings){
      state.averageEarnings = averageEarnings;
    },
    setTotalEarnings (state, totalEarnings){
      state.totalEarnings = totalEarnings;
    },
    //TODO mutator for winningBox
    setMatrix (state, matrix){
      state.matrix = matrix;
    },
    setCoachChatMsgs (state, coachChatMsgs){
      state.coachChatMsgs = coachChatMsgs;
    },
    setPlayerChatMsgs (state, playerChatMsgs){
      state.playerChatMsgs = playerChatMsgs;
    },
    setRows (state, rows){
      state.rows = rows;
    },
    setCols (state, cols){
      state.cols = cols;
    }

  },
  actions: {
    updateData(context){
      let timerID = setInterval(() => {
        // This will allow me to make regular calls to the server to check things
        console.log("in updateData in store" + context.state.userID);
        axios.get('/api/inGame/' + context.state.userID)
      }, 1500);
    },

    register(context,info){
          context.commit('setName', info.name);
        axios.post('/api/userRegister',info).then(response => {
          context.commit('setUserID',response.data);
        }).catch(error => {
          console.log(error)
          context.commit('setUserID', '');
          context.commit('setRole','');
        });
    },

    addChatMsg(context,post) {
      axios.post("/api/users/" + context.state.user.user_id + "/posts",post).then(response => {
      	return context.dispatch('getPosts');
      }).catch(err => {
      	console.log("addPost failed:",err);
      });
    },

    getPosts(context){
      console.log("Am i in the store getPosts?");
      return axios.get('/api/posts').then(response => {

        context.commit('setFeed',response.data.posts);
      }).catch(err => {
        console.log("getFeed Failed: ", err);
      });
    },
    //matrixID is temporary will grab the game from the round
    submitChoice(context, choice)
    {
      console.log("roundID: ", context.state.roundID);
      console.log("userID: ", context.state.userID);
      axios.post("/api/round/"+context.state.roundID+'/'+context.state.userID, choice).then(response =>{
        console.log(response.data);
      }).catch(err => {
        console.log("submitChoice Failed: ", err);
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
