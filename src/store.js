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
    }
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

  }
});
