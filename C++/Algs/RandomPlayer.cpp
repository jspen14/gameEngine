#include "RandomPlayer.h"

RandomPlayer::RandomPlayer() {
    printf("RandomPlayer constructor\n");
}

RandomPlayer::RandomPlayer(int _me) {
    printf("RandomPlayer constructor\n");
    me = _me;
}

RandomPlayer::~RandomPlayer() {
    printf("RandomPlayer destructor\n");
}

void RandomPlayer::Reset() {
}

bool RandomPlayer::orient2Game(char *gameString) {
    char gameOverTest[1024];
    strcpy(gameOverTest, gameString);

    char *token = strtok(gameOverTest, "{ \",:");
    char *tipo = strtok(NULL, "{ \",:");
    //printf("tipo: %s\n", tipo);
    if (!strcmp(tipo, "Quit")) {
        return false;
    }

    //printf("gameString: %s\n", gameString);

    games[currentTime] = new Game(gameString);
    
    ignbs.updateWithGame(games[currentTime]);

    return true;
}

void RandomPlayer::produceStartCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void RandomPlayer::produceEndCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void RandomPlayer::processStartCheapTalk(char buf[10000]) {
}

void RandomPlayer::processEndCheapTalk(char buf[10000]) {
}

int RandomPlayer::Move() {
	return rand() % games[currentTime]->A[me];
}

void RandomPlayer::moveUpdate(int actions[2], double dollars[2]) {
    currentTime ++;
}
