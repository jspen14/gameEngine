const express = require('express');
const bodyParser = require('body-parser');

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));

// Knex Setup
const env = process.env.NODE_ENV || 'development';
const config = require('./knexfile')[env];
const knex = require('knex')(config);
// bcrypt setup
let bcrypt = require('bcrypt');
const saltRounds = 10;

// jwt setup
const jwt = require('jsonwebtoken');
let jwtSecret = process.env.jwtSecret;
if (jwtSecret === undefined) {
  console.log("You need to define a jwtSecret environment variable to continue.");
  knex.destroy();
  process.exit();
}

//------------Game--------------------------
class Game {
  constructor(gameID, player1, player2){
    this._gameID = gameID;
    this._player1 = player1;
    this._player2 = player2;
    this._p1Choice= null;
    this._p2Choice= null;
    this._currentRound = 0;
    //this probably needs to change
    this.numberOfRounds=1;
    this.rounds=[];
    this.roundEarnings=0;
    this.averageEarnings=0;
    this.totalEarnings=0;
    //roundOver()/Both players submitted?
    //show results
    //submitChoice
    //createRounds
    //recordRounds
    //nextGame
    //EndGame
    //Checking function that updates every x seconds in store

  }
  //Getters
  get round() {
    return this._round;
  }
  get player1() {
    return this._player1;
  }
  get player2() {
    return this._player2;
  }
  get gameID() {
    return this._gameID;
  }
  get nextRound(){
    if(this._currentRound<this.numberOfRounds)
    {
      return this._currentRound+1;
    }
  }
  get p1Choice(){
    return this._p1Choice;
  }
  get p2Choice(){
    return this._p2Choice;
  }
  //Setters
  set gameID(newgameID){
    this._gameID= newgameID;
  }
  set player1(player1){
    this._player1=player1;
  }
  set player2(player2){
    this._player2=player2;
  }
  set p1Choice(choice1){
    this._p1Choice=choice1;
  }
  set p2Choice(choice2){
    this._p2Choice=choice2;
  }
  bothSubmitted()
  {
    
    if (this.p1Choice!==null && this.p2Choice!==null)
      { 
        return true;
      }
    else 
      return true;
    
  }
  amISubmitted(whichUser)
  {
    if(whichUser===0)
    {
      if(p1Choice!==null)
        return true;
    }
    else
    {
      if(p2Choice!==null)
        return true;
    }
    return false;
  }
  isPlayer1(userID)
  {
    if(userID==this.player1)
      return true;
    return false;
  }
  calcEarnings()
  {
    //fix this to pull from matrix
    this.roundEarnings= 100;
    this.averageEarnings= roundEarnings/(this.currentRound+1);
    this.totalEarnings +=roundEarnings;
  }
// createRounds()
  completeRound(){
    //report results
    this.calcEarnings();
    //increment round
    this.round +=1;
    p1Choice=null;
    p2Choice=null;
    //figure end game
  }

}

//--------------------------------------------
//include game.js
//const gamejs=require('./game_model.js');
// Data
let currentGames = [];
//refers to game_model.js
let gameModels= [];

let availableUsers = [{role: "Coach", userID: 17, name: "Joe"}, {role: "Coach", userID: 18, name: "Jon"},
                      {role: "Player", userID: 19, name: "Josh"}, {role: "Player", userID: 20, name: "Chad"}];

// Endpoint Functions
app.get('/api/availableUsers',(req,res) => {

  res.send(availableUsers);
});

app.get('/api/currentGames',(req,res) =>{
  res.send(currentGames);
});

app.post('/api/inGameStatus', (req,res)=>{
  let user=req.body;
  let id = user.id;

 // pick up from here tomorrow ... get the ID from the current game and send it back to the store
  if(userIsInAvaiablePlayers(user)===false)
  {
    availableUsers.push(user);
  }
  for (let i = 0; i < currentGames.length; i++){
    if (id == currentGames[i].player1ID || id == currentGames[i].coach1ID)
    {
      res.status(200).json({inGameStatus: true, gameID: currentGames[i].gameID, which:0});
      return;
        }
    if(id == currentGames[i].player2ID || id == currentGames[i].coach2ID)
    {
      res.status(200).json({inGameStatus: true, gameID: currentGames[i].gameID, which:1});
      return;
    }
  }
  res.status(200).json({inGameStatus: false});
  return;
});


function userIsInAvaiablePlayers(user){
  let x;
  for(x=0;x<availableUsers.length;x++)
  {
    if(user.id==availableUsers[x].id)
    {
      return true;
    }
  }
  return false;
}
const verifyToken = (req, res, next) => {
  const token = req.headers['authorization'];
  if (!token)
    return res.status(403).send({ error: 'No token provided.' });
  jwt.verify(token, jwtSecret, function(err, decoded) {
    if (err)
      return res.status(500).send({ error: 'Failed to authenticate token.' });
    // if everything good, save to request for use in other routes
    req.userID = decoded.id;
    next();
  });
}
const getGameIndex = (gameID) =>{
  return gameModels.findIndex(x=> x.gameID ==gameID);
}

app.get('/api/coachChat/',(req,res) => {
  let id = parseInt(req.params.playerID);
  res.send([{role: "coach", text: "Hi"},{role: "player", text: "How are you?"},{role: "coach", text: "I am doing well, thanks! How are you?"},
                    {role: "player", text: "Great. Just ready to win some money!"},{role: "player", text: "Can you help me with that?"},
                    {role: "coach", text: "I sure can!"},{role: "player", text: "Perfect. Just tell me what I should do."},
                    {role: "coach", text: "Okay. Let's start out by playing our personal best. Choose X"},{role: "player", text: "Okay"}]);
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

app.get('/api/matrix/:id', (req,res)=> {
  let id=parseInt(req.params.id);
  knex('matrices').where('id',id).select('type', 'matrix').then(matrix => {
    res.status(200).json({matrix:matrix});
  }).catch(error =>{
    res.status(500).json({error});
  });
});
app.get('/api/gameState/:id/:which',(req,res)=>{
  let id =parseInt(req.params.id);
  let index=getGameIndex(id);
  let which=parseInt(req.params.id);
  console.log("gameSTATE:")
  console.log("id: ",id);
  console.log("index: ",index);
  console.log("which: ",which);
  console.log("----")
  if(gameModels[index].bothSubmitted())
  {
    res.status(200).json({gameState:'done',
                          roundEarnings: gameModels[index].roundEarnings, 
                          averageEarnings: gameModels[index].averageEarnings,
                          totalEarnings: gameModels[index].totalEarnings,
                          nextRound: gameModels[index].nextRound,
                          })
  }
  else if(gameModels[index].amISubmitted(which))
    res.status(200).json({gameState:'submitted'});
  else
    res.status(200).json({gameState:'unsubmitted'});

});
app.post('/api/createGame', (req,res) =>{
 // We need to have the player IDs
  console.log("in createGame on server: " + req.body);
  console.log(req.body.player1ID, req.body.player2ID, req.body.coach1ID, req.body.coach2ID)
  return knex('games').insert({player1ID:req.body.player1ID, coach1ID:req.body.coach1ID, player2ID:req.body.player2ID, coach2ID:req.body.coach2ID})
    .then(ids => {
      // Put game into currentGames array
      let gID=parseInt(ids[0]);
      let p1ID=parseInt(req.body.player1ID);
      let p2ID=parseInt(req.body.player2ID);
      let game = {currentRound:0, 
                  gameID:gID, 
                  player1ID:p1ID, 
                  coach1ID:parseInt(req.body.coach1ID),
                  player2ID: p2ID, 
                  coach2ID:parseInt(req.body.coach2ID)
                };
      currentGames.push(game);
      console.log("NEW GAME: ", gID,p1ID,p2ID);
      let newGame= new Game(gID,p1ID,p2ID);
      gameModels.push(newGame);
      // Send gameID to admin so he can view game progress
      knex('games').where({id: ids[0]}).first();
      res.status(200).json({gameID:ids[0]});
    }).catch(error => {
      console.log(error);
      res.status(500).json({error})
    });
});
//submit Choice
app.post('/api/game',(req,res)=>
{
  let gameID=parseInt(req.body.game);
  let which=parseInt(req.body.which);
  let choice=parseInt(req.body.choice);
  console.log('----------------------------');
  console.log('gameID: ',gameID);
  console.log("choice: ",choice);
  //round is not being used
  let round = parseInt(req.body.round);
  let index = getGameIndex(gameID);
  console.log("which: ",which);
  if(gameModels[index]==null)
  {
    res.status(400).send("bad request!/game was not found!")
  }
  if(which===0)
  {
    gameModels[index].p1Choice=choice;
    console.log('1 p1Choice: ', gameModels[index].p1Choice)
    console.log('2 p2Choice: ', gameModels[index].p2Choice)
    res.status(200).send("success");
  }
  else if(which ===1)
  {
    gameModels[index].p2Choice=choice;
    console.log('3 p1Choice: ', gameModels[index].p1Choice)
    console.log('4 p2Choice: ', gameModels[index].p2Choice)
    res.status(200).send("success");
  }
  else{
    console.log("error in submit choice server side");
    res.status(500).send('failure');

  }
});


// Login
app.post('/api/login', (req, res) => {

  if (!req.body.name || !req.body.password)
    return res.status(400).send();
  knex('users').where('name',req.body.name).first().then(user => {
    if (user === undefined) {
      res.status(403).send("Invalid credentials");
      throw new Error('abort');
    }
    return [bcrypt.compare(req.body.password, user.hash),user];
  }).spread((result,user) => {
        if (result) {
       let token = jwt.sign({ id: user.id }, jwtSecret, {
        expiresIn: 86400 // expires in 24 hours
       });

       //Add to availableUsers here
      availableUsers.push(user);

      res.status(200).json({user:{name:user.name,id:user.id,role:user.role},token:token});
    }
    else
    {
      res.status(403).send("Invalid credentials");
    }
    return;
  }).catch(error => {
    if (error.message !== 'abort') {
      console.log(error);
      res.status(500).json({ error });
    }
  });
});

//Register
app.post('/api/users', (req, res) => {
if ( !req.body.password || !req.body.name)
    return res.status(400).send();
knex('users').where('name',req.body.name).first().then(user => {
    console.log(user);
    if (user !== undefined) {
      res.status(409).send("User name already exists");
      throw new Error('abort');
    }
    return bcrypt.hash(req.body.password, saltRounds);
  }).then(hash => {
    return knex('users').insert({hash: hash, role:req.body.role,
         name:req.body.name});
  }).then(ids => {
    return knex('users').where('id',ids[0]).first().select('name','id','role');
  }).then(user => {
    let token = jwt.sign({ id: user.id }, jwtSecret, {
      expiresIn: 86400 // expires in 24 hours
    });
    availableUsers.push(user);
    res.status(200).json({user:user,token:token});
    return;
  }).catch(error => {
    if (error.message !== 'abort') {
      console.log(error);
      res.status(500).json({ error });
    }
  });
});
app.get('/api/me', verifyToken, (req,res) => {
  knex('users').where('id',req.userID).first().select('name','id','role').then(user => {
    res.status(200).json({user:user});
  }).catch(error => {
    res.status(500).json({ error });
  });
});


app.listen(3000, () => console.log("Server listening on port 3000!"));
