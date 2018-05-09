//This adds fake users, games and rounds for testing

// Knex Setup
const env = process.env.NODE_ENV || 'development';
const config = require('../knexfile')[env];  
const knex = require('knex')(config);
//Takes Json as argument on command line
const fs = require('fs');
const fileNames = process.argv.splice(2);

let games=[
	{	
		player1ID: 1,
		player2ID: 2,
		coach1ID: 3,
		coach2ID: 4,
	}
];
let users= [
{

	role: "player",
	coachType: null,
	name: "bill"
},
{
	role: "player",
	coachType: null,
	name: "ted"
},
{
	role: "coach",
	coachType: "dummy",
	name: "trump"
},
{
	role: "coach",
	coachType: "dummy",
	name: "obama"
},
{

	role: "player",
	coachType: null,
	name: "John"
},
{
	role: "player",
	coachType: null,
	name: "Doe"
},
{
	role: "coach",
	coachType: "dummy",
	name: "Carnegie"
},
{
	role: "coach",
	coachType: "dummy",
	name: "Mellon"
}
]
let rounds = [
	{
		gameID: 1,
		matrixID: 1,
		player1choice: null,
		player2choice: null
	},
		{
		gameID: 1,
		matrixID: 2,
		player1choice: null,
		player2choice: null
	},

]

let deleteUsers = () => {
  return knex('users').del();
}
/*
let deleteGames = () => {
	return knex('games').del();
}
let deleteRounds = () => {
	return knex('rounds').del();
}
*/
let insertUsers = () => {
  let promises = [];
  	users.forEach(user => {
    promises.push(knex('users').insert({role: user.role, coachType: user.coachType, name: user.name }));
  });
  return Promise.all(promises);
}
/*
let insertGames = ()=> {
	let promises =[];
	games.forEach(game =>{
		promises.push(knex('games').insert({player1ID: knex('users').where('id', game.player1ID).select('id'), 
											player2ID: knex('users').where('id', game.player2ID).select('id'),
											 coach1ID: knex('users').where('id', game.coach1ID).select('id'), 
											 coach2ID: knex('users').where('id', game.coach2ID).select('id')}));
	});
	return Promise.all(promises);
}
let insertRounds = ()=> {
	let promises =[];
	rounds.forEach(round =>{
		promises.push(knex('rounds').insert({
			gameID: knex('games').where('id', round.gameID).select('id'), 
			matrixID: knex('matrices').where('id',round.matrixID).select('id'), 
			player1choice: round.player1choice,
			player2choice: round.player2choice}));
	});
	return Promise.all(promises);
}

deleteRounds().then(() => {
  return deleteGames();
}).then(() => {
  return deleteUsers();
}).then(() => {
  return insertUsers();
}).then(() => {
  return insertGames();
}).then(() => {
	return	insertRounds();
}).then(()=> {
	console.log("created test data");
});
*/
deleteUsers().then(()=>{
	return insertUsers();
}).then(()=>console.log("inserted Users"));

