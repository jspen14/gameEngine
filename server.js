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


app.get('/api/roundEarnings',(req,res) => {
  res.send(playerOneRoundEarnings);
});

app.get('/api/totalEarnings',(req,res) => {
  res.send(playerOneTotalEarnings);
});

app.get('/api/averageEarnings',(req,res) => {
  res.send(playerOneAverageEarnings);
});

app.get('/api/roundOption',(req,res) => {
  res.send(playerOneOption);
});

app.get('/api/submissionStatus',(req,res) => {
  res.send(playerOneStatus);
});

app.post('/api/roundOption', (req,res) => {
  console.log("in roundOption");
  if (playerOneStatus == "open"){
    playerOneOption = req.body.roundOption;
    playerOneStatus = "closed";
    if (playerOneStatus == "closed" && playerTwoStatus == "closed"){
      computeRoundOutcome();
    }
  }
  res.send(playerOneStatus);
});

app.get('/api/coachChat/:playerID',(req,res) => {
  let id= parseInt(req.params.playerID);
  knex('game')
  res.send(chatFeedOne);
});

app.post('/api/coachChat', (req,res) => {
  msgId = msgId + 1;
  let chatMsg = {id:msgIdOne, text:req.body.text, role:req.body.role};//include when created
  chatFeedOne.unshift(chatMsg);
  res.send(chatMsg);
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

app.get('/api/matrix/:id', (req,res)=> {
  let id=parseInt(req.params.id);
  console.log(id);
  knex('matrices').where('id',id).select('type', 'matrix').then(matrix => {
    res.status(200).json({matrix:matrix});
  }).catch(error =>{
    res.status(500).json({error});
  });
 
});

// Non-Endpoint Functions
function computeRoundOutcome(){
  console.log("in computeRoundOutcome");

  if(playerOneOption == "1" && playerTwoOption == "1"){
    playerOneRoundEarnings = ".10";
    playerTwoRoundEarnings = ".10";
  }
  else if(playerOneOption == "1" && playerTwoOption == "2"){
    playerOneRoundEarnings = "-.02";
    playerTwoRoundEarnings = ".07";
  }
  else if(playerOneOption == "2" && playerTwoOption == "1"){
    playerOneRoundEarnings = ".07";
    playerTwoRoundEarnings = "-.02";
  }
  else{
    playerOneRoundEarnings = ".05";
    playerTwoRoundEarnings = ".05";
  }

  playerOneTotalEarnings = computeTotalEarnings(playerOneRoundEarnings, playerOneTotalEarnings);
  playerTwoTotalEarnings = computeTotalEarnings(playerTwoRoundEarnings, playerTwoTotalEarnings);

  playerOneAverageEarnings = computeAverageEarnings(roundNumber, playerOneTotalEarnings);
  playerTwoAverageEarnings = computeAverageEarnings(roundNumber, playerTwoTotalEarnings);

  playerOneStatus = "open";
  playerTwoStatus = "open";
  roundNumber++;
}

function computeTotalEarnings(roundEarnings, totalEarnings){

  totalEarnings = parseFloat(totalEarnings) + parseFloat(roundEarnings);
  return (totalEarnings.toFixed(2)).toString();
}

function computeAverageEarnings(roundNumber, totalEarnings){
  console.log("AE" + ((parseFloat(totalEarnings)/roundNumber).toFixed(2)).toString());
  return ((parseFloat(totalEarnings)/roundNumber).toFixed(2)).toString();
}

app.listen(3000, () => console.log("Server listening on port 3000!"));
