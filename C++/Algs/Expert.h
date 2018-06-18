#ifndef EXPERT_H
#define EXPERT_H

#include "defs.h"
#include "Game.h"

class Expert {
public:
	Expert();
	virtual ~Expert();

    virtual void init(Game *_g);
    virtual void Reset();
    virtual double getPotential();
    virtual void selectAction(Game *_g);
    virtual void update(int acts[2], double payoffs[2], bool active);
    virtual void produceStartCheapTalk(char *buf);
    virtual void processStartCheapTalk(char *buf);
    virtual const char *whoAmI();
    void resetInternalExpectation();
    virtual bool matchesPlan(int proposal[2]);
    
    int me;
    double potential;
    
    int selectedAction;
    double internalExpectationTally;
};

#endif