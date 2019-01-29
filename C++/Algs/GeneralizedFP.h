#ifndef GENERALIZEDFP_H
#define GENERALIZEDFP_H

#include "defs.h"
#include "Player.h"
#include "Expert.h"
#include "Game.h"

#define SOCIAL_WELFARE_GUY      0
#define FOLK_EGAL_GUY           1
#define SAFE_GUY                2
#define MAX_GUY                 3
#define MAXIMIZER0_GUY          4
#define MAXIMIZER1_GUY          5
#define MAXIMIZER2_GUY          6
//#define ATTACK_GUY              7
//#define CHARITABLE_GUY          8
//#define STACKELBERG_GUY         9

#define MAX_GUYS                10
#define MAX_ACTs                10

class GeneralizedFP : public Player, public Expert {
public:
    GeneralizedFP();
    GeneralizedFP(const char *quien, int _me, bool _asExpert);
    ~GeneralizedFP();

	void Reset();
    bool orient2Game(char *gameString);
    void produceStartCheapTalk(char buf[10000]);
    void processStartCheapTalk(char buf[10000]);
    void produceEndCheapTalk(char buf[10000]);
    void processEndCheapTalk(char buf[10000]);
	int Move();
	void moveUpdate(int actions[2], double dineros[2]);
    
    void addTipo(int code);
    void getGamma();
    void getRankings();
    void sortEm(double utils[MAX_ACTs], int rankings[MAX_ACTs]);
    void rankSocialWelfare(int acts[MAX_ACTs]);
    void rankFolkEgal(int acts[MAX_ACTs]);
    void rankSafe(int acts[MAX_ACTs]);
    void rankMax(int acts[MAX_ACTs]);
    void rankMaximizer0(int acts[MAX_ACTs]);
    void rankMaximizer1(int acts[MAX_ACTs]);
    void rankMaximizer2(int acts[MAX_ACTs]);
    //void rankAttack(int acts[MAX_ACTs]);
    //void rankCharity(int acts[MAX_ACTs]);
    //void rankStackelberg(int acts[MAX_ACTs]);
    
    void bestResponse(int index, double partStrat[MAX_ACTs], double strat[MAX_ACTs]);
    double getSignalQuality(int index);
    int findCorrespondence(int tipo, int himAct);
    
    int tipos[MAX_GUYS];
    int numTipos;
    
    double kappa[MAX_GUYS], gamma[MAX_GUYS];
    double acciones[MAX_GUYS][MAX_ACTs];
    
    int theRankings[MAX_GUYS][MAX_ACTs];
    int cuenta[MAX_GUYS][MAX_ACTs];
        
    // additional stuff to implement Expert
    void init(Game *_g);
    double getPotential();
    void selectAction(Game *_g);
    void update(int acts[2], double payoffs[2], bool active);
    const char *whoAmI();
    bool matchesPlan(int proposal[2]);

    double maxUtil;
    
    int numGames[MAX_ACTs];
    
};

#endif