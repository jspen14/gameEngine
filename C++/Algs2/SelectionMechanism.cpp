#include "SelectionMechanism.h"


SelectionMechanism::SelectionMechanism() {
    printf("SelectionMechanism constructor\n");
}

SelectionMechanism::~SelectionMechanism() {}

void SelectionMechanism::selectExpert(Game *_g) {}

void SelectionMechanism::update(int actions[2], double dollars[2]) {}

void SelectionMechanism::evaluateProposal(char buf[10000]) {}
