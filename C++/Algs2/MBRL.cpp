#include "MBRL.h"

MBRL::MBRL() {
    printf("incomplete MBRL constructor\n");
    exit(1);
}

MBRL::MBRL(int _me) {
    int i, j, k;
    
    me = _me;
    
    myLastMove = hisLastMove = -1;
    
    sampleCount = 0;
    lowVal = highVal = 0.0;
    kappa = new int**[NUM_BINS];
    V = new double**[NUM_BINS];
    newV = new double**[NUM_BINS];
    for (i = 0; i < NUM_BINS; i++) {
        kappa[i] = new int*[NUM_BINS];
        V[i] = new double*[NUM_BINS];
        newV[i] = new double*[NUM_BINS];
        for (j = 0; j < NUM_BINS; j++) {
            kappa[i][j] = new int[NUM_BINS];
            V[i][j] = new double[NUM_BINS];
            newV[i][j] = new double[NUM_BINS];
            for (k = 0; k < NUM_BINS; k++) {
                kappa[i][j][k] = 1;
                V[i][j][k] = 0.0;
                newV[i][j][k] = 0.0;
            }
        }
    }
}

MBRL::~MBRL() {
    int i, j, k;
    
    for (i = 0; i < NUM_BINS; i++) {
        for (j = 0; j < NUM_BINS; j++) {
            delete kappa[i][j];
            delete V[i][j];
            delete newV[i][j];
        }
        delete kappa[i];
        delete V[i];
        delete newV[i];
    }
    delete kappa;
    delete V;
    delete newV;
}

void MBRL::Reset() {
}

bool MBRL::orient2Game(char *gameString) {
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

void MBRL::produceStartCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void MBRL::processStartCheapTalk(char buf[10000]) {
}

void MBRL::produceEndCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void MBRL::processEndCheapTalk(char buf[10000]) {
}

int MBRL::Move() {
    int a;
    
    double gamma = 0.95;
    
    if (sampleCount == 0) {
        // play randomly
        a = rand() % games[currentTime]->A[me];
    }
    else {
        int i, j, c = 0;
        for (i = 0; i < NUM_BINS; i++) {
            c += kappa[myLastMove][hisLastMove][i];
        }
        printf("c = %i\n", c);
        
        double U[10], h, l, inc, R, prob;
        printf("Utilities:\n");
        for (i = 0; i < games[currentTime]->A[me]; i++) {
            U[i] = 0.0;
            R = 0.0;
            l = getLowV(me, i);
            h = getHighV(me, i);
            inc = (double)(h - l) / NUM_BINS;
            printf("probs (%lf): ", l);
            for (j = 0; j < NUM_BINS; j++) {
                prob = (kappa[myLastMove][hisLastMove][j] / ((double)c));
                printf("%lf  ", prob);
                R += prob * (l + (inc * j));
                U[i] += prob * (gamma * maxV(i, j));
            }
            U[i] += R;
            printf("\n");
            printf("%i: %lf; %lf\n", i, R, U[i]);
        }
        printf("\n");
    
        double explore = 1.0 / (1.0 + (currentTime/5.0));
        printf("explore (%i) = %lf\n", currentTime, explore);
        if (rand() % 1000 < (explore*1000)) {
            printf("random move\n");
            a = rand() % games[currentTime]->A[me];
        }
        else {
            double mx = -99999;
            for (i = 0; i < games[currentTime]->A[me]; i++) {
                if (U[i] > mx) {
                    a = i;
                    mx = U[i];
                }
            }
        }
        //a = rand() % games[currentTime]->A[me];
    }
    
    return a;
}

void MBRL::moveUpdate(int actions[2], double dineros[2]) {
    int myNewMove = getMove(me, actions);
    int hisNewMove = getMove(1-me, actions);
    printf("Moves: %i, %i\n", myNewMove, hisNewMove);

    if (sampleCount > 0)
        kappa[myLastMove][hisLastMove][hisNewMove]++;

    sampleCount++;
    lowVal += getLowV(me, actions[me]);
    highVal += getHighV(me, actions[me]);

    printf("low-high: %lf -- %lf\n", lowVal / sampleCount, highVal / sampleCount);

    myLastMove = myNewMove;
    hisLastMove = hisNewMove;

    // update V
    valueIteration();
    
    currentTime ++;    
}

int MBRL::getMove(int index, int actions[2]) {
    double low = getLowV(1-index, actions[1-index]);
    double high = getHighV(1-index, actions[1-index]);
    
    if (low == high)
        return NUM_BINS / 2;
    
    double actual = games[currentTime]->M[1-index][actions[0]][actions[1]];
    double ratio = (actual - low) / (high - low);

    return (int)(ratio * (NUM_BINS-1) + 0.5);
}

double MBRL::getLowV(int index, int a) {
    double low = 999999;
    int i;
    
    for (i = 0; i < games[currentTime]->A[1-index]; i++) {
        if (index == 0) {
            if (games[currentTime]->M[index][a][i] < low)
                low = games[currentTime]->M[index][a][i];
        }
        else {
            if (games[currentTime]->M[index][i][a] < low)
                low = games[currentTime]->M[index][i][a];
        }
    }

    return low;
}

double MBRL::getHighV(int index, int a) {
    double high = -999999;
    int i;
    
    for (i = 0; i < games[currentTime]->A[1-index]; i++) {
        if (index == 0) {
            if (games[currentTime]->M[index][a][i] > high)
                high = games[currentTime]->M[index][a][i];
        }
        else {
            if (games[currentTime]->M[index][i][a] > high)
                high = games[currentTime]->M[index][i][a];
        }
    }

    return high;
}

void MBRL::valueIteration() {
    while (vIterate() > 0.1);

    int i, j, k;
    printf("Kappa:\n");
    for (i = 0; i < NUM_BINS; i++) {
        for (j = 0; j < NUM_BINS; j++) {
            printf("%i, %i: ", i, j);
            for (k = 0; k < NUM_BINS; k++) {
                printf("%i  ", kappa[i][j][k]);
            }
            printf("\n");
        }
    }
    printf("\n\n");

    printf("V:\n");
    for (i = 0; i < NUM_BINS; i++) {
        for (j = 0; j < NUM_BINS; j++) {
            printf("%i, %i: ", i, j);
            for (k = 0; k < NUM_BINS; k++) {
                printf("%.3lf\t", V[i][j][k]);
            }
            printf("\n");
        }
    }
    printf("\n\n");
}

double MBRL::vIterate() {
    int i, j, k, m;
    double R, prob, val;
    
    double gamma = 0.95;
    
    for (i = 0; i < NUM_BINS; i++) {
        for (j = 0; j < NUM_BINS; j++) {
            R = getR(i, j);
            for (k = 0; k < NUM_BINS; k++) {
                val = 0.0;
                for (m = 0; m < NUM_BINS; m++) {
                    prob = getProb(i, j, m);
                    val += prob * maxV(k, m);
                }
                newV[i][j][k] = R + gamma * val;
            }
        }
    }

    double cambio = 0.0;
    //printf("V:\n");
    for (i = 0; i < NUM_BINS; i++) {
        for (j = 0; j < NUM_BINS; j++) {
            //printf("%i, %i: ", i, j);
            for (k = 0; k < NUM_BINS; k++) {
                cambio += fabs(V[i][j][k] - newV[i][j][k]);
                V[i][j][k] = newV[i][j][k];
                //printf("%.3lf\t", V[i][j][k]);
            }
            //printf("\n");
        }
    }
    //printf("\n\n");
    
    return cambio;
}

double MBRL::maxV(int yo, int el) {
    double mx = -99999;
    int i;
    for (i = 0; i < NUM_BINS; i++) {
        if (V[yo][el][i] > mx)
            mx = V[yo][el][i];
    }
    
    return mx;
}

double MBRL::getProb(int lMe, int lHim, int aHim) {
    int c = 0;
    int i;
    for (i = 0; i < NUM_BINS; i++) {
        c += kappa[lMe][lHim][i];
    }
    
    return ((double)kappa[lMe][lHim][aHim]) / c;
}

double MBRL::getR(int lMe, int lHim) {
    int c = 0;
    int i;
    for (i = 0; i < NUM_BINS; i++) {
        c += kappa[lMe][lHim][i];
    }
    double val = 0.0;
    for (i = 0; i < NUM_BINS; i++) {
        val += valor(i) * ((double)kappa[lMe][lHim][i]) / c;
    }
    
    return val;
}

double MBRL::valor(int bin) {
    double low = lowVal / sampleCount;
    double high = highVal / sampleCount;
    
    return ((double)bin / (NUM_BINS-1) * (high - low)) + low;
}



