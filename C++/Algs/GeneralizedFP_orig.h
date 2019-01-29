#ifndef GENERALIZEDFP_H
#define GENERALIZEDFP_H

#include "defs.h"
#include "Player.h"
#include "Game.h"

#define SOCIAL_WELFARE_GUY      0
#define FOLK_EGAL_GUY           1
#define SAFE_GUY                2
#define MAX_GUY                 3
#define MAXIMIZER0_GUY          4
#define MAXIMIZER1_GUY          5
#define ATTACK_GUY              6
#define CHARITABLE_GUY          7
#define STACKELBERG_GUY         8
#define RANDOM_GUY              9

#define MAX_GUYS                10
#define MAX_ACTs                10

class GeneralizedFP : public Player {
public:
    GeneralizedFP();
    GeneralizedFP(int _me);
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
    void getAcciones();
    void accionesSocialWelfare(double acts[MAX_ACTs]);
    void accionesFolkEgal(double acts[MAX_ACTs]);
    void accionesSafe(double acts[MAX_ACTs]);
    void accionesMax(double acts[MAX_ACTs]);
    void accionesMaximizer0(double acts[MAX_ACTs]);
    void accionesMaximizer1(double acts[MAX_ACTs]);
    void accionesAttack(double acts[MAX_ACTs]);
    void accionesCharity(double acts[MAX_ACTs]);
    void accionesStackelberg(double acts[MAX_ACTs]);
    void accionesRandom(double acts[MAX_ACTs]);
    
    int tipos[MAX_GUYS];
    int numTipos;
    
    double kappa[MAX_GUYS], gamma[MAX_GUYS];
    double acciones[MAX_GUYS][MAX_ACTs];
    
    
};

#endif