#ifndef PLUSPLUSSELECTOR_H
#define PLUSPLUSSELECTOR_H

#include "defs.h"
#include "Game.h"
#include "Expert.h"
#include "SelectionMechanism.h"

#define NOMatch     -1
#define NOProposal  0
#define AMatch      1

class PlusPlusSelector : public SelectionMechanism {
public:
    PlusPlusSelector();
    PlusPlusSelector(int _me, Expert **_setOfExperts, int _numExperts);
    ~PlusPlusSelector();
    
    void selectExpert(Game *_g);
    void update(int actions[2], double dollars[2]);
    void evaluateProposal(char *buf);
    
    int pickSatisficingExpert();
    void extractJointAction(char buf[10000], int proposal[2]);
    bool isCongruent(int index);
    
    int t;
    int me;
    double V_nbs, Rbar;
    double aspiration;
    
    int **match;
  
    FILE *logfp;
    double currentScore;
    int cycleCount, cycleLength;
};

#endif