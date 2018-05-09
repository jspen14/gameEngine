class Game {
	constructor(gameID, player1, player2){
		this.gameID = gameID;
		this.player1 = player1;
		this.player2 = player2;
		this.p1Choice= null;
		this.p2Choice= null;
		this.round = 1;
		//this probably needs to change
		this.numberOfRounds=1;
		this.rounds=[];
	}
	//Getters
	get round() {
		return this.round;
	}
	get player1() {
		return this.player1;
	}
	get player2() {
		return this.player2;
	}
	get gameID() {
		return this.gameID;
	}
	//Setters
	set p1Choice(choice) {
		this.p1Choice= choice
		if(this.isRoundComplete())
		{
			this.completeRound(){

			}
		}
	}
	set p2Choice(choice) {
		this.p2Choice = choice
	}
	//Methods

	completeRound(){
		//report results

		//increment round
		this.round +=1;
		p1Choice=null;
		p2Choice=null;
		//figure end game
	}
	isRoundComplete(){
		if(p1Choice!==null and p2Choice!==null){
			return true;
		}
		else{
			return false
		}
	}

}
module.exports = Game;
/*
Game is created
Player1 and player2 select options.
while( player1.option ==Null or player2.option ==Null)
{
	wait..?
}
when both answeres are submitted -> complete round

complete round:
	report results to db
	upadate earnings
	increment round number
	get associated matrix for next round
if all rounds complete
	complete game

*/
//create rounds method?