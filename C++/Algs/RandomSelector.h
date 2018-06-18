#ifndef RANDOMSELECTOR_H
#define RANDOMSELECTOR_H

#include "defs.h"
#include "Game.h"
#include "Expert.h"
#include "SelectionMechanism.h"

class RandomSelector : public SelectionMechanism {
public:
    RandomSelector();
    RandomSelector(Expert **_setOfExperts, int _numExperts);
    ~RandomSelector();
    
    void selectExpert(Game *_g);
    void update(int actions[2], double dollars[2]);
    
    int t;
};

#endif