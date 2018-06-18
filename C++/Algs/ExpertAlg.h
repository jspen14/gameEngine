#ifndef EXPERTALG_H
#define EXPERTALG_H

#include "defs.h"
#include "Player.h"
#include "Game.h"
#include "Expert.h"
#include "MaxMinExpert.h"
#include "TriggerStrat.h"
#include "ListenerExpert.h"
#include "ExploiterExpert.h"
#include "SelectionMechanism.h"
#include "RandomSelector.h"
#include "PlusPlusSelector.h"
#include "igNBS.h"

class ExpertAlg : public Player {
public:
    ExpertAlg();
    ExpertAlg(int _me, char *playerString);
    ~ExpertAlg();

	void Reset();
    bool orient2Game(char *gameString);
    void produceStartCheapTalk(char buf[10000]);
    void processStartCheapTalk(char buf[10000]);
    void produceEndCheapTalk(char buf[10000]);
    void processEndCheapTalk(char buf[10000]);
	int Move();
	void moveUpdate(int actions[2], double dineros[2]);
    
    //Expert *currentExpert;
    
    // set of experts
    Expert **setOfExperts;
    int numExperts;
    
    // expert selection mechanism
    SelectionMechanism *selMech;
    
    igNBS ignbs;
    int selectedAction;
};

#endif