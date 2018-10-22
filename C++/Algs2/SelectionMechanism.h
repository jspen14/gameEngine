#ifndef SELECTIONMECHANISM_H
#define SELECTIONMECHANISM_H

#include "defs.h"
#include "Expert.h"
#include "Game.h"

class SelectionMechanism {
public:
    SelectionMechanism();
    ~SelectionMechanism();
    
    virtual void selectExpert(Game *_g);
    virtual void update(int actions[2], double dollars[2]);
    virtual void evaluateProposal(char buf[10000]);
    
    Expert **setOfExperts;
    int numExperts;
    
    Expert *currentExpert;
    int experto;
    
    Game *currentGame;
};

#endif