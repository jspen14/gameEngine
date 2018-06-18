#include "PlusPlusSelector.h"

PlusPlusSelector::PlusPlusSelector() {
    printf("incomplete PlusPlusSelector constructor\n");
}

PlusPlusSelector::PlusPlusSelector(int _me, Expert **_setOfExperts, int _numExperts) {
    setOfExperts = _setOfExperts;
    numExperts = _numExperts;
    me = _me;
    
    currentExpert = NULL;
    experto = -1;
    
    t = 0;
    currentGame = NULL;
    Rbar = 0.0;
    V_nbs = 0;
    currentScore = 0.0;
    
    cycleCount = 0;
    cycleLength = 4;
    
    match = new int*[numExperts];
    for (int i = 0; i < numExperts; i++) {
        match[i] = new int[cycleLength];
        for (int j = 0; j < cycleLength; j++) {
            match[i][j] = NOProposal;
        }
    }
    
    if (me == 0)
        logfp = fopen("thelog_0.txt", "w");
    else
        logfp = fopen("thelog_1.txt", "w");
}

PlusPlusSelector::~PlusPlusSelector() {
    fclose(logfp);
    
    for (int i = 0; i < numExperts; i++)
        delete match[i];
    delete match;
}
    
void PlusPlusSelector::selectExpert(Game *_g) {
    if (currentExpert != NULL)
        //fprintf(logfp, "%i: aspiration = %lf; potential = %lf\n", t, aspiration, currentExpert->getPotential());
        fprintf(logfp, "%i: %i\n", t, experto);

    currentGame = _g;
    if ((currentExpert == NULL) || (cycleCount >= cycleLength)) {
        cycleCount = 0;
        if (t == 0) {
            aspiration = currentGame->nbs->sol->R[me];
            experto = pickSatisficingExpert();
            currentExpert = setOfExperts[experto];
            printf("picked expert %i with potential %lf\n", experto, currentExpert->getPotential()); fflush(stdout);
        }
        else {            
            //printf("nratio vs sratio: %lf to %lf\n", nratio, sratio);
            double probStay = 0.0;
            printf("expected = %lf; actual = %lf\n", currentExpert->internalExpectationTally, currentScore);
            if (currentExpert->internalExpectationTally > 0.0)
                probStay = pow(currentScore / currentExpert->internalExpectationTally, 4.0);
            double externalValidation = pow(currentExpert->getPotential() / aspiration, 4.0);
            printf("externalValidation: %lf (%lf, %lf)\n", externalValidation, currentExpert->getPotential(), aspiration);
            if (externalValidation < probStay)
                probStay = externalValidation;
            double num = (rand() % 1001) / 1000.0;
            printf("probStay = %lf; num = %lf\n", probStay, num);
            if (num > probStay) {
                int old = experto;
                experto = pickSatisficingExpert();
                currentExpert = setOfExperts[experto];
                printf("picked expert %i with potential %lf\n", experto, currentExpert->getPotential()); fflush(stdout);

                if (old != experto) {
                    // new expert selected -- reset dhats
                    currentExpert->Reset();
                    //currentExpert->resetInternalExpectation();
                    //currentScore = 0.0;
                }
            }
            else {
                printf("continue with current expert (%i)\n", experto);
            }
            //currentExpert->Reset();
            currentExpert->resetInternalExpectation();
            currentScore = 0.0;
        }
    }
}

void PlusPlusSelector::update(int actions[2], double dollars[2]) {
    Rbar += dollars[me];
    currentScore += dollars[me];
    V_nbs += currentGame->nbs->sol->R[me];
    cycleCount++;
    t++;

    // update the aspiration level
    double w = 1.0 / (1.0+(t/30.0));
    aspiration = (w * (V_nbs / t)) + ((1.0 - w) * (Rbar / t));
    printf("aspiration level: %lf\n", aspiration);
}

void PlusPlusSelector::evaluateProposal(char buf[10000]) {
    int i, j;
    for (i = 0; i < numExperts; i++) {
        for (j = cycleLength-1; j > 0; j--) {
            match[i][j] = match[i][j-1];
        }
    }

    if (buf[0] != '-') {
        int proposal[2];
        extractJointAction(buf, proposal);
        
        printf("proposal: %i%i\n", proposal[0], proposal[1]);
    
        // see which experts plan to do this proposal this round
        for (i = 0; i < numExperts; i++) {
            if (setOfExperts[i]->matchesPlan(proposal))
                match[i][0] = AMatch;
            else
                match[i][0] = NOMatch;
        }
    }
    else {
        for (i = 0; i < numExperts; i++) {
            match[i][0] = NOProposal;
        }
    }

    // TODO: decide if a switch is in order
    printf("Congruent: ");
    for (i = 0; i < numExperts; i++) {
        if (isCongruent(i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

bool PlusPlusSelector::isCongruent(int index) {
    int i;
    
    bool has = false;
    for (i = 0; i < cycleLength; i++) {
        if (match[index][i] == NOMatch)
            return false;
        else if (match[index][i] == AMatch)
            has = true;
    }

    if (has)
        return true;
    else
        return false;
}

void PlusPlusSelector::extractJointAction(char buf[10000], int proposal[2]) {
    char cpy[1024];
    
    strcpy(cpy, buf);
    cpy[1] = '\0';
    proposal[0] = atoi(cpy);
    
    strcpy(cpy, buf);
    cpy[2] = '\0';
    proposal[1] = atoi(cpy+1);

    printf("Proposed joint action: %i%i\n", proposal[0], proposal[1]);
}


int PlusPlusSelector::pickSatisficingExpert() {
    int i;
    
    bool *satisficingExperts = new bool[numExperts];
    int numSatisficingExperts = 0;
    
    printf("pickSatisficingExpert (aspiration = %lf):\n", aspiration);
    for (i = 0; i < numExperts; i++) {
        printf("%i: potential = %lf (%i)\n", i, setOfExperts[i]->getPotential(), (int)(isCongruent(i)));
        if ((setOfExperts[i]->getPotential() >= aspiration) && isCongruent(i)) {
            satisficingExperts[i] = true;
            numSatisficingExperts ++;
        }
        else
            satisficingExperts[i] = false;
    }
    
    if (numSatisficingExperts == 0) {
        // find out which experts are satisficing
        printf("Satisficing Experts: "); fflush(stdout);
        for (i = 0; i < numExperts; i++) {
            //printf("potential = %lf\n", setOfExperts[i]->getPotential());
            if (setOfExperts[i]->getPotential() >= aspiration) {
                numSatisficingExperts ++;
                satisficingExperts[i] = true;
                printf("1");
            }
            else {
                satisficingExperts[i] = false;
                printf("0");
            }
        }
        printf(" (%i)\n", numSatisficingExperts); fflush(stdout);
    }
    
    // randomly pick one of the satisficing experts
    if (numSatisficingExperts == 0) {
        if (experto < 0)
            return 0;
        else
            return experto;
    }
    
    int pick = rand() % numExperts;
    while (!satisficingExperts[pick])
        pick = rand() % numExperts;
    
    delete[] satisficingExperts;
    
    return pick;
}