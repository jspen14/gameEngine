const express = require('express');
const bodyParser = require('body-parser');

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));

app.use(express.static('public')); // I'm not sure exactly what this is supposed to do.

// Knex Setup
const env = process.env.NODE_ENV || 'development';
const config = require('./knexfile')[env];
const knex = require('knex')(config);

// Data
let currentGames = [];

// Endpoint Functions
app.get('/api/currentGames', (req,res) =>{
  console.log(currentGames);
  res.send(currentGames);
});

app.get('/api/roundEarnings:gameID',(req,res) => {
  res.send("rE");
});

app.get('/api/totalEarnings',(req,res) => {
  res.send("tE");
});

app.get('/api/averageEarnings',(req,res) => {
  res.send("aE");
});

app.get('/api/roundOption',(req,res) => {
  res.send("rO-GET");
});

app.get('/api/submissionStatus',(req,res) => {
  res.send('sS');
});

app.post('/api/roundOption', (req,res) => {
  console.log("in roundOption");
  res.send('rO-POST');
});

app.get('/api/coachChat/',(req,res) => {
  let id = parseInt(req.params.playerID);
  res.send([{role: "coach", text: "Hi"},{role: "player", text: "How are you?"},{role: "coach", text: "I am doing well, thanks! How are you?"},
                    {role: "player", text: "Great. Just ready to win some money!"},{role: "player", text: "Can you help me with that?"},
                    {role: "coach", text: "I sure can!"},{role: "player", text: "Perfect. Just tell me what I should do."},
                    {role: "coach", text: "Okay. Let's start out by playing our personal best. Choose X"},{role: "player", text: "Okay"}]);
});

app.post('/api/userRegister',(req,res) => {
  return knex('users').insert({role: req.body.role, coachType: req.body.coachType, name: req.body.name})
    .then(ids => {
      console.log(ids[0]);
      knex('users').where({id: ids[0]}).first();
      res.status(200).json({playerID:ids[0]});
    }).catch(error => {
      console.log(error);
      res.status(500).json({error})
    });
});

app.post('/api/coachChat', (req,res) => {
  // Use unshift to put messages at the top instead of the bottom

  knex('users').insert({role: "player", coachType: "N/A"})
    .catch(error => {
      console.log(error);
      res.status(500).json({error});
    });

  res.send([]);
});

app.post('/api/createGame', (req,res) =>{
 // We need to have the player IDs
  return knex('games').insert({player1ID:req.body.player1ID, coach1ID:req.body.coach1ID, player2ID:req.body.player2ID, coach2ID:req.body.coach2ID})
    .then(ids => {
      // Put game into currentGames array
      let game = {roundNum:0, gameID:ids[0], player1:req.body.player1ID, coach1:req.body.coach1ID,
                  player2:req.body.player2ID, coach2:req.body.coach2ID};
      currentGames.push(game);
      // Send gameID to admin so he can view game progress
      knex('games').where({id: ids[0]}).first();
      res.status(200).json({gameID:ids[0]});
    }).catch(error => {
      console.log(error);
      res.status(500).json({error})
    });
});
//Clears and Genterates random payouts
app.post('/api/payouts',(req,res) =>{
  console.log("Payouts");
  let i;
  //clears payouts
  p1Payouts=[];
  p2Payouts=[];
  //genterates new semetric values for payouts
  for(i=0;i<4;i++ ){
    let p1randInt=Math.floor(Math.random()*10);
    let p2randInt=Math.floor(Math.random()*10);
    if(i==2){
      p1Payouts.push(p1Payouts[1]);
      p2Payouts.push(p2Payouts[1]);
    }
    else{
      p1Payouts.push(p1randInt);
      p2Payouts.push(p2randInt);
    }
  }
  console.log([p1Payouts,p2Payouts]);
  res.send([p1Payouts,p2Payouts]);
});
//Gets Payouts for P1
app.get('/api/payouts/1', (req,res)=> {
  res.send(p1Payouts);
});
//Gets Payouts for P2
app.get('/api/payouts/2', (req,res)=>{
  res.send(p2Payouts);
});

// Non-Endpoint Functions
function computeRoundOutcome(){
  console.log("in computeRoundOutcome");

}

function computeTotalEarnings(roundEarnings, totalEarnings){

}

function computeAverageEarnings(roundNumber, totalEarnings){

}

app.listen(3000, () => console.log("Server listening on port 3000!"));
