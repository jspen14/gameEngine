#include "GeneralizedFP.h"


GeneralizedFP::GeneralizedFP() {
    printf("incomplete GeF constructor\n");
    exit(1);
}

GeneralizedFP::GeneralizedFP(int _me) {
    me = _me;
    
    numTipos = 0;
    addTipo(SOCIAL_WELFARE_GUY);
    addTipo(FOLK_EGAL_GUY);
    addTipo(SAFE_GUY);
    addTipo(MAX_GUY);
    addTipo(MAXIMIZER0_GUY);
    addTipo(MAXIMIZER1_GUY);
    addTipo(ATTACK_GUY);
    addTipo(CHARITABLE_GUY);
    addTipo(STACKELBERG_GUY);
    addTipo(RANDOM_GUY);

    for (int i = 0; i < numTipos; i++) {
        kappa[i] = 1.0;
    }
    
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
    
    getGamma();
    getAcciones();

    double *actionGamma = new double[games[currentTime]->A[1-me]];
    for (a = 0; a < games[currentTime]->A[1-me]; a++) {
        actionGamma[a] = 0.0;
    }
    
    for (i = 0; i < numTipos; i++) {
        for (a = 0; a < games[currentTime]->A[1-me]; a++) {
            actionGamma[a] += gamma[i] * acciones[i][a];
        }
    }

    printf("Gamma: ");
    for (a = 0; a < games[currentTime]->A[1-me]; a++) {
        printf("%lf ", actionGamma[a]);
    }
    printf("\n");
    
    double *utilities = new double[games[currentTime]->A[me]];
    printf("Utilities (new):\n");
    for (a = 0; a < games[currentTime]->A[me]; a++) {
        utilities[a] = 0.0;
        for (j = 0; j < games[currentTime]->A[1-me]; j++) {
            if (me == 0)
                utilities[a] += actionGamma[j] * games[currentTime]->M[me][a][j];
            else
                utilities[a] += actionGamma[j] * games[currentTime]->M[me][j][a];
        }
        printf("   %i: %lf\n", a, utilities[a]);
    }

// ******************* old start ***********************
/*
    // estimate the utility of each action
    double *utilities = new double[games[currentTime]->A[me]];
    double sum;
    
    printf("Utilities (old):\n");
    for (a = 0; a < games[currentTime]->A[me]; a++) {
        utilities[a] = 0.0;
        for (i = 0; i < numTipos; i++) {
            sum = 0.0;
            for (j = 0; j < games[currentTime]->A[1-me]; j++) {
                if (me == 0)
                    sum += acciones[i][j] * games[currentTime]->M[me][a][j];
                else
                    sum += acciones[i][j] * games[currentTime]->M[me][j][a];
            }
            utilities[a] += sum * gamma[i];
        }
        printf("   %i: %lf\n", a, utilities[a]);
    }
*/
// *******************  old end  ***********************
    
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

    delete[] utilities;
    delete[] actionGamma;
    
    int select = inthere[rand() % num];
    printf("selected action %i\n", select);

    return select;
}

void GeneralizedFP::moveUpdate(int actions[2], double dineros[2]) {
    // find the tipos that correctly predicted his action
    // spread the weight of 1.0 around to those that correctly predicted the action (gamma[i] * acciones[i][selected])
    
    double weight[MAX_GUYS];
    int i;
    double mag = 0.0;
    for (i = 0; i < numTipos; i++) {
        weight[i] = gamma[i] * acciones[i][actions[1-me]];
        mag += weight[i];
    }
    
    printf("mag = %lf\n", mag);
    if (mag > 0.0) {
        printf("kappa: ");
        for (i = 0; i < numTipos; i++) {
            kappa[i] += weight[i] / mag;
            printf("%.2lf  ", kappa[i]);
        }
        printf("\n");
    }
    
    currentTime ++;
}

void GeneralizedFP::getGamma() {
    double sum = 0.0;
    int i;
    
    for (i = 0; i < numTipos; i++) {
        sum += kappa[i];
    }

    for (i = 0; i < numTipos; i++) {
        gamma[i] = kappa[i] / sum;
    }
    
    printf("Assessment:\n");
    for (i = 0; i < numTipos; i++) {
        printf("Tipo %i: %lf (%lf)\n", i, gamma[i], kappa[i]);
    }
}

void GeneralizedFP::getAcciones() {
    int i;
    
    for (i = 0; i < numTipos; i++) {
        switch (tipos[i]) {
            case SOCIAL_WELFARE_GUY: accionesSocialWelfare(acciones[i]); break;
            case FOLK_EGAL_GUY: accionesFolkEgal(acciones[i]); break;
            case SAFE_GUY: accionesSafe(acciones[i]); break;
            case MAX_GUY: accionesMax(acciones[i]); break;
            case MAXIMIZER0_GUY: accionesMaximizer0(acciones[i]); break;
            case MAXIMIZER1_GUY: accionesMaximizer1(acciones[i]); break;
            case ATTACK_GUY: accionesAttack(acciones[i]); break;
            case CHARITABLE_GUY: accionesCharity(acciones[i]); break;
            case STACKELBERG_GUY: accionesStackelberg(acciones[i]); break;
            case RANDOM_GUY: accionesRandom(acciones[i]); break;
            default: printf("getAcciones: unknown tipo\n"); break;
        }
    }
    
    printf("\n");
}

void GeneralizedFP::accionesSocialWelfare(double acts[MAX_ACTs]) {
    int i, j;
    double max = -999999, val;
    int num = 0;
    int inthere[100];
    
    for (i = 0; i < games[currentTime]->A[0]; i++) {
        for (j = 0; j < games[currentTime]->A[1]; j++) {
            val = games[currentTime]->M[0][i][j] + games[currentTime]->M[1][i][j];
            if (val > max) {
                max = val;
                num = 1;
                if (me == 0)
                    inthere[0] = j;
                else
                    inthere[0] = i;
            }
            else if (val == max) {
                if (me == 0)
                    inthere[num] = j;
                else
                    inthere[num] = i;
                num ++;
            }
        }
    }
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        acts[inthere[i]] += 1.0 / num;
    }

    printf("SocialWelfare action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesFolkEgal(double acts[MAX_ACTs]) {
    int i, j;
    double max = -999999, val;
    int num = 0;
    int inthere[100];
    
    for (i = 0; i < games[currentTime]->A[0]; i++) {
        for (j = 0; j < games[currentTime]->A[1]; j++) {
            val = games[currentTime]->M[0][i][j];
            if (games[currentTime]->M[1][i][j] < val)
                val = games[currentTime]->M[1][i][j];
            if (val > max) {
                max = val;
                num = 1;
                if (me == 0)
                    inthere[0] = j;
                else
                    inthere[0] = i;
            }
            else if (val == max) {
                if (me == 0)
                    inthere[num] = j;
                else
                    inthere[num] = i;
                num ++;
            }
        }
    }
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        acts[inthere[i]] += 1.0 / num;
    }

    printf("FolkEgal action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesSafe(double acts[MAX_ACTs]) {
    int i;
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++)
        acts[i] = games[currentTime]->minmax[1-me]->ms[i];

    printf("Safe action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesMax(double acts[MAX_ACTs]) {
    int i, j;
    double max = -999999, val;
    int num = 0;
    int inthere[100];
    
    for (i = 0; i < games[currentTime]->A[0]; i++) {
        for (j = 0; j < games[currentTime]->A[1]; j++) {
            val = games[currentTime]->M[1-me][i][j];
            if (val > max) {
                max = val;
                num = 1;
                if (me == 0)
                    inthere[0] = j;
                else
                    inthere[0] = i;
            }
            else if (val == max) {
                if (me == 0)
                    inthere[num] = j;
                else
                    inthere[num] = i;
                num ++;
            }
        }
    }
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        acts[inthere[i]] += 1.0 / num;
    }

    printf("Max action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesMaximizer0(double acts[MAX_ACTs]) {
    int i, j;
    double max = -999999, val;
    int num = 0;
    int inthere[100];
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        val = 0.0;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                val += games[currentTime]->M[1-me][i][j];
            else
                val += games[currentTime]->M[1-me][j][i];
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
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        acts[inthere[i]] += 1.0 / num;
    }

    printf("Maximizer0 action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesMaximizer1(double acts[MAX_ACTs]) {
    int i, j;
    double max = -999999, val;
    int num = 0;
    int inthere[100];
    
    printf("        HeVals: ");
    for (i = 0; i < games[currentTime]->A[me]; i++) {
        val = 0.0;
        for (j = 0; j < games[currentTime]->A[1-me]; j++) {
            if (me == 0)
                val += games[currentTime]->M[me][i][j];
            else
                val += games[currentTime]->M[me][j][i];
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
        printf("%lf ", val);
    }
    printf("\n");
    
    double tmp[MAX_ACTs];
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        tmp[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        tmp[inthere[i]] += 1.0 / num;
    }
    
    // tmp now specifies the strategy he estimates I'll do if I'm a Maximizer0
    printf("\tHe thinks I'll play: ");
    for (i = 0; i < games[currentTime]->A[me]; i++) {
        printf("%.2lf ", tmp[i]);
    }
    printf("\n");


    // now calculate what he'll do if he's rational
    max = -99999;
    num = 0;
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        val = 0.0;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1)
                val += games[currentTime]->M[1-me][i][j] * tmp[j];
            else
                val += games[currentTime]->M[1-me][j][i] * tmp[j];
        }
        //printf("%i: %lf\n", i, val);
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
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        acts[inthere[i]] += 1.0 / num;
    }
    
    printf("Maximizer1 action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesAttack(double acts[MAX_ACTs]) {
    int i;
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++)
        acts[i] = games[currentTime]->attack[1-me]->ms[i];

    printf("Attack action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesCharity(double acts[MAX_ACTs]) {
    int i, j;
    double max = -999999, val;
    int num = 0;
    int inthere[100];
    
    for (i = 0; i < games[currentTime]->A[0]; i++) {
        for (j = 0; j < games[currentTime]->A[1]; j++) {
            val = games[currentTime]->M[me][i][j];
            if (val > max) {
                max = val;
                num = 1;
                if (me == 0)
                    inthere[0] = j;
                else
                    inthere[0] = i;
            }
            else if (val == max) {
                if (me == 0)
                    inthere[num] = j;
                else
                    inthere[num] = i;
                num ++;
            }
        }
    }
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        acts[inthere[i]] += 1.0 / num;
    }

    printf("Charity action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesStackelberg(double acts[MAX_ACTs]) {
    int i, j;
    double max = -999999, val, valme;
    int num = 0;
    int inthere[100];
    
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        val = 0.0;
        valme = -99999;
        for (j = 0; j < games[currentTime]->A[me]; j++) {
            if (me == 1) {
                if (games[currentTime]->M[me][i][j] > valme) {
                    val = games[currentTime]->M[1-me][i][j];
                    valme = games[currentTime]->M[me][i][j];
                }
            }
            else {
                if (games[currentTime]->M[me][j][i] > valme) {
                    val = games[currentTime]->M[1-me][j][i];
                    valme = games[currentTime]->M[me][j][i];
                }
            }
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

    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 0.0;
    }
    
    for (i = 0; i < num; i++) {
        acts[inthere[i]] += 1.0 / num;
    }

    printf("Stackelberg action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
}

void GeneralizedFP::accionesRandom(double acts[MAX_ACTs]) {
    int i;
    
    printf("Random action probs: ");
    for (i = 0; i < games[currentTime]->A[1-me]; i++) {
        acts[i] = 1.0 / games[currentTime]->A[1-me];
        printf("%.2lf ", acts[i]);
    }
    printf("\n");
    
}

