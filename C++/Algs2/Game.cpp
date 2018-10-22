#include "Game.h"

Game::Game() {
}

Game::Game(char *entry) {
    printf("entry: %s\n", entry); fflush(stdout);

    char *token = strtok(entry, "{ \",:");
    tipoString = strtok(NULL, " \",:");
    strtok(NULL, " \",:");
    payoffString = strtok(NULL, " \":");

    M = NULL;
    char tmp[1024];
    strcpy(tmp, payoffString);
    printf("tipoString, %s\n", tipoString);
    instantiateGame(tipoString, tmp);

    printf("compute strategies\n"); fflush(stdout);

    minmax[0] = new minimaxLog(A, 0);
    minmax[0]->getMinimax(A, 0, M[0]);
    minmax[1] = new minimaxLog(A, 1);
    minmax[1]->getMinimax(A, 1, M[1]);

    attack[0] = new minimaxLog(A, 0);
    attack[0]->getAttack(A, 0, M[1]);
    attack[1] = new minimaxLog(A, 1);
    attack[1]->getAttack(A, 1, M[0]);
    computePureNEs();
    computeMixedNEs();
    computeLowHighNE();

    printGame();

    printf("done\n"); fflush(stdout);

    nbs = new NBS(M, A);
    printf("NBS = (%lf, %lf)\n", nbs->sol->R[0], nbs->sol->R[1]); fflush(stdout);

    // find all of the 2-solutions that can be sustained as rNE
    //printf("rNE offers:\n");
    computeRNEoffers();
}

Game::~Game() {
	int i, j;

    if (M != NULL) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < A[0]; j++)
                delete M[i][j];
            delete M[i];
        }
        delete M;
    }

    delete minmax[0];
    delete minmax[1];

    //printf("-"); fflush(stdout);

    for (i = 0; i < numSolutions; i++)
        delete gameSolutions[i];
    delete gameSolutions;

    //printf("-"); fflush(stdout);

    for (i = 0; i < numrNEoffers; i++)
        delete rNEoffers[i];
    delete rNEoffers;

    //printf("-"); fflush(stdout);
}

void Game::instantiateGame(char* tipoString, char *payoffString) {
    if (!strncmp(tipoString, "Matrix", 6)) {
        A[0] = tipoString[6] - '0';
        A[1] = tipoString[8] - '0';

        M = new double**[2];
        int i, j;
        for (i = 0; i < 2; i++) {
            M[i] = new double*[A[0]];
            for (j = 0; j < A[0]; j++) {
                M[i][j] = new double[A[0]];
            }
        }

        char *token;
        for (i = 0; i < A[0]; i++) {
            for (j = 0; j < A[1]; j++) {
                if ((i == 0) && (j == 0))
                    M[0][i][j] = atof(strtok(payoffString, "[],"));
                else
                    M[0][i][j] = atof(strtok(NULL, "[],"));
                M[1][i][j] = atof(strtok(NULL, "[],"));
            }
        }

        // set up game solutions
        gameSolutions = new Solution*[A[0]*A[1]];
        numSolutions = 0;
        for (i = 0; i < A[0]; i++) {
            for (j = 0; j < A[1]; j++) {
                gameSolutions[numSolutions] = new Solution(i, j, M[0][i][j], M[1][i][j]);
                numSolutions++;
            }
        }
    }
    else {
        printf("Game of unknown type (%s).  Exiting\n", tipoString);
        exit(1);
    }
}

void Game::printGame() {
    int i, j;

    printf("\n   |      ");

    for (i = 0; i < A[1]; i++)
        printf("%i      |      ", i);
    printf("\n");
    for (i = 0; i < A[0]; i++) {
        printf("--------------------------------------\n %i | ", i);
        for (j = 0; j < A[1]; j++) {
            printf("%.1lf , %.1lf | ", M[0][i][j], M[1][i][j]);
        }
        printf("\n");
    }
    printf("--------------------------------------\n\n");
    printf("Minimax0: %lf: (%lf, %lf)\n", minmax[0]->mv, minmax[0]->ms[0], minmax[0]->ms[1]);
    printf("Minimax1: %lf: (%lf, %lf)\n", minmax[1]->mv, minmax[1]->ms[0], minmax[1]->ms[1]);
    //printf("Attack0: %lf: (%lf, %lf)\n", attack[0]->mv, attack[0]->ms[0], attack[0]->ms[1]);
    //printf("Attack1: %lf: (%lf, %lf)\n", attack[1]->mv, attack[1]->ms[0], attack[1]->ms[1]);
    //printf("Game Solutions:\n");
    //for (i = 0; i < numSolutions; i++) {
    //    gameSolutions[i]->print();
    //}

    printf("pureNEs:  ");
    for (i = 0; i < numPureNEs; i++) {
        printf("(%i, %i); ", pureNEs[i][0], pureNEs[i][1]);
    }
    printf("\n");
    printf("mixedNEs:  ");
    for (i = 0; i < numMixedNEs; i++) {
        printf("(%.2lf, %.2lf) = %lf, %lf; ", mixedNEs[i][0], mixedNEs[i][1], mixedVals[i][0], mixedVals[i][1]);
    }
    printf("\n\n");
}

void Game::createString(char str[1024]) {
    sprintf(str, "{\"type\": \"%s\", \"payoffs\": %s}", tipoString, payoffString);
    //printf("%s\n", str);
}

void Game::computeRNEoffers() {
    int i, j;
    double vals[2];
    int a1[2], a2[2];
    double p1[2], p2[2], w[2] = {0.5, 0.5};

    numrNEoffers = 0;
    rNEoffers = new Solution*[1000]; // make sure I allocate enough memory
    for (i = 0; i < numSolutions; i++) {
        if ((gameSolutions[i]->R[0] >= minmax[0]->mv) && (gameSolutions[i]->R[1] >= minmax[1]->mv)) {
            rNEoffers[numrNEoffers] = new Solution(gameSolutions[i]->actions[0][0], gameSolutions[i]->actions[0][1], gameSolutions[i]->R[0], gameSolutions[i]->R[1]);
            numrNEoffers ++;
        }

        for (j = i+1; j < numSolutions; j++) {
            vals[0] = (gameSolutions[i]->R[0] + gameSolutions[j]->R[0]) / 2.0;
            vals[1] = (gameSolutions[i]->R[1] + gameSolutions[j]->R[1]) / 2.0;
            if ((vals[0] >= minmax[0]->mv) && (vals[1] >= minmax[1]->mv)) {
                a1[0] = gameSolutions[i]->actions[0][0];
                a1[1] = gameSolutions[j]->actions[0][0];
                a2[0] = gameSolutions[i]->actions[0][1];
                a2[1] = gameSolutions[j]->actions[0][1];
                p1[0] = gameSolutions[i]->R[0];
                p1[1] = gameSolutions[j]->R[0];
                p2[0] = gameSolutions[i]->R[1];
                p2[1] = gameSolutions[j]->R[1];

                rNEoffers[numrNEoffers] = new Solution(a1, a2, p1, p2, w);
                numrNEoffers ++;
            }
        }

        if (numrNEoffers > 1000) {
            printf("not enough memory allocated\n");
            exit(1);
        }
    }

    //printf("numrNEoffers: %i\n", numrNEoffers);
    //for (i = 0; i < numrNEoffers; i++) {
    //    rNEoffers[i]->print();
    //}
}

void Game::computePureNEs() {
    numPureNEs = 0;

    int i, j;
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            if (isBestResponse(0, i, j) && isBestResponse(1, i, j)) {
                pureNEs[numPureNEs][0] = i;
                pureNEs[numPureNEs][1] = j;
                numPureNEs++;
            }
        }
    }
}

bool Game::isBestResponse(int index, int a1, int a2) {
    int i;
    double val;

    if (index == 0) {
        val = M[0][a1][a2];
        for (i = 0; i < A[index]; i++) {
            if (i == a1)
                continue;
            if (M[0][i][a2] > val)
                return false;
        }
    }
    else {
        val = M[1][a1][a2];
        for (i = 0; i < A[1-index]; i++) {
            if (i == a2)
                continue;
            if (M[1][a1][i] > val)
                return false;
        }
    }

    return true;
}

void Game::computeMixedNEs() {
    numMixedNEs = 0;

    if (((numPureNEs % 2) == 1) || (A[0] > 2) || (A[1] > 2))
        return;

    mixedNEs[numMixedNEs][0] = (M[1][1][1] - M[1][1][0]) / (M[1][0][0] - M[1][1][0] - M[1][0][1] + M[1][1][1]);
    mixedNEs[numMixedNEs][1] = (M[0][1][1] - M[0][0][1]) / (M[0][0][0] - M[0][0][1] - M[0][1][0] + M[0][1][1]);
    mixedVals[numMixedNEs][0] = (M[0][0][0] * mixedNEs[numMixedNEs][1]) + (M[0][0][1] * (1.0 - mixedNEs[numMixedNEs][1]));
    mixedVals[numMixedNEs][1] = (M[1][0][0] * mixedNEs[numMixedNEs][0]) + (M[1][1][0] * (1.0 - mixedNEs[numMixedNEs][0]));
    numMixedNEs = 1;
}

void Game::computeLowHighNE() {
    double low = 999999, val;
    int i;

    bool includeMixed = true;//false;

    // lowne for player 0
    for (i = 0; i < numPureNEs; i++) {
        val = M[0][pureNEs[i][0]][pureNEs[i][1]];
        if (val < low)
            low = val;
    }

    if (includeMixed) {
        for (i = 0; i < numMixedNEs; i++) {
            if (mixedVals[i][0] < low)
                low = mixedVals[i][0];
        }
    }

    lowne[0] = low;

    // lowne for player 1
    low = 999999;
    for (i = 0; i < numPureNEs; i++) {
        val = M[1][pureNEs[i][0]][pureNEs[i][1]];
        if (val < low)
            low = val;
    }

    if (includeMixed) {
        for (i = 0; i < numMixedNEs; i++) {
            if (mixedVals[i][1] < low)
                low = mixedVals[i][1];
        }
    }

    lowne[1] = low;

    double high = -999999;
    // highne for player 0
    for (i = 0; i < numPureNEs; i++) {
        val = M[0][pureNEs[i][0]][pureNEs[i][1]];
        if (val > high)
            high = val;
    }

    if (includeMixed) {
        for (i = 0; i < numMixedNEs; i++) {
            if (mixedVals[i][0] > high)
                high = mixedVals[i][0];
        }
    }

    highne[0] = high;

    high = -999999;
    // highne for player 1
    for (i = 0; i < numPureNEs; i++) {
        val = M[1][pureNEs[i][0]][pureNEs[i][1]];
        if (val > high)
            high = val;
    }

    if (includeMixed) {
        for (i = 0; i < numMixedNEs; i++) {
            if (mixedVals[i][1] > high)
                high = mixedVals[i][1];
        }
    }

    highne[1] = high;

}
