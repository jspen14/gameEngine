#include "TriggerStrat.h"

TriggerStrat::TriggerStrat() {
    printf("incomplete TriggerStrat constructor\n");
}

TriggerStrat::TriggerStrat(int _me, int _targetType, igNBS *_ignbs, bool _punishment) {
    me = _me;
    targetType = _targetType;
    ignbs = _ignbs;
    punishment = _punishment;

    t = 0;
    Reset();
    guilt = 0.0;
    runningScore = 0.0;
    bullyWeight = 1.0;
}

TriggerStrat::TriggerStrat(int _me, int _targetType, igNBS *_ignbs, bool _punishment, double _bullyWeight) {
    me = _me;
    targetType = _targetType;
    ignbs = _ignbs;
    punishment = _punishment;

    t = 0;
    Reset();
    guilt = 0.0;
    runningScore = 0.0;
    bullyWeight = _bullyWeight;
}

TriggerStrat::~TriggerStrat() {
}

void TriggerStrat::init(Game *_g) {
    double epsilon = 1.0;

    currentValor[0] = currentValor[1] = 0.0;

    if (targetType == NBS_V) {
        //currentValor[0] = ignbs->nbs[0];
        //currentValor[1] = ignbs->nbs[1];
        potential = ignbs->nbs[me];
    }
    else if (targetType == BULLY1_V) {
        //currentValor[me] = ignbs->nbs[me] + epsilon;
        //currentValor[1-me] = ignbs->nbs[1-me] - epsilon;
        bullyTarget = 0.0;
        potential = ignbs->nbs[me] + epsilon;
    }
    else if (targetType == BULLIED1_V) {
        //currentValor[me] = ignbs->nbs[me] - epsilon;
        //currentValor[1-me] = ignbs->nbs[1-me] + epsilon;
        bullyTarget = 0.0;
        potential = ignbs->nbs[me] - epsilon;
    }
    else {
        //currentValor[0] = 0.0;
        //currentValor[1] = 0.0;
        potential = 0.0;
    }

    runningTally[0] = runningTally[1] = 0.0;
    //potential = currentValor[me];
}

void TriggerStrat::Reset() {
    //potential = -99999;

    //runningTally[0] = runningTally[1] = 0.0;

    // set runningTally to what it should have been if the expert had always been properly followed
    switch (targetType) {
        case NBS_V:
            runningTally[0] = ignbs->nbs[0];
            runningTally[1] = ignbs->nbs[1];
            break;
        case BULLY1_V:
            runningTally[me] = ignbs->getBullyValue(me, bullyTarget);
            runningTally[1-me] = bullyTarget;
            break;

        case BULLIED1_V:
            runningTally[1-me] = ignbs->getBullyValue(1-me, bullyTarget);
            runningTally[me] = bullyTarget;
            break;
        default:
            printf("TriggerStrat::Reset -- identity unknown\n");
            exit(1);
            break;
    }

    printf("runningTally: %lf, %lf\n", runningTally[0], runningTally[1]);

    guilt = 0.0;
}

double TriggerStrat::getPotential() {
    if (t != 0)
        return potential / t;
    else
        return potential;
}

void TriggerStrat::selectAction(Game *_g) {
    g = _g;

    //double wght = 0.5;
    switch (targetType) {
        case NBS_V:
            //selectedAction = nbsAction(g, 0.01);//1.0 / (1.0 + (t+0.01)/5.0));
            selectedAction = nbsAction(g, 1.0 / (1.0 + (t+0.01)/50.0));
            //selectedAction = nbsAction(g, 0.99);//1.0 / (1.0 + (t+0.01)/5.0));

            printf("projectedPayoffs: %.1lf, %.1lf\n", projectedPayoff[0], projectedPayoff[1]);
            // update currentValor
            currentValor[0] = ignbs->nbs[0];
            //if (currentValor[0] < (runningTally[0] + projectedPayoff[0]))
            //    currentValor[0] = (runningTally[0] + projectedPayoff[0]);

            currentValor[1] = ignbs->nbs[1];
            //if (currentValor[1] < (runningTally[1] + projectedPayoff[1]))
            //    currentValor[1] = (runningTally[1] + projectedPayoff[1]);

            break;
        case BULLY1_V:
            selectedAction = bullyAction(_g, 1-me);

            currentValor[me] = ignbs->getBullyValue(me, bullyTarget);
            currentValor[1-me] = bullyTarget;
            bullyTarget = bullyWeight * ignbs->nbs[1-me] + (1.0-bullyWeight) * ignbs->maxminSum[1-me];

            break;
        case BULLIED1_V:
            selectedAction = bullyAction(_g, me);

            // update currentValor
            currentValor[1-me] = ignbs->getBullyValue(1-me, bullyTarget);
            currentValor[me] = bullyTarget;
            bullyTarget = bullyWeight * ignbs->nbs[me] + (1.0-bullyWeight) * ignbs->maxminSum[me];

            break;
        default:
            printf("I don't know who I am\n");
            selectedAction = 0;
            break;
    }

    if ((guilt > 0.0) && punishment) {
        printf("needs to be punished: %lf\n", guilt);

        selectedAction = implementAttack(_g);
    }
}

int TriggerStrat::implementAttack(Game *_g) {
    int i;

    double num = rand() / (double)RAND_MAX;
    double sum = 0.0;
    for (i = 0; i < _g->A[me]; i++) {
        sum += _g->attack[me]->ms[i];
        if (num <= sum)
            return i;
    }

    return _g->A[me]-1;
}

// wght is the proportion between NBS and maxmin of opponent
int TriggerStrat::bullyAction(Game *_g, int bulliedIndex) {
    printf("target = %lf (vs. %lf) \n", bullyTarget, runningTally[bulliedIndex]);

    if (runningTally[bulliedIndex] < bullyTarget) {
        printf("Give bread crumbs\n");
        return nbsAction(_g, 0.8);
    }
    else {
        printf("Exploit!\n");
        double W[10][10];
        if (bulliedIndex == 0)
            computeSWUtility(g, W, 0.1, 0.9);
        else
            computeSWUtility(g, W, 0.9, 0.1);

        int i, j;
        double w_alto = -99999;
        int ind[2];
        for (i = 0; i < _g->A[0]; i++) {
            for (j = 0; j < _g->A[1]; j++) {
                if (W[i][j] > w_alto) {
                    w_alto = W[i][j];
                    ind[0] = i;
                    ind[1] = j;
                }
            }
        }
        printf("selected action %i after selecting solution %i, %i\n", ind[me], ind[0], ind[1]);
        expectedActions[0] = ind[0];
        expectedActions[1] = ind[1];
        projectedPayoff[0] = g->M[0][ind[0]][ind[1]];
        projectedPayoff[1] = g->M[1][ind[0]][ind[1]];

        return ind[me];
    }
}

int TriggerStrat::nbsAction(Game *_g, double w) {
    int i, j;

    double W[10][10];
    computeSWUtility(g, W, 0.5, 0.5);

    double F[10][10];
    computeFairUtility(g, F);

    double U[10][10];
    double u_alto = -99999;

    int ind[2];
    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            U[i][j] = w * W[i][j] + (1-w) * F[i][j];
            if (U[i][j] > u_alto) {
                u_alto = U[i][j];
                ind[0] = i;
                ind[1] = j;
            }
            //printf("U(%i, %i) = %.2lf (%.2lf, %.2lf)\n", i, j, U[i][j], W[i][j], F[i][j]);
        }
    }
    printf("selected action %i after selecting solution %i, %i\n", ind[me], ind[0], ind[1]);
    expectedActions[0] = ind[0];
    expectedActions[1] = ind[1];
    projectedPayoff[0] = g->M[0][ind[0]][ind[1]];
    projectedPayoff[1] = g->M[1][ind[0]][ind[1]];

    return ind[me];
}

void TriggerStrat::computeSWUtility(Game *_g, double W[10][10], double w1, double w2) {
    int i, j;

    double sw_alto = -99999, sw_bajo = 99999;
    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            W[i][j] = socialWelfare(_g, i, j, w1, w2);
            if (W[i][j] > sw_alto) {
                sw_alto = W[i][j];
            }
            if (W[i][j] < sw_bajo) {
                sw_bajo = W[i][j];
            }
        }
    }

    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            if (sw_alto > sw_bajo) {
                W[i][j] = (W[i][j] - sw_bajo) / (sw_alto - sw_bajo);
            }
            else {
                W[i][j] = 1.0;
            }
        }
    }
}

double TriggerStrat::socialWelfare(Game *_g, int a1, int a2, double w1, double w2) {
    return w1 * g->M[0][a1][a2] + w2 * g->M[1][a1][a2];
}

void TriggerStrat::computeFairUtility(Game *_g, double F[10][10]) {
    int i, j;

    double f_alto = -99999, f_bajo = 99999;
    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            F[i][j] = prodAdvantages(_g, i, j);
            if (F[i][j] > f_alto) {
                f_alto = F[i][j];
            }
            if (F[i][j] < f_bajo) {
                f_bajo = F[i][j];
            }
            //printf("F(%i, %i): %lf\n", i, j, F[i][j]);
        }
    }

    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            if (f_alto > f_bajo) {
                F[i][j] = (F[i][j] - f_bajo) / (f_alto - f_bajo);
            }
            else {
                F[i][j] = 1.0;
            }
        }
    }
}

double TriggerStrat::prodAdvantages(Game *_g, int a1, int a2) {
    double adv1, adv2;

    adv1 = (runningTally[0] + g->M[0][a1][a2]) - ignbs->maxminSum[0];
    adv2 = (runningTally[1] + g->M[1][a1][a2]) - ignbs->maxminSum[1];

    //printf("adv = %lf, %lf\n", adv1, adv2);

    if ((adv1 < 0.0) || (adv2 < 0.0))
        return 0.0;
    else
        return adv1 * adv2;
}

void TriggerStrat::update(int acts[2], double payoffs[2], bool active) {
    runningTally[0] += payoffs[0];
    runningTally[1] += payoffs[1];

    runningScore += projectedPayoff[me];
    internalExpectationTally += projectedPayoff[me];

    if (punishment && active) {
        if (guilt > 0.0)
            guilt += payoffs[1-me] - projectedPayoff[1-me];
        else if (acts[1-me] != expectedActions[1-me]) {
            double delta = 0.5; // extra initial punishment
            guilt += payoffs[1-me] - projectedPayoff[1-me] + delta;
        }
        if (guilt < 0.0)
            guilt = 0.0;

        printf("guilt = %lf\n", guilt);
    }
    else if (!active) {
        guilt = 0.0;
    }

    // update the potential
    //if (t != 0) {// in the first round, the potential is initialized based on the first game -- no need to add it now
    potential = currentValor[me];

    //if (runningScore > currentValor[me])
    //    potential = runningScore;
    //}
    t++;

    //printf("TriggerStrat-Potential2: %lf\n", potential2);
    printf("TriggerStrat-Potential: %lf\n", getPotential());
    printf("currentValors: %lf, %lf\n", currentValor[0], currentValor[1]);
}

void TriggerStrat::produceStartCheapTalk(char *buf) {
    if (!punishment || guilt <= 0.0)
        sprintf(buf, "%i%i\n", expectedActions[0], expectedActions[1]);
    else
        strcpy(buf, "--\n");

    printf("\nPre Decision Cheap Talk: %s\n", buf);
}

void TriggerStrat::bullyExpected(Game *_g, int bulliedIndex) {
    double W[10][10];
    if (bulliedIndex == 0)
        computeSWUtility(g, W, 0.1, 0.9);
    else
        computeSWUtility(g, W, 0.9, 0.1);

    int i, j;
    double w_alto = -99999;
    int ind[2];
    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            if (W[i][j] > w_alto) {
                w_alto = W[i][j];
                ind[0] = i;
                ind[1] = j;
            }
        }
    }
    bullyExpectedActions[0] = ind[0];
    bullyExpectedActions[1] = ind[1];
}

void TriggerStrat::nbsExpected(Game *_g, double w) {
    int i, j;

    double W[10][10];
    computeSWUtility(g, W, 0.5, 0.5);

    double F[10][10];
    computeFairUtility(g, F);

    double U[10][10];
    double u_alto = -99999;

    int ind[2];
    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            U[i][j] = w * W[i][j] + (1-w) * F[i][j];
            if (U[i][j] > u_alto) {
                u_alto = U[i][j];
                ind[0] = i;
                ind[1] = j;
            }
        }
    }

    nbsExpectedActions[0] = ind[0];
    nbsExpectedActions[1] = ind[1];
}

const char *TriggerStrat::whoAmI() {
    switch (targetType) {
        case (NBS_V): return "Fair TriggerStrat"; break;
        case (BULLIED1_V): return "Bullied TriggerStrat"; break;
        case (BULLY1_V): return "Bully TriggerStrat"; break;
        default: return "Unknown TriggerStrat"; break;
    }
}

bool TriggerStrat::matchesPlan(int proposal[2]) {
    printf("Proposal: %i%i; Plan: %i%i\n", proposal[0], proposal[1], expectedActions[0], expectedActions[1]);

    if ((proposal[0] == expectedActions[0]) && (proposal[1] == expectedActions[1]))
        return true;

    return false;
}
