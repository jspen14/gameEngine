#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "Game.h"

class Player {
public:
	Player();
	virtual ~Player();

	virtual void Reset();
    virtual bool orient2Game(char *gameString);
    virtual void produceStartCheapTalk(char buf[10000]);
    virtual void processStartCheapTalk(char buf[10000]);
    virtual void produceEndCheapTalk(char buf[10000]);
    virtual void processEndCheapTalk(char buf[10000]);
	virtual int Move();
	virtual void moveUpdate(int actions[2], double dineros[2]);
    
    Game **games;
    int currentTime;
    int me;
};

#endif