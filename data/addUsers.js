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
let insertUsers = () => {
  let promises = [];
  	users.forEach(user => {
    promises.push(knex('users').insert({role: user.role, coachType: user.coachType, name: user.name }));
  });
    return Promise.all(promises);
}
deleteUsers().then(()=>{
	return insertUsers();
}).then(()=>console.log("inserted Users"));
