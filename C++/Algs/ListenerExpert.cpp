#include "ListenerExpert.h"

ListenerExpert::ListenerExpert() {
    printf("incomplete ListenerExpert constructor\n");
    potential = 0.0;
}

ListenerExpert::ListenerExpert(int _me, igNBS *_ignbs) {
    me = _me;
    t = 0;
    
    potential = 0.0;
    backup = new TriggerStrat(me, BULLIED1_V, _ignbs, false, 1.0); // begin by assuming a fair partner
    //upperBound = 1.0;
    //lowerBound = 0.0;
    targetDir = 0;
    targetInc = 0.05;
}

ListenerExpert::~ListenerExpert() {
    delete backup;
}

void ListenerExpert::init(Game *_g) {
    backup->init(_g);
    potential = backup->ignbs->nbs[me];
}

void ListenerExpert::Reset() {
    backup->Reset();
    backup->bullyWeight = 1.0;
    targetDir = 0;
    targetInc = 0.05;
}

double ListenerExpert::getPotential() {
    if (t != 0)
        return potential / t;
    else
        return potential;
}

void ListenerExpert::selectAction(Game *_g) {
    g = _g;
    
    // infer the joint action we should play -- these will be overwridden if there's a proposal via cheap talk
    backup->selectAction(_g);
    //printf("backup currentValors: %lf, %lf; bullyTarget = %lf\n", backup->currentValor[0], backup->currentValor[1], backup->bullyTarget);
    
    selectedAction = backup->selectedAction;
    backup->nbsExpected(_g, 1.0 / (1.0 + (t+0.01)/50.0));
    backup->bullyExpected(_g, me);

    ja[0] = backup->expectedActions[0];
    ja[1] = backup->expectedActions[1];
    
    projectedPayoff[0] = g->M[0][ja[0]][ja[1]];
    projectedPayoff[1] = g->M[1][ja[0]][ja[1]];
    
    //rand() % _g->A[me]; // select a random action for now
}

void ListenerExpert::update(int acts[2], double payoffs[2], bool active) {
    t++;
    
    potential = backup->bullyTarget;//projectedPayoff[me];
    //potential = backup->ignbs->nbs[me]; // assume a listener will get a fair shake
    
    internalExpectationTally += projectedPayoff[me];
    
    backup->update(acts, payoffs, active);
    //printf("update the backup; potential = %lf\n", backup->potential);
    
    updateModel(acts, ja);
    
    printf("listenPotential: %lf\n", getPotential());
}

void ListenerExpert::produceStartCheapTalk(char *buf) {
    strcpy(buf, "--\n");
}

void ListenerExpert::processStartCheapTalk(char *buf) {
    if (buf[0] != '-') {
        int expected[2];
        expected[0] = ja[0];
        expected[1] = ja[1];
    
        // a joint action was proposed, so listen to it
        extractJointAction(buf);
        selectedAction = ja[me];
        
        // I can update my model of the bully target
        updateModel(ja, expected);
    }
    else {
        // just use what I inferred previously; so nothing else to do for now
    }
}

void ListenerExpert::extractJointAction(char *buf) {
    char cpy[1024];
    
    strcpy(cpy, buf);
    cpy[1] = '\0';
    ja[0] = atoi(cpy);
    
    strcpy(cpy, buf);
    cpy[2] = '\0';
    ja[1] = atoi(cpy+1);

    printf("Proposed joint action: %i%i\n", ja[0], ja[1]);
    
    projectedPayoff[0] = g->M[0][ja[0]][ja[1]];
    projectedPayoff[1] = g->M[1][ja[0]][ja[1]];
    
    //potential += projectedPayoff[me];
}


void ListenerExpert::updateModel(int observed[2], int expected[2]) {
    if (observed[1-me] == expected[1-me]) {
        //    if he did what I expected him to do; do nothing
        printf("He acted as expected\n");
    }
    else {
        if (observed[1-me] == backup->nbsExpectedActions[1-me]) {
            //    if he tried to play fair, but I thought he should exploit, increase bullyWeight
            if (targetDir == 1)
                targetInc *= 1.1;
            else if (targetDir == 0) {
                targetInc /= 2.0;
            }
            targetDir = 1;
            if (targetInc > 0.05)
                targetInc = 0.05;

            backup->bullyWeight += targetInc;
            
            printf("increase bullyWeight\n");
            
            if (backup->bullyWeight > 1.0)
                backup->bullyWeight = 1.0;
        }
        if (observed[1-me] == backup->bullyExpectedActions[1-me]) {
            //    if he tried to exploit, but I thought he should play fair, decrease bullyWeight
            if (targetDir == -1)
                targetInc *= 1.1;
            else if (targetDir == 1) {
                targetInc /= 2.0;
            }
            targetDir = -1;
            if (targetInc > 0.05)
                targetInc = 0.05;

            backup->bullyWeight -= targetInc;
            
            printf("decrease bullyWeight\n");

            if (backup->bullyWeight < 0.0)
                backup->bullyWeight = 0.0;
        }
    }
    printf("bullyWeight = %lf (%lf)\n", backup->bullyWeight, targetInc);
}

const char *ListenerExpert::whoAmI() {
    return "ListenerExpert";
}

bool ListenerExpert::matchesPlan(int proposal[2]) {
    return false;
}