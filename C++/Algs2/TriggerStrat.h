#ifndef TRIGGERSTRAT_H
#define TRIGGERSTRAT_H

#include "defs.h"
#include "Expert.h"
#include "igNBS.h"

#define BULLIED1_V    0
#define BULLIED2_V    1
#define FAIR_V        2
#define BULLY1_V      3
#define BULLY2_V      4
#define NBS_V         5
#define WELFARE_V     6

class TriggerStrat : public Expert {
public:
	TriggerStrat();
    TriggerStrat(int _me, int _targetType, igNBS *ignbs, bool _punishment);
    TriggerStrat(int _me, int _targetType, igNBS *_ignbs, bool _punishment, double _bullyWeight);
	~TriggerStrat();

    void init(Game *_g);
    void Reset();
    double getPotential();
    void selectAction(Game *_g);
    void update(int acts[2], double payoffs[2], bool active);
    
    int bullyAction(Game *_g, int bulliedIndex);
    int implementAttack(Game *_g);
    int nbsAction(Game *_g, double w);
    void computeSWUtility(Game *_g, double W[10][10], double w1, double w2);
    double socialWelfare(Game *_g, int a1, int a2, double w1, double w2);
    void computeFairUtility(Game *_g, double F[10][10]);
    double prodAdvantages(Game *_g, int a1, int a2);

    void produceStartCheapTalk(char *buf);
    
    void bullyExpected(Game *_g, int bulliedIndex);
    void nbsExpected(Game *_g, double w);
    
    const char *whoAmI();
    bool matchesPlan(int proposal[2]);

    double currentValor[2];
    int targetType;
    Game *g;
    int t;

    double runningTally[2];
    //double maxminSum[2];
    
    igNBS *ignbs;
    
    double bullyTarget, bullyWeight;
    
    bool punishment;
    double projectedPayoff[2];
    int expectedActions[2];
    double guilt;
    double runningScore;
    
    int nbsExpectedActions[2], bullyExpectedActions[2];
};


#endif