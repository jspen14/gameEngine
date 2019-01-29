#ifndef MaxMinExpert_H
#define MaxMinExpert_H

#include "defs.h"
#include "Game.h"
#include "Expert.h"

class MaxMinExpert : public Expert {
public:
	MaxMinExpert();
    MaxMinExpert(int _me);
	~MaxMinExpert();

    void init(Game *_g);
    void Reset();
    double getPotential();
    void selectAction(Game *_g);
    int implementProtection(Game *_g);
    
    void update(int acts[2], double payoffs[2], bool active);
    void produceStartCheapTalk(char *buf);
    void processStartCheapTalk(char *buf);
    const char *whoAmI();
    bool matchesPlan(int proposal[2]);
    
    int me, t;
    double potential, futuro;
};

#endif