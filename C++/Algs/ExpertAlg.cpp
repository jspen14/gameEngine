#include "ExpertAlg.h"

ExpertAlg::ExpertAlg() {
    printf("ExpertAlg constructor\n");
}

ExpertAlg::ExpertAlg(int _me, char *playerString) {
    me = _me;

    if (!strcmp("bully", playerString)) {
        setOfExperts = new Expert*[1];
        setOfExperts[0] = new TriggerStrat(me, BULLY1_V, &ignbs, true, 0.25);
        numExperts = 1;
    }
    else if (!strcmp("bullied", playerString)) {
        setOfExperts = new Expert*[1];
        setOfExperts[0] = new TriggerStrat(me, BULLIED1_V, &ignbs, true, 0.25);
        numExperts = 1;
    }
    else if (!strcmp("fair", playerString)) {
        setOfExperts = new Expert*[1];
        setOfExperts[0] = new TriggerStrat(me, NBS_V, &ignbs, true);
        numExperts = 1;
    }
    else if (!strcmp("maxmin", playerString)) {
        setOfExperts = new Expert*[1];
        setOfExperts[0] = new MaxMinExpert(me);
        numExperts = 1;
    }
    else if (!strcmp("listener", playerString)) {
        setOfExperts = new Expert*[1];
        setOfExperts[0] = new ListenerExpert(me, &ignbs);
        numExperts = 1;
    }
    else if (!strcmp("exploiter", playerString)) {
        setOfExperts = new Expert*[1];
        setOfExperts[0] = new ExploiterExpert(me, &ignbs);
        numExperts = 1;
    }
    else if (!strcmp("spp", playerString)) {
        setOfExperts = new Expert*[8];
        setOfExperts[0] = new TriggerStrat(me, BULLY1_V, &ignbs, true, 0.25);
        setOfExperts[1] = new TriggerStrat(me, BULLY1_V, &ignbs, true, 0.80);
        setOfExperts[2] = new TriggerStrat(me, NBS_V, &ignbs, true);
        setOfExperts[3] = new TriggerStrat(me, NBS_V, &ignbs, false);
        setOfExperts[4] = new ListenerExpert(me, &ignbs);
        setOfExperts[5] = new MaxMinExpert(me);
        //setOfExperts[3] = new TriggerStrat(me, BULLY1_V, &ignbs, false, 0.25);
        //setOfExperts[4] = new TriggerStrat(me, BULLY1_V, &ignbs, false, 0.80);
        //setOfExperts[5] = new TriggerStrat(me, NBS_V, &ignbs, false);
        //setOfExperts[0] = new ExploiterExpert(me, &ignbs);
        numExperts = 6;
    }
    else {
        printf("unknown ExpertAlg\n");
        exit(1);
    }

    selMech = new PlusPlusSelector(me, setOfExperts, numExperts);
    //selMech = new RandomSelector(setOfExperts, numExperts);
}

ExpertAlg::~ExpertAlg() {
    printf("ExpertAlg destructor\n");

    for (int i = 0; i < numExperts; i++)
        delete setOfExperts[i];

    delete setOfExperts;
    delete selMech;
}

void ExpertAlg::Reset() {
}

bool ExpertAlg::orient2Game(char *gameString) {

    //printf("gameString: %s\n", gameString); fflush(stdout);

    char gameOverTest[1024];
    strcpy(gameOverTest, gameString);

    char *token = strtok(gameOverTest, "{ \",:");
    char *tipo = strtok(NULL, "{ \",:");
    //printf("tipo: %s\n", tipo);
    if (!strcmp(tipo, "Quit")) {
        return false;
    }

    games[currentTime] = new Game(gameString);

    ignbs.updateWithGame(games[currentTime]);

    int i;
    if (currentTime == 0) {
        // set the initial potential of each expert
        for (i = 0; i < numExperts; i++) {
            setOfExperts[i]->init(games[currentTime]);
        }

    }

    selMech->selectExpert(games[currentTime]);

    // have each expert produce their own plan (they may change this later)
    for (i = 0; i < numExperts; i++) {
        printf("\nExpert %i selectAction (%s):\n", i, setOfExperts[i]->whoAmI());
        setOfExperts[i]->selectAction(games[currentTime]);
    }

    return true;
}

void ExpertAlg::produceStartCheapTalk(char buf[10000]) {
    //strcpy(buf, "--\n");
    //printf("produce start cheaptalk\n"); fflush(stdout);
    selMech->currentExpert->produceStartCheapTalk(buf);
}

void ExpertAlg::produceEndCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void ExpertAlg::processStartCheapTalk(char buf[10000]) {
    // tell all of the experts
    for (int i = 0; i < numExperts; i++)
        setOfExperts[i]->processStartCheapTalk(buf);

    // see if we want to switch experts
    selMech->evaluateProposal(buf);
}

void ExpertAlg::processEndCheapTalk(char buf[10000]) {
}

int ExpertAlg::Move() {
    return selMech->currentExpert->selectedAction;
}

void ExpertAlg::moveUpdate(int actions[2], double dollars[2]) {
    currentTime ++;

    //printf("ExpertAlg moveUpdate\n"); fflush(stdout);

    // we need to update all of the experts
    //printf("Potentials:\n");
    for (int i = 0; i < numExperts; i++) {
        if (i == selMech->experto) {
            printf("** %i: ", i);
            setOfExperts[i]->update(actions, dollars, true);
        }
        else {
            printf("%i: ", i);
            setOfExperts[i]->update(actions, dollars, false);
        }
        //printf("%i: %lf\n", i, setOfExperts[i]->getPotential());
    }

    // update the expert-selection mechanism
    selMech->update(actions, dollars);
}
