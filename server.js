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

app.get('/api/matrix/:id', (req,res)=> {
  let id=parseInt(req.params.id);
  knex('matrices').where('id',id).select('type', 'matrix').then(matrix => {
    res.status(200).json({matrix:matrix});
  }).catch(error =>{
    res.status(500).json({error});
  });
 

app.post('/api/createGame', (req,res) =>{
  console.log(req.body);


  res.status(200).json({gameID:0}); // TODO update this zero so that it actually returns the gameID
});

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
