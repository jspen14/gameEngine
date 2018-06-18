#include "RandomSelector.h"

RandomSelector::RandomSelector() {
    printf("incomplete RandomSelector constructor\n");
}

RandomSelector::RandomSelector(Expert **_setOfExperts, int _numExperts) {
    setOfExperts = _setOfExperts;
    numExperts = _numExperts;
    currentExpert = NULL;
    experto = -1;
    
    t = 0;
    currentGame = NULL;
}

RandomSelector::~RandomSelector() {}
    
void RandomSelector::selectExpert(Game *_g) {
    if ((currentExpert == NULL) || ((t % 4) == 0)) {
        experto = rand() % numExperts;
        currentExpert = setOfExperts[experto];
    }
}

void RandomSelector::update(int actions[2], double dollars[2]) {
    t++;
}
