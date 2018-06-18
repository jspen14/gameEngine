#ifndef ListenerExpert_H
#define ListenerExpert_H

#include "defs.h"
#include "Game.h"
#include "Expert.h"
#include "igNBS.h"
#include "TriggerStrat.h"

class ListenerExpert : public Expert {
public:
	ListenerExpert();
    ListenerExpert(int _me, igNBS *_ignbs);
	~ListenerExpert();

    void init(Game *_g);
    void Reset();
    double getPotential();
    void selectAction(Game *_g);
    void update(int acts[2], double payoffs[2], bool active);
    void produceStartCheapTalk(char *buf);
    void processStartCheapTalk(char *buf);
    void extractJointAction(char *buf);
    const char *whoAmI();
    bool matchesPlan(int proposal[2]);
    
    void updateModel(int observed[2], int expected[2]);
    
    int me, t;
    double potential;
    int ja[2];
    double projectedPayoff[2];
    
    Game *g;
    
    TriggerStrat *backup;
    //double upperBound, lowerBound;
    int targetDir;
    double targetInc;
};

#endif