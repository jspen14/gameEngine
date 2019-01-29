#ifndef MBRL_H
#define MBRL_H

#include "defs.h"
#include "Player.h"
#include "Game.h"

#define NUM_BINS        2

class MBRL : public Player {
public:
    MBRL();
    MBRL(int _me);
    ~MBRL();

	void Reset();
    bool orient2Game(char *gameString);
    void produceStartCheapTalk(char buf[10000]);
    void processStartCheapTalk(char buf[10000]);
    void produceEndCheapTalk(char buf[10000]);
    void processEndCheapTalk(char buf[10000]);
	int Move();
	void moveUpdate(int actions[2], double dineros[2]);

    int getMove(int index, int actions[2]);
    double getLowV(int index, int a);
    double getHighV(int index, int a);
    
    void valueIteration();
    double vIterate();
    double maxV(int yo, int el);
    double getProb(int lMe, int lHim, int aHim);
    double getR(int lMe, int lHim);
    double valor(int bin);

    int myLastMove, hisLastMove;
    
    // things I model
    int ***kappa;
    double lowVal, highVal;
    int sampleCount;
    
    // things I compute
    double ***V, ***newV;
};

#endif