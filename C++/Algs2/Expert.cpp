#include "Expert.h"

Expert::Expert() {
    printf("Expert constructor\n");
    potential = 0.0;
}

Expert::~Expert() {}

void Expert::init(Game *_g) {}

void Expert::Reset() {}

double Expert::getPotential() {
    return potential;
}

void Expert::selectAction(Game *_g) {
}

void Expert::update(int acts[2], double payoffs[2], bool active) {}

void Expert::produceStartCheapTalk(char *buf) {
    strcpy(buf, "--\n");
}

void Expert::processStartCheapTalk(char *buf) {}

const char *Expert::whoAmI() {
    return "Generic Expert";
}

void Expert::resetInternalExpectation() {
    internalExpectationTally = 0.0;
}

bool Expert::matchesPlan(int proposal[2]) {
    return false;
}
