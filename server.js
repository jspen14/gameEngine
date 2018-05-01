const express = require('express');
const bodyParser = require('body-parser');

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));

app.use(express.static('public')); // I'm not sure exactly what this is supposed to do.

// Shared Variables
let roundNumber = 1;

//C1P1 Communication API Endpoints
  let chatFeedOne = [];
  let msgIdOne = 0;
  let playerOneStatus = "open";
  let playerOneOption = '1';
  let playerOneRoundEarnings = '0';
  let playerOneTotalEarnings = '0';
  let playerOneAverageEarnings = '0';
  let p1Payouts=[];
  let p2Payouts=[];

app.get('/api/p1roundEarnings',(req,res) => {
  res.send(playerOneRoundEarnings);
  console.log("in p1roundEarningsround");
});

app.get('/api/p1totalEarnings',(req,res) => {
  res.send(playerOneTotalEarnings);
});

app.get('/api/p1averageEarnings',(req,res) => {
  res.send(playerOneAverageEarnings);
});

app.get('/api/p1roundOption',(req,res) => {
  res.send(playerOneOption);
});

app.get('/api/p1submissionStatus',(req,res) => {
  res.send(playerOneStatus);
});

app.put('/api/p1RoundOption', (req,res) => {
  console.log("in p1roundOption");
  if (playerOneStatus == "open"){
    playerOneOption = req.body.roundOption;
    playerOneStatus = "closed";
    if (playerOneStatus == "closed" && playerTwoStatus == "closed"){
      computeRoundOutcome();
    }
  }
});

app.get('/api/p1coachChat',(req,res) => {
  res.send(chatFeedOne);
});

app.post('/api/p1coachChat', (req,res) => {
  msgIdOne = msgIdOne + 1;
  let chatMsg = {id:msgIdOne, text:req.body.text, role:req.body.role};
  chatFeedOne.unshift(chatMsg);
  res.send(chatMsg);
});

//C2P2 Communication API Enpoints
  let chatFeedTwo = [];
  let msgIdTwo = 0;
  let playerTwoStatus = "closed";
  let playerTwoOption = '1';
  let playerTwoRoundEarnings = '0';
  let playerTwoTotalEarnings = '0';
  let playerTwoAverageEarnings = '0';

app.get('/api/p2roundEarnings',(req,res) => {
  res.send(playerTwoRoundEarnings);
});

app.get('/api/p2totalEarnings',(req,res) => {
  res.send(playerTwoTotalEarnings);
});

app.get('/api/p2averageEarnings',(req,res) => {
  res.send(playerTwoAverageEarnings);
});

app.get('/api/p2roundOption',(req,res) => {
  res.send(playerTwoOption);
});

app.get('/api/p2submissionStatus',(req,res) => {
  res.send(playerTwoStatus);
});

app.put('/api/p2RoundOption', (req,res) => {
  console.log("in p2roundOption");
  if (playerTwoStatus == "open"){
    playerTwoOption = req.body.roundOption;
    playerTwoStatus = "closed";
    if (playerOneStatus == "closed" && playerTwoStatus == "closed"){
      computeRoundOutcome();
    }
  }
});

app.get('/api/p2coachChat',(req,res) => {
  res.send(chatFeedTwo);
});

app.post('/api/p2coachChat', (req,res) => {
  msgIdTwo = msgIdTwo + 1;
  let chatMsg = {id:msgIdTwo, text:req.body.text, role:req.body.role};
  chatFeedTwo.unshift(chatMsg);
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
// Non-Endpoint Functions
function computeRoundOutcome(){
  console.log("in computeRoundOutcome");

  if(playerOneOption == "1" && playerTwoOption == "1"){
    playerOneRoundEarnings = ".1";
    playerTwoRoundEarnings = ".1";
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

  playerOneTotalEarnings = computeTotalEarnings(playerOneRoundEarnings, playerOneTotalEarnings).toString();
  playerTwoTotalEarnings = computeTotalEarnings(playerTwoRoundEarnings, playerTwoTotalEarnings).toString();

  playerOneAverageEarnings = computeAverageEarnings(roundNumber, playerOneTotalEarnings).toString();
  playerTwoAverageEarnings = computeAverageEarnings(roundNumber, playerTwoTotalEarnings).toString();

  playerOneStatus = "open";
  playerTwoStatus = "open";
  roundNumber++;
}

function computeTotalEarnings(roundEarnings, totalEarnings){

  totalEarnings = parseFloat(totalEarnings) + parseFloat(roundEarnings);
  return totalEarnings;
}

function computeAverageEarnings(roundNumber, totalEarnings){
  return parseFloat(totalEarnings)/roundNumber;
}

app.listen(3000, () => console.log("Server listening on port 3000!"));
