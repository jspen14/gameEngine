#include "MaxMinExpert.h"

MaxMinExpert::MaxMinExpert() {
    printf("incomplete MaxMinExpert constructor\n");
    potential = 0.0;
}

MaxMinExpert::MaxMinExpert(int _me) {
    me = _me;
    t = 0;
    potential = 0.0;
}

MaxMinExpert::~MaxMinExpert() {}

void MaxMinExpert::init(Game *_g) {}

void MaxMinExpert::Reset() {
    potential = 0.0;
}

double MaxMinExpert::getPotential() {
    if (t != 0)
        return potential / t;
    else
        return potential;
}

void MaxMinExpert::selectAction(Game *_g) {
    printf("maxmin selectAction\n"); fflush(stdout);

    futuro = potential + _g->minmax[me]->mv;
    printf("potential sums = %lf\n", potential);
    selectedAction = implementProtection(_g);
    
    printf("selectedAction = %i\n", selectedAction); fflush(stdout);
}

int MaxMinExpert::implementProtection(Game *_g) {
    int i;
    
    double num = rand() / (double)RAND_MAX;
    double sum = 0.0;
    for (i = 0; i < _g->A[me]; i++) {
        sum += _g->minmax[me]->ms[i];
        if (num <= sum)
            return i;
    }
    
    return _g->A[me]-1;
}


void MaxMinExpert::update(int acts[2], double payoffs[2], bool active) {
    internalExpectationTally += futuro - potential;

    potential = futuro;
    t++;
    printf("MaxMinPotential = %lf\n", getPotential());
}

void MaxMinExpert::produceStartCheapTalk(char *buf) {
    strcpy(buf, "--\n");
}

void MaxMinExpert::processStartCheapTalk(char *buf) {}

const char *MaxMinExpert::whoAmI() {
    return "MaxMinExpert";
}

bool MaxMinExpert::matchesPlan(int proposal[2]) {
    return false;
}
