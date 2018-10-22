#include "GeneralizedFP.h"


GeneralizedFP::GeneralizedFP() {
    printf("incomplete GeF constructor\n");
    exit(1);
}

GeneralizedFP::GeneralizedFP(const char *quien, int _me, bool _asExpert) {
    me = _me;
    asExpert = _asExpert;
    
    numTipos = 0;
    
    if (!strcmp(quien, "GeF")) {
        addTipo(SOCIAL_WELFARE_GUY);
        addTipo(FOLK_EGAL_GUY);
        addTipo(SAFE_GUY);
        addTipo(MAX_GUY);
        addTipo(MAXIMIZER0_GUY);
        addTipo(MAXIMIZER1_GUY);
        addTipo(MAXIMIZER2_GUY);
        //addTipo(ATTACK_GUY);
        //addTipo(CHARITABLE_GUY);
        //addTipo(STACKELBERG_GUY);
    }
    else if (!strcmp(quien, "maximizer1")) {
        addTipo(MAXIMIZER0_GUY);
    }
    else if (!strcmp(quien, "maximizer2")) {
        addTipo(MAXIMIZER1_GUY);
    }
    
    for (int i = 0; i < numTipos; i++) {
        kappa[i] = 1.0;
        for (int j = 0; j < MAX_ACTs; j++) {
            cuenta[i][j] = 1;
        }
    }
    
    for (int i = 0; i < MAX_ACTs; i++)
        numGames[i] = 0;
}

void GeneralizedFP::addTipo(int code) {
    tipos[numTipos] = code;
    numTipos ++;
}

GeneralizedFP::~GeneralizedFP() {
}

void GeneralizedFP::Reset() {
}

bool GeneralizedFP::orient2Game(char *gameString) {
    char gameOverTest[1024];
    strcpy(gameOverTest, gameString);

    char *token = strtok(gameOverTest, "{ \",:");
    char *tipo = strtok(NULL, "{ \",:");
    //printf("tipo: %s\n", tipo);
    if (!strcmp(tipo, "Quit")) {
        return false;
    }

    //printf("gameString: %s\n", gameString);

    games[currentTime] = new Game(gameString);
    
    //ignbs.updateWithGame(games[currentTime]);

    return true;
}

void GeneralizedFP::produceStartCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void GeneralizedFP::processStartCheapTalk(char buf[10000]) {
}

void GeneralizedFP::produceEndCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void GeneralizedFP::processEndCheapTalk(char buf[10000]) {
}

int GeneralizedFP::Move() {
    int a, i, j;
    
    getRankings();
    getGamma();
    
    double *utilities = new double[games[currentTime]->A[me]];
    printf("Utilities:\n");
    for (a = 0; a < games[currentTime]->A[me]; a++) {
        utilities[a] = 0.0;
        for (j = 0; j < games[currentTime]->A[1-me]; j++) {
            if (me == 0)
                utilities[a] += gamma[j] * games[currentTime]->M[me][a][j];
            else
                utilities[a] += gamma[j] * games[currentTime]->M[me][j][a];
        }
        printf("   %i: %lf\n", a, utilities[a]);
    }
    
    // pick the action with the highest utility (break ties with uniform random)
    double max = -999999, val;
    int num = 0;
    int inthere[100];

    for (a = 0; a < games[currentTime]->A[me]; a++) {
        val = utilities[a];
        if (val > max) {
            max = val;
            num = 1;
            inthere[0] = a;
        }
        else if (val == max) {
            inthere[num] = a;
            num ++;
        }
    }
    
    maxUtil = max;

    delete[] utilities;
    
    int select = inthere[rand() % num];
    printf("GeF: selected action %i\n", select);

    return select;
}

void GeneralizedFP::moveUpdate(int actions[2], double dineros[2]) {

    // update cuenta
    //printf("update cuenta\n"); fflush(stdout);
    int i, index;
    for (i = 0; i < numTipos; i++) {
        index = findCorrespondence(i, actions[1-me]);
        cuenta[i][index] ++;
    }
    
    printf("Cuenta:\n");
    for (i = 0; i < numTipos; i++) {
        printf("Tipo %i: ", i);
        for (int j = 0; j < games[currentTime]->A[1-me]; j++) {
            printf("%i ", cuenta[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Cuenta Expanded:\n");
    double v;
    for (i = 0; i < numTipos; i++) {
        printf("Tipo %i: ", i);
        for (int j = 0; j < 5; j++) {
            v = cuenta[i][j];
            if (numGames[j] > 0)
                v = 1.0 + ((v - 1.0) / (numGames[j])) * numGames[0];
            printf("%.1lf (%i) ", v, numGames[j]);
        }
        printf("\n");
    }
    printf("\n");

    
    for (i = 0; i < games[currentTime]->A[me]; i++)
        numGames[i] ++;
    
    printf("numGames: ");
    for (i = 0; i < MAX_ACTs; i++) {
        printf("%i ", numGames[i]);
    }
    printf("\n");

    currentTime ++;
}

int GeneralizedFP::findCorrespondence(int tipo, int himAct) {
    int i;
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        if (theRankings[tipo][i] == himAct)
            return i;
    }

    return -1;
}

void GeneralizedFP::getGamma() {
    int i, j;
    double sum = 0.0;

    // compute the weights
    double *weights = new double[numTipos];
/*
    // weighted kappa
    for (i = 0; i < numTipos; i++) {
        weights[i] = getSignalQuality(i);
        sum += weights[i];
    }
    printf("Weights: ");
    for (i = 0; i < numTipos; i++) {
        weights[i] /= sum;
        printf("%lf ", weights[i]);
    }
    printf("\n");
*/

    // best takes all kapp
    double mx = -99999;
    int index;
    for (i = 0; i < numTipos; i++) {
        weights[i] = 0.0;
        if (getSignalQuality(i) > mx) {
            mx = getSignalQuality(i);
            index = i;
        }
    }
    weights[index] = 1.0;
    printf("Best model = %i\n", index);
    
    // compute kappa
    for (j = 0; j < games[currentTime]->A[1-me]; j++) {
        kappa[j] = 0.0;
    }

    for (j = 0; j < games[currentTime]->A[1-me]; j++) {
        kappa[j] += cuenta[index][findCorrespondence(index, j)];
        
        // account for games that had more than games[currentTime]->A[1-me] actions
        
        
        // account for games that had less than j actions
        if (numGames[j] > 0)
            kappa[j] = 1.0 + ((kappa[j] - 1.0) / (numGames[j])) * numGames[0];
    }
/*
    for (i = 0; i < numTipos; i++) {
        for (j = 0; j < games[currentTime]->A[1-me]; j++) {
            kappa[j] += weights[i] * cuenta[i][theRankings[i][j]];
        }
    }
*/
    printf("kappa: ");
    for (j = 0; j < games[currentTime]->A[1-me]; j++) {
        printf("%lf ", kappa[j]);
    }
    printf("\n");
    
    // compute gamma
    sum = 0.0;
    for (j = 0; j < games[currentTime]->A[1-me]; j++) {
        sum += kappa[j];
    }
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        gamma[i] = kappa[i] / sum;
    }
    
    printf("Assessment: ");
    for (j = 0; j < games[currentTime]->A[1-me]; j++) {
        printf("%lf ", gamma[j]);
    }
    printf("\n");
    
    delete[] weights;
}

double GeneralizedFP::getSignalQuality(int index) {
    int i;
    double sum = 0;
    double epsilon = 0.1;
    
    double poder = 4.0;// + (currentTime / 50.0);
    
    for (i = 1; i < games[currentTime]->A[1-me]; i++) {
        sum += pow((double)(abs(cuenta[index][i] - cuenta[index][0])), poder);
    }
    
    return sum + epsilon;
}

void GeneralizedFP::getRankings() {
    int i;
    
    for (i = 0; i < numTipos; i++) {
        switch (tipos[i]) {
            case SOCIAL_WELFARE_GUY: rankSocialWelfare(theRankings[i]); break;
            case FOLK_EGAL_GUY: rankFolkEgal(theRankings[i]); break;
            case SAFE_GUY: rankSafe(theRankings[i]); break;
            case MAX_GUY: rankMax(theRankings[i]); break;
            case MAXIMIZER0_GUY: rankMaximizer0(theRankings[i]); break;
            case MAXIMIZER1_GUY: rankMaximizer1(theRankings[i]); break;
            case MAXIMIZER2_GUY: rankMaximizer2(theRankings[i]); break;
            //case ATTACK_GUY: rankAttack(theRankings[i]); break;
            //case CHARITABLE_GUY: rankCharity(theRankings[i]); break;
            //case STACKELBERG_GUY: rankStackelberg(theRankings[i]); break;
            default: printf("getRankings: unknown tipo\n"); break;
        }
    }
    
    printf("\n");
}

void GeneralizedFP::sortEm(double utils[MAX_ACTs], int rankings[MAX_ACTs]) {
    int i, j;
    double tmp;
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++)
        rankings[i] = i;
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        for (j = 0; j < games[currentTime]->A[1-me]-1; j++) {
            if ((utils[rankings[j]] < utils[rankings[j+1]]) || ((utils[rankings[j]] == utils[rankings[j+1]]) && (rand() % 2))) {
                tmp = rankings[j];
                rankings[j] = rankings[j+1];
                rankings[j+1] = tmp;
            }
        }
    }
}

void GeneralizedFP::rankSocialWelfare(int rankings[MAX_ACTs]) {
    int i, j;
    double max, val, utils[MAX_ACTs];
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        max = -999999;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                val = games[currentTime]->M[0][i][j] + games[currentTime]->M[1][i][j];
            else
                val = games[currentTime]->M[0][j][i] + games[currentTime]->M[1][j][i];
            
            if (val > max)
                max = val;
        }
        utils[i] = max;
    }
    
    // sort them
    sortEm(utils, rankings);

    printf("SocialWelfare rankings: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%i (%.2lf) ", rankings[i], utils[rankings[i]]);
    }
    printf("\n");
}

void GeneralizedFP::rankFolkEgal(int rankings[MAX_ACTs]) {
    int i, j;
    double max, val, utils[MAX_ACTs];
    
    for (i = 0; i < games[currentTime]->A[0]; i++) {
        max = -999999;
        for (j = 0; j < games[currentTime]->A[1]; j++) {
            val = games[currentTime]->M[0][i][j];
            if (games[currentTime]->M[1][i][j] < val)
                val = games[currentTime]->M[1][i][j];
            
            if (val > max)
                max = val;
        }
        utils[i] = max;
    }
    
    // sort them
    sortEm(utils, rankings);

    printf("FolkEgal rankings: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%i (%.2lf) ", rankings[i], utils[rankings[i]]);
    }
    printf("\n");
}

void GeneralizedFP::rankSafe(int rankings[MAX_ACTs]) {
    int i, j;
    double min, val, utils[MAX_ACTs];
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        min = 999999;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                val = games[currentTime]->M[1-me][i][j];
            else
                val = games[currentTime]->M[1-me][j][i];
            
            if (val < min)
                min = val;
        }
        utils[i] = min;
    }

    // sort them
    sortEm(utils, rankings);

    printf("Safe rankings: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%i (%.2lf) ", rankings[i], utils[rankings[i]]);
    }
    printf("\n");
}

void GeneralizedFP::rankMax(int rankings[MAX_ACTs]) {
    int i, j;
    double max, val, utils[MAX_ACTs];
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        max = -999999;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                val = games[currentTime]->M[1-me][i][j];
            else
                val = games[currentTime]->M[1-me][j][i];
            
            if (val > max)
                max = val;
        }
        utils[i] = max;
    }
    
    // sort them
    sortEm(utils, rankings);

    printf("Max rankings: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%i (%.2lf) ", rankings[i], utils[rankings[i]]);
    }
    printf("\n");
}

void GeneralizedFP::bestResponse(int index, double partStrat[MAX_ACTs], double strat[MAX_ACTs]) {
    int i, j;
    double max = -999999, val, utils[MAX_ACTs];
    int num = 0;
    int inthere[100];
    
    for (i = 0; i < games[currentTime]->A[index]; i++) {
        val = 0.0;
        for (j = 0; j < games[currentTime]->A[1-index]; j++) {
            if (index == 0)
                val += games[currentTime]->M[index][i][j] * partStrat[j];
            else
                val += games[currentTime]->M[index][j][i] * partStrat[j];
        }
        if (val > max) {
            max = val;
            num = 1;
            inthere[0] = i;
        }
        else if (val == max) {
            inthere[num] = i;
            num ++;
        }
    }
    
    for (i = 0; i < games[currentTime]->A[index]; i++) {
        strat[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        strat[inthere[i]] += 1.0 / num;
    }
}

void GeneralizedFP::rankMaximizer0(int rankings[MAX_ACTs]) {
    int i, j;
    double utils[MAX_ACTs];
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        utils[i] = 0.0;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                utils[i] += games[currentTime]->M[1-me][i][j];
            else
                utils[i] += games[currentTime]->M[1-me][j][i];
        }
    }
    
    // sort them
    sortEm(utils, rankings);

    printf("Maximizer0 rankings: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%i (%.2lf) ", rankings[i], utils[rankings[i]]);
    }
    printf("\n");
}

void GeneralizedFP::rankMaximizer1(int rankings[MAX_ACTs]) {
    int i, j;
    double val, utils[MAX_ACTs];

    double tmp[MAX_ACTs];
    double him[MAX_ACTs];
    for (i = 0; i < games[currentTime]->A[1-me]; i++)
        him[i] = 1.0 / games[currentTime]->A[1-me];
    bestResponse(me, him, tmp);
    
    printf("\t%i (I) will play %lf, %lf, so ... \n", me, tmp[0], tmp[1]);

    // now calculate what he'll do if he's rational
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        val = 0.0;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                val += games[currentTime]->M[1-me][i][j] * tmp[j];
            else
                val += games[currentTime]->M[1-me][j][i] * tmp[j];
        }
        utils[i] = val;
    }
    
    // sort them
    sortEm(utils, rankings);

    printf("Maximizer1 rankings: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%i (%.2lf) ", rankings[i], utils[rankings[i]]);
    }
    printf("\n");
}

void GeneralizedFP::rankMaximizer2(int rankings[MAX_ACTs]) {
    int i, j;
    double val, utils[MAX_ACTs];
    double tmp[MAX_ACTs];
    double him[MAX_ACTs];
    
    for (i = 0; i < games[currentTime]->A[me]; i++)
        tmp[i] = 1.0 / games[currentTime]->A[me];
    bestResponse(1-me, tmp, him);
    
    printf("\t%i will play %lf, %lf ... ", 1-me, him[0], him[1]);
    
    bestResponse(me, him, tmp);
    
    printf("so %i (I) will play %lf, %lf\n", me, tmp[0], tmp[1]);

    // now calculate what he'll do if he's rational
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        val = 0.0;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                val += games[currentTime]->M[1-me][i][j] * tmp[j];
            else
                val += games[currentTime]->M[1-me][j][i] * tmp[j];
        }
        utils[i] = val;
    }
    
    // sort them
    sortEm(utils, rankings);

    printf("Maximizer2 rankings: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%i (%.2lf) ", rankings[i], utils[rankings[i]]);
    }
    printf("\n");
}

// ****************************************
//
//      Functions to implement Expert
//
// ****************************************

void GeneralizedFP::init(Game *_g) {}

double GeneralizedFP::getPotential() {
    if (currentTime != 0)
        return potential / currentTime;
    else
        return potential;
}

void GeneralizedFP::selectAction(Game *_g) {
    games[currentTime] = _g;
    
    selectedAction = Move();
}

void GeneralizedFP::update(int acts[2], double payoffs[2], bool active) {
    moveUpdate(acts, payoffs);
    
    potential += maxUtil;
    internalExpectationTally += maxUtil;
    printf("GeF potential: %lf\n", getPotential());
}

const char *GeneralizedFP::whoAmI() {
    return "GeF";
}

bool GeneralizedFP::matchesPlan(int proposal[2]) {
    return false;
}




