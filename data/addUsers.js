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
		role: "Player",
		name: "Trump"
	},
	{
		role: "Player",
		name: "Carnegie"
	},
	{
		role: "Player",
		name: "Thumper"
	},
	{
		role: "Player",
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

let insertUsers = () => {
  let promises = [];
  	users.forEach(user => {
    promises.push(knex('users').insert({role: user.role, name: user.name, hash: "password"}));
  });
  return Promise.all(promises);
}

insertUsers().then(()=>console.log("Inserted Users"));
