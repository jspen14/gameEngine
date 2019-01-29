#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include "defs.h"
#include "Player.h"
#include "Game.h"
#include "igNBS.h"

class RandomPlayer : public Player {
public:
    RandomPlayer();
    RandomPlayer(int _me);
    ~RandomPlayer();

	void Reset();
    bool orient2Game(char *gameString);
    void produceStartCheapTalk(char buf[10000]);
    void processStartCheapTalk(char buf[10000]);
    void produceEndCheapTalk(char buf[10000]);
    void processEndCheapTalk(char buf[10000]);
	int Move();
	void moveUpdate(int actions[2], double dineros[2]);
    
    int c[10];
    //igNBS ignbs;
};

#endif