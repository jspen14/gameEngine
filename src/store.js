import Vue from 'vue';
import Vuex from 'vuex';

import axios from 'axios';

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    roundID: '',
    gameID: '',
    playerID: '',
    coachID: '',
    roundOption: '',
    submissionStatus: '',
    roundEarnings: '',
    averageEarnings: '',
    totalEarnings: '',
    //TODO think about how to show the winningBox
    matrix: [],
    role: '',
    coachChatMsgs: [],
    playerChatMsgs: [],

  },
  getters: {
    roundID: state => state.roundID,
    gameID: state => state.gameID,
    playerID: state => state.playerID,
    coachID: state => state.coachID,
    roundOption: state => state.roundOption,
    submissionStatus: state => state.submissionStatus,
    roundEarnings: state => state.roundEarnings,
    averageEarnings: state => state.averageEarnings,
    totalEarnings: state => state.totalEarnings,
    //TODO put in a getter for the winningBox variable
    matrix: state => state.matrix,
    role: state => state.role,
    coachChatMsgs: state => state.coachChatMsgs,
    playerChatMsgs: state => state.playerChatMsgs,

  },
  mutations: {
    setRoundID (state, roundID){
      state.roundID = roundID;
    },
    setGameID (state, gameID){
      state.gameID = gameID;
    },
    setPlayerID (state, playerID){
      state.playerID = playerID;
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
    setRole (state, role){
      state.role = role;
    },
    setCoachChatMsgs (state, coachChatMsgs){
      state.coachChatMsgs = coachChatMsgs;
    },
    setPlayerChatMsgs (state, playerChatMsgs){
      state.playerChatMsgs = playerChatMsgs;
    },

  },
  actions: {

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
    getMatrix(context, matrixID){
      axios.get("/api/matrix/" + matrixID).then(response => {
        console.log("matrix");
        let data= response.data.matrix[0];
        let mx=data.matrix;

        let type= data.type;
        console.log(mx);
        console.log(type);
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
        //console.log(mx.length);
        while(index<mx.length)
        {

          if(isNaN(mx[index]))
          {
            index++;
          }
          else{
            
            let temp=index;
            console.log(mx[index]);
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


        console.log(temparray);
        console.log(matrix);
        console.log("done");
        context.commit('setMatrix', matrix);
      }).catch(err => {
        console.log("getMatrix Failed:", err);

      });
    },

  }
});
