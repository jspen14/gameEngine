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

// Data
let currentGames = [];

let availableUsers = [{role: "Coach", id: 1, name: "Joe"}, {role: "Coach", id: 2, name: "Jon"},
                      {role: "Player", id: 3, name: "Josh"}, {role: "Player", id: 4, name: "Chad"}];


// Endpoint Functions
app.get('/api/availableUsers',(req,res) => {

  res.send(availableUsers);
});

app.get('/api/currentGames',(req,res) =>{
  res.send(currentGames);
});

app.get('/api/inGameStatus/:id', (req,res)=>{
  let id = parseInt(req.params.id);

 // pick up from here tomorrow ... get the ID from the current game and send it back to the store
  for (let i = 0; i < currentGames.length; i++){
    if (id == currentGames[i].player1ID || id == currentGames[i].coach1ID ||
        id == currentGames[i].player2ID || id == currentGames[i].coach2ID){

          res.status(200).json({inGameStatus: true, gameID: currentGames[i].gameID});
          return;
        }
  }
  res.status(200).json({inGameStatus: false});
});

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
  console.log("in coachChatID: (userID - gameID): " + req.params.userID + " - " + req.params.gameID);

  knex('chatID').where({
    gameID: req.params.gameID,
    user1: req.params.userID,
    chatType: 'P/C',
  }).orWhere({
    gameID: req.params.gameID,
    user2: req.params.userID,
    chatType: 'P/C',
  }).then(response => {
    console.log("this is from the server: " + response[0].id)
    res.status(200).json({id: response[0].id});
    return;
  });

});

app.post('/api/coachChatMsgs', (req,res) => {
  console.log(req.body);

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

  console.log("Server: " + chatID);

  knex('chats').where('chatID',chatID).then(response => {
    console.log("response from /api/coachChat: " + response[0].message);
    res.status(200).json({messages: response});
  }).catch(err => {
    console.log("GET /api/coachChat/:chatID Failed: " + err);
    res.status(500);
  })

});

// END JSPENCER CHAT STUFF

app.get('/api/matrix/:id', (req,res)=> {
  let id=parseInt(req.params.id);
  knex('matrices').where('id',id).select('type', 'matrix').then(matrix => {
    res.status(200).json({matrix:matrix});
  }).catch(error =>{
    res.status(500).json({error});
  });
});

app.post('/api/createGame', (req,res) =>{

  return knex('games').insert({
    player1ID:req.body.player1ID, //knex('users').where('id', req.body.player1ID).select('id')
    coach1ID:req.body.coach1ID,
    player2ID:req.body.player2ID,
    coach2ID:req.body.coach2ID})
    .then(ids => {
      // Put game into currentGames array
      let game = {roundNum:0, gameID:parseInt(ids[0]), player1ID:parseInt(req.body.player1ID), coach1ID:parseInt(req.body.coach1ID),
                  player2ID:parseInt(req.body.player2ID), coach2ID:parseInt(req.body.coach2ID)};
      currentGames.push(game);

      // Send gameID to admin so we can view game progress
      knex('games').where({id: ids[0]}).first(); // I'm pretty sure that this isn't doing anything
      console.log("on server in createGame: " + ids[0]);
      res.status(200).json({gameID:ids[0]});
    }).catch(error => {
      console.log(error);
      res.status(500).json({error})
    });
});

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
