#include "Player.h"

Player::Player() {
    printf("Player constructor\n");
    currentTime = 0;
    games = new Game*[1000];
    
    asExpert = false;
}

Player::~Player() {
    printf("Player destructor ... "); fflush(stdout);
    
    if (!asExpert) {
        for (int i = 0; i < currentTime; i++) {
            //printf("%i ", i); fflush(stdout);
            delete games[i];
        }
        delete games;
    }
    
    printf("Destructed\n"); fflush(stdout);
}

void Player::Reset() {
}

bool Player::orient2Game(char *gameString) {
    printf("Player orient2Game()\n");
    return true;
}

void Player::produceStartCheapTalk(char buf[10000]) {
    printf("Player produceStartCheapTalk\n");
}

void Player::produceEndCheapTalk(char buf[10000]) {
    printf("Player produceEndCheapTalk\n");
}

void Player::processStartCheapTalk(char buf[10000]) {
    printf("Player processStartCheapTalk\n");
}

void Player::processEndCheapTalk(char buf[10000]) {
    printf("Player processEndCheapTalk\n");
}

int Player::Move() {
    printf("Player move\n");
	return 0;
}

void Player::moveUpdate(int actions[2], double dollars[2]) {
	printf("Player update\n");
}
