#ifndef HUMAN_H
#define HUMAN_H

#include "defs.h"
#include "Player.h"
#include "Game.h"

class Human : public Player {
public:
    Human();
    Human(int _me);
    ~Human();

	void Reset();
    bool orient2Game(char *gameString);
    void produceStartCheapTalk(char buf[10000]);
    void processStartCheapTalk(char buf[10000]);
    void produceEndCheapTalk(char buf[10000]);
    void processEndCheapTalk(char buf[10000]);
	int Move();
	void moveUpdate(int actions[2], double dineros[2]);    
};

#endif