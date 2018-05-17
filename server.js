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
  constructor(gameID, player1, player2, coach1, coach2){
    this._gameID = gameID;
    this._player1 = player1;
    this._player2 = player2;
    this._coach1 = coach1;
    this._coach2 = coach2;
    this._p1Choice= null;
    this._p2Choice= null;
    this._p1Ready= false;
    this._p2Ready= false;
    this._currentRound = 1;
    //fix this later -- make it so number of rounds autimatically updates
    this._numberOfRounds=3;

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
  get coach1() {
    return this._coach1;
  }
  get coach2() {
    return this._coach2;
  }
  get gameID() {
    return this._gameID;
  }
  get currentRound(){
    return this._currentRound;
  }
  get p1Choice(){
    return this._p1Choice;
  }
  get p2Choice(){
    return this._p2Choice;
  }
  get numberOfRounds(){
    return this._numberOfRounds;
  }

  get p1Ready(){
    return this._p1Ready;
  }
  get p2Ready(){
    return this._p2Ready;
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
  set coach1(coach){
    this._coach1=coach;
  }
  set coach2(coach){
    this._coach2=coach;
  }

  set p1Choice(choice1){
    this._p1Choice=choice1;
  }
  set p2Choice(choice2){
    this._p2Choice=choice2;
  }
  set currentRound(round){
    this._currentRound=round;
  }
  set numberOfRounds(x){
    this._numberOfRounds=x;
  }

  set p1Ready(status){
    this._p1Ready=status;
  }
  set p2Ready(status){
    this._p2Ready=status;
  }
  bothSubmitted()
  {
    
    if (this.p1Choice===null || this.p2Choice===null)
      { 
        return false;
      }
    return true; 
  }
  bothReady(){
    if(this.p1Ready && this.p2Ready)
    {
      return true;
    }
    return false;
  }
  amISubmitted(whichUser)
  {
    if(whichUser===0)
    {
      if(this.p1Choice!==null)
        return true;
    }
    else
    {
      if(this.p2Choice!==null)
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


  goToNextRound(){
    this.p1Choice=null;
    this.p2Choice=null;
    this.p1Ready=false;
    this.p2Ready=false;
    this.currentRound+=1
  }
}

//--------------------------------------------

// Data

let gameModels= [];

let availableUsers = [{role: "Coach", id: 17, name: "Joe"}, {role: "Coach", id: 18, name: "Jon"},
                      {role: "Player", id: 19, name: "Josh"}, {role: "Player", id: 20, name: "Chad"}];

// Endpoint Functions
app.get('/api/availableUsers',(req,res) => {
  res.send(availableUsers);
});
app.post('/api/inGameStatus', (req,res)=>{
  let user=req.body;
  let id = user.id;

 // pick up from here tomorrow ... get the ID from the current game and send it back to the store
  if(userIsInAvaiablePlayers(id)===false)
  {
    availableUsers.push(user);
  }
  for (let i = 0; i < gameModels.length; i++){
    if (id == gameModels[i].player1 || id == gameModels[i].coach1)
    {
      res.status(200).json({inGameStatus: true, gameID: gameModels[i].gameID, which:0});
      return;
        }
    if(id == gameModels[i].player2 || id == gameModels[i].coach2)
    {
      res.status(200).json({inGameStatus: true, gameID: gameModels[i].gameID, which:1});
      return;
    }
  }
  res.status(200).json({inGameStatus: false});
  return;
});


function userIsInAvaiablePlayers(id){
  let x;
  for(x=0;x<availableUsers.length;x++)
  {
    if(id==availableUsers[x].id)
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
//TODO: if gameID and matrixID combo already exist don't insert into table...
app.post('/api/round', (req,res) =>{
return knex('rounds').select().where('gameID', req.body.gameID)
.andWhere('matrixID', req.body.currentRound)
.then(function(rows){
  if(rows.length===0)
  {
  knex('rounds').insert({gameID: req.body.gameID, matrixID:req.body.currentRound, 
  player1choice: req.body.p1Choice, player2choice: req.body.p2Choice, 
  p1Earnings: req.body.p1Earnings, p2Earnings: req.body.p2Earnings})
.then(res.status(200).send("success"))
  }
  else{
    res.status(200).send("already Added");
  }
})

.catch(error =>{
  console.log(error);
  res.status(500).json({error});
})
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
  let which=parseInt(req.params.which);
  if(gameModels[index].bothSubmitted())
  {
    res.status(200).json({gameState:'done',
                          p1Choice: gameModels[index].p1Choice,
                          p2Choice: gameModels[index].p2Choice,
                          })
  }
  else if(gameModels[index].amISubmitted(which))
    res.status(200).json({gameState:'submitted'});
  else
    res.status(200).json({gameState:'unsubmitted'});
});
//indicates player is ready for the next round
app.post('/api/ready', (req,res)=>{
  let id= parseInt(req.body.gameID);
  let which= parseInt(req.body.which);
  let index=getGameIndex(id);
   console.log("ready - id: "+id+", which: "+which);
   console.log("bothReady? ", gameModels[index].bothReady());
  if( which===0)
  {
    gameModels[index].p1Ready=true;
    res.status(200).send("success");
  }
  else if( which===1)
  {
    gameModels[index].p2Ready=true;
    res.status(200).send("success");
  }
  else
  {
    res.status(400).send("error in /ready");
  }
});
//gets current round
app.get('/api/ready/:gameID',(req,res)=>{
  let id= parseInt(req.params.gameID);
  let index= getGameIndex(id);
  if(gameModels[index].bothReady())
  {
      gameModels[index].goToNextRound();
  }
  res.status(200).json({currentRound: gameModels[index].currentRound});

})

app.get('/api/numberOfRounds/:gameID',(req,res)=>{
  let id= parseInt(req.params.gameID);
  let index= getGameIndex(id);
  res.status(200).json({numberOfRounds:gameModels[index].numberOfRounds});
})
app.get('/api/totalEarnings/:gameID/:which',(req,res)=>{
  let id= parseInt(req.params.gameID);
  let which =parseInt(req.params.which);
  if(which===0){
    return knex('rounds').sum('p1Earnings').where('gameID',id).then(earnings => {
      let x= JSON.parse(JSON.stringify(earnings[0]));
      res.status(200).send(x['sum(`p1Earnings`)']);
    });
  }
  return knex('rounds').sum('p2Earnings').where('gameID',id).then(earnings => {
      let x= JSON.parse(JSON.stringify(earnings[0]));
      res.status(200).send(x['sum(`p2Earnings`)']);
  });
})
function removeFromAvaiableUsers(id){
  let index= null;
  for(let x=0;x<availableUsers;x++)
  {
    if(availableUsers[x].id==id)
      index=x;
  }
  if(index!==null)
    availableUsers.splice(index,1);
}
app.post('/api/createGame', (req,res) =>{
 // Pick up from here update create game so I'm not hitting the foreign key constraint
    return knex('games').insert(
      {player1ID:knex('users').where('id',req.body.player1ID).select('id'), 
      coach1ID:knex('users').where('id',req.body.coach1ID).select('id'), 
      player2ID:knex('users').where('id',req.body.player2ID).select('id'), 
      coach2ID:knex('users').where('id',req.body.coach2ID).select('id')})
    .then(ids => {
      
      let gID=parseInt(ids[0]);
      let p1ID=parseInt(req.body.player1ID);
      let p2ID=parseInt(req.body.player2ID);
      let c1ID=parseInt(req.body.coach1ID);
      let c2ID=parseInt(req.body.coach2ID);
      console.log("NEW GAME: ", gID,p1ID,p2ID,c1ID, c2ID);
      console.log(availableUsers);
      removeFromAvaiableUsers(p1ID);
      removeFromAvaiableUsers(p2ID);
      removeFromAvaiableUsers(c1ID);
      removeFromAvaiableUsers(c2ID);
      console.log(availableUsers);
      let game= new Game(gID,p1ID,p2ID, c1ID, c2ID);
      gameModels.push(game);
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
    res.status(200).send("success");
  }
  else if(which ===1)
  {
    gameModels[index].p2Choice=choice;
    res.status(200).send("success");
  }
  else{
    console.log("error in submit choice server side");
    res.status(500).send('failure');

  }
});
app.delete('/api/game/:id', (req,res)=>
{
  let gameID=parseInt(req.params.id);
  let index= getGameIndex(gameID);


  if(typeof index != "undefined")
  {
    gameModels.splice(index,1);
  }
  console.log(gameModels);
  res.status(200).send("deletedGame");
})
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
       if(!userIsInAvaiablePlayers(user.id))
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
