// Knex Setup
const env = process.env.NODE_ENV || 'development';
const config = require('../knexfile')[env];  
const knex = require('knex')(config);
//Takes Json as argument on command line
const fs = require('fs');
const fileNames = process.argv.splice(2);

const fileName = fileNames[0];
let temp= JSON.parse(fs.readFileSync(fileName, 'utf-8'));
let matrices=[];

for(let i=0; i<temp.numGames; i++)
{
  matrices.push( { type:temp.games[i].type, matrix:temp.games[i].payoffs});

}

/*matrices=[
{type: "2x2", matrix: [[[4,4],[2,10]],[[10,2],[0,0]]]}
//{type: "2x2", matrix: [[[1,2],[2,3]],[[2,1],[3,0]]]}
]*/
console.log("matrices");
console.log(matrices);

let deleteMatrices = () => {
  return knex('matrices').del();
}

let insertMatrices = () => {
  let promises = [];
  matrices.forEach(matrix => {
    promises.push(knex('matrices').insert({type: matrix.type, matrix: matrix.matrix}));
  });
  return Promise.all(promises);
}


deleteMatrices().then(() => {
  return insertMatrices();
}).then(() => {
  console.log("OK, matrices created");
});
