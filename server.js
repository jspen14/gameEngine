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
    this._p1Earnings=[];
    this._p2Earnings=[];
    this._matrix=[];
    this.getMatrix();

    //fix this later -- make it so number of rounds autimatically updates
    this._numberOfRounds=this.setNumberOfRounds();

  }
  //Getters
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
  get p1Earnings(){
    return this._p1Earnings;
  }
  get p2Earnings(){
    return this._p2Earnings;
  }
  get matrix(){
    return this._matrix;
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
  set matrix(m){
    this._matrix=m;
  }
  addP1Earnings(earnings)
  {
    this.p1Earnings.push(earnings);
  }
  addP2Earings(earnings)
  {
    this.p2Earnings.push(earnings);
  }
  getP1RoundEarnings()
  {
    return this.p1Earnings[this.currentRound-1];
  }
  getP2RoundEarnings()
  {
    return this.p2Earnings[this.currentRound-1];
  }
  getP1TotalEarnings()
  {
    let total=0;
    for(let x=0;x<this.p1Earnings.length;x++)
    {
      total+=this.p1Earnings[x];
    }
    return total;
  }
  getP2TotalEarnings()
  {
    let total=0;
    for(let x=0;x<this._p2Earnings.length;x++)
    {
      total+=this._p2Earnings[x];
    }
    return total;
  }
  getP1AverageEarnings()
  {
    let total=0;
    for(let x=0;x<this._p1Earnings.length;x++)
    {
      total+=this._p1Earnings[x];
    }
    if(this._p1Earnings.length==0)
      return 0;
    return total/this._p1Eearnings.length;
  }
  getP2AverageEarnings()
  {
    let total=0;
    for(let x=0;x<this._p2Earnings.length;x++)
    {
      total+=this._p2Earnings[x];
    }
    if(this._p2Earnings.length==0)
      return 0;
    return total/this._p2Earnings.length;
  }
  bothSubmitted()
  {
    if (this.p1Choice===null || this.p2Choice===null)
      { 
        return false;
      }
    return true; 
  }
  updateEarnings()
  {
    if(this.bothSubmitted() && this.p1Earnings.length<this.currentRound && this.p2Earnings.length<this.currentRound)
    {
      this.p1Earnings.push(this.matrix[this.p1Choice][this.p2Choice][0]);
      this.p2Earnings.push(this.matrix[this.p1Choice][this.p2Choice][1]);
      knex('rounds').insert({gameID: this.gameID, 
        matrixID:this.currentRound, 
        player1choice: this.p1Choice, 
        player2choice: this.p2Choice, 
        p1Earnings: this.getP1RoundEarnings(), 
        p2Earnings: this.getP2RoundEarnings()}).then()
    }
    
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
    this.getMatrix();
  }
  setNumberOfRounds(){
    knex('matrices').count('id').then(n =>{
      this.numberOfRounds=parseInt(n[0]["count(`id`)"]);
    })
  }
  getMatrix(){

    knex('matrices').where('id',this.currentRound).select('type', 'matrix').then(q => {
    
    let data=q[0];
    let mx=data.matrix
    let type= data.type;
    let dimensions= type.split('x');
    for(let i=0; i<dimensions.length;i++)
    {
      dimensions[i]=parseInt(dimensions[i]);
    }
    let rows=dimensions[0];
    let cols=dimensions[1];
    let index=0;
    let temparray=[]
    while(index<mx.length)
    {

      if(isNaN(mx[index]))
        index++;
      else{
        let temp=index;
        while(!isNaN(mx[temp]))
          temp++;
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
    this.matrix=matrix;
          
    }).catch(err => {
      console.log("getMatrix Failed:", err);

    });
  }
}

//--------------------------------------------

// Data

let gameModels= [];

let availableUsers = [];

// Endpoint Functions
app.get('/api/availableUsers',(req,res) => {
  res.send(availableUsers);
});
app.post('/api/inGameStatus', (req,res)=>{
  let user=req.body;
  let id = user.id;
 
  if(userIsInAvaiablePlayers(id)===false)
  {
    availableUsers.push(user);
  }
  for (let i = 0; i < gameModels.length; i++){
    if (id == gameModels[i].player1 || id == gameModels[i].coach1)
    {
      res.status(200).json({inGameStatus: true, gameID: gameModels[i].gameID, which:0, round: gameModels[i].currentRound});
      return;
        }
    if(id == gameModels[i].player2 || id == gameModels[i].coach2)
    {
      res.status(200).json({inGameStatus: true, gameID: gameModels[i].gameID, which:1, round: gameModels[i].currentRound});
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

// START JSPENCER CHAT STUFF
app.post('/api/coachChatID',(req,res) => {
  var coachID;

  knex('games').where('id',req.body.gameID).then(response => {
    if (req.body.userID == response[0].player1ID){
      coachID = response[0].coach1ID;
    }
    else if (req.body.userID == response[0].player2ID){
      coachID = response[0].coach2ID;
    }
    else {
      res.status(500);
      return;
    }
  }).then(query => {
    return knex('chatID').insert({
      gameID: req.body.gameID,
      user1: req.body.userID,
      user2: coachID,
      chatType: 'P/C'
    }).then(chatID => {
      res.status(200).json({id: chatID[0]});
    }).catch(err => {
      console.log("Error in /api/coachChatID: " + err)
      res.status(500);
    });

  }).catch(err => {
    console.log("Error in /api/coachChatID: " + err)
  });
});

app.get('/api/coachChatID/:userID/:gameID',(req,res) => {

  knex('chatID').where({
    gameID: req.params.gameID,
    user1: req.params.userID,
    chatType: 'P/C',
  }).orWhere({
    gameID: req.params.gameID,
    user2: req.params.userID,
    chatType: 'P/C',
  }).then(response => {
    res.status(200).json({id: response[0].id});
    return;
  });

});

app.post('/api/coachChatMsgs', (req,res) => {

  knex('chats').insert({
    chatID: req.body.chatID,
    userID: req.body.userID,
    message: req.body.text,
  }).then(response => {
    // I might want to have a return here, but I'm not sure yet
    res.status(200);
    return;
  }).catch(err => {
    console.log("POST /api/coachChat Failed: " + err);
    res.status(500);
    return;
  });

});

app.get('/api/coachChatMsgs/:chatID',(req,res) => {
  // I will have this retrieve all the information from all the chats with a matching chatID
  let chatID = parseInt(req.params.chatID);

  knex('chats').where('chatID',chatID).then(response => {

    res.status(200).json({messages: response});
  }).catch(err => {
    console.log("GET /api/coachChat/:chatID Failed: " + err);
    res.status(500);
  })

});

// END JSPENCER CHAT STUFF

//update Coach
app.get('/api/game/:id/:which',(req,res)=>
{
  let index= getGameIndex(parseInt(req.params.id));
  let which= parseInt(req.params.which);
  let totalEarnings;
  let roundEarnings;
  if(which===0)
  {
    totalEarnings=gameModels[index].getP1TotalEarnings();
    roundEarnings=gameModels[index].getP1RoundEarnings();
  }
  else
  {
    totalEarnings=gameModels[index].getP2TotalEarnings();
    roundEarnings=gameModels[index].getP2RoundEarnings();
  }

  res.status(200).json({
    round:gameModels[index].currentRound, 
    totalEarnings: totalEarnings, 
    roundEarnings: roundEarnings, 
    p1Choice: gameModels[index].p1Choice, 
    p2Choice: gameModels[index].p2Choice});

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
    gameModels[index].updateEarnings();
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
    res.status(500).send("error in /ready");
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
app.get('/api/totalEarnings/:gameID/:which', (req,res)=>{
  let index= getGameIndex(parseInt(req.params.gameID));
  let which =parseInt(req.params.which);
  if(which===0)
  {
    if(gameModels[index].p1Earnings.length===0)
    {
      res.status(200).json({earnings:0});
    }
    else{
      res.status(200).json({earnings:gameModels[index].getP1TotalEarnings()});
    }
  }
  else if(which===1)
  {
    if(gameModels[index].p2Earnings.length===0)
    {
      res.status(200).json({earnings:0});
    }
    else{
      res.status(200).json({earnings: gameModels[index].getP2TotalEarnings()});
    }
  }
  else
    res.status(500).send("error");
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
      removeFromAvaiableUsers(p1ID);
      removeFromAvaiableUsers(p2ID);
      removeFromAvaiableUsers(c1ID);
      removeFromAvaiableUsers(c2ID);
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
  let index = getGameIndex(gameID);
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
