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

//
let msgs = [{role: "Coach", text: "Hi"},{role: "Player", text: "How are you?"},{role: "Coach", text: "I am doing well, thanks! How are you?"},
                  {role: "Player", text: "Great. Just ready to win some money!"},{role: "Player", text: "Can you help me with that?"},
                  {role: "Coach", text: "I sure can!"},{role: "Player", text: "Perfect. Just tell me what I should do."},
                  {role: "Coach", text: "Okay. Let's start out by playing our personal best. Choose X"},{role: "Player", text: "Okay"}];

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
  res.send();
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

app.post('/api/createGame', (req,res) =>{
 // We need to have the player IDs
  console.log("in createGame on server: " + req.body);
  console.log(req.body.player1ID, req.body.player2ID, req.body.coach1ID, req.body.coach2ID)
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
      // Send gameID to admin so he can view game progress
      knex('games').where({id: ids[0]}).first();
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
