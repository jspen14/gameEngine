#include "TriggerStrat.h"

TriggerStrat::TriggerStrat() {
    printf("incomplete TriggerStrat constructor\n");
}

TriggerStrat::TriggerStrat(int _me, int _targetType, double _coef, double _gamma) {
    me = _me;
    targetType = _targetType;
    coef = _coef;
    
    printf("gamma = %lf\n", _gamma);
    gamma = _gamma;

    t = 0;
    Reset();
}

TriggerStrat::~TriggerStrat() {
}

void TriggerStrat::init(Game *_g) {
    determineValors(_g);
    potential = currentValor[me];
}

void TriggerStrat::Reset() {
    D_t[0] = D_t[1] = 0.0;
    //potential = -99999;
}

double TriggerStrat::getPotential() {
    if (t != 0)
        return potential / t;
    else
        return potential;
}

// assume the rewards are no lower than 0
int TriggerStrat::selectAction(Game *_g) {
    g = _g;
    
    // compute currentValor for this game (if t == 0, we should have already done that)
    if (t != 0)
        determineValors(g);
    printf("currentValor: %.2lf, %.2lf\n", currentValor[0], currentValor[1]);
    
    // compute jaUtility for each ja
    numJAs = g->A[0] * g->A[1];
    int i, j;
    double Dhat1, Dhat2;
    for (i = 0; i < g->A[0]; i++) {
        for (j = 0; j < g->A[1]; j++) {
            Dhat1 = D_t[0] + (g->M[0][i][j] - currentValor[0]);
            Dhat2 = D_t[1] + (g->M[1][i][j] - currentValor[1]);

            jaUtility[i][j] = pow((g->M[0][i][j] + g->M[1][i][j]), coef);  // divide by weighted social welfare
            jaUtility[i][j] /= (Dhat1*Dhat1 + Dhat2*Dhat2) + 1.0;
            //jaUtility[i][j] /= fabs(pow(Dhat1 - Dhat2, 1.0)) + 1.0;
            //jaUtility[i][j] /= (fabs(Dhat1) + fabs(Dhat2)) + 1.0;
            
            printf("%i, %i: %lf (dhat: %lf, %lf)\n", i, j, jaUtility[i][j], Dhat1, Dhat2);
        }
    }
    
    // now pick the joint action
    int **best = new int*[2];
    best[0] = new int[g->A[0]*g->A[1]];
    best[1] = new int[g->A[0]*g->A[1]];
    int numBest = 0;
    double mejor = -99999;
    for (i = 0; i < g->A[0]; i++) {
        for (j = 0; j < g->A[1]; j++) {
            if (jaUtility[i][j] > mejor) {
                mejor = jaUtility[i][j];
                best[0][0] = i;
                best[1][0] = j;
                numBest = 1;
            }
            else if (jaUtility[i][j] == mejor) {
                printf("tie\n");
                best[0][numBest] = i;
                best[1][numBest] = j;
                numBest++;
            }
        }
    }
    
    printf("numBest = %i\n", numBest);
    
    int sel = rand()%numBest;
    int a = best[me][sel];
    printf("selected action %i after selecting solution %i, %i\n", a, best[0][sel], best[1][sel]);
    
    delete best[0];
    delete best[1];
    delete[] best;
    
    return a;
}

void TriggerStrat::update(int acts[2], double payoffs[2], bool active) {
    // update debt (D_t)
    if (active) {
        D_t[0] = (gamma * D_t[0]) + (g->M[0][acts[0]][acts[1]] - currentValor[0]);
        D_t[1] = (gamma * D_t[1]) + (g->M[1][acts[0]][acts[1]] - currentValor[1]);
    }

    printf("Debt: %lf, %lf\n", D_t[0], D_t[1]); fflush(stdout);

    // update the potential
    if (t != 0) // in the first round, the potential is initialized based on the first game -- no need to add it now
        potential += currentValor[me];
    t++;
    
    printf("Potential: %lf\n", getPotential());
}

void TriggerStrat::determineValors(Game *_g) {
    double alto;
    int index;
    
    switch (targetType) {
        case NBS_V:
            currentValor[0] = _g->nbs->sol->R[0];
            currentValor[1] = _g->nbs->sol->R[1];
            break;
        case BULLY1_V:
            alto = -99999;
            for (int i = 0; i < _g->numrNEoffers; i++) {
                if (_g->rNEoffers[i]->R[me] > alto) {
                    alto = _g->rNEoffers[i]->R[me];
                    index = i;
                }
                else if ((_g->rNEoffers[i]->R[me] == alto) && (_g->rNEoffers[i]->R[1-me] > _g->rNEoffers[index]->R[1-me])) {
                    index = i;
                }
            }
            currentValor[0] = _g->rNEoffers[index]->R[0];
            currentValor[1] = _g->rNEoffers[index]->R[1];
            break;
        case BULLIED1_V:
            alto = -99999;
            for (int i = 0; i < _g->numrNEoffers; i++) {
                if (_g->rNEoffers[i]->R[1-me] > alto) {
                    alto = _g->rNEoffers[i]->R[1-me];
                    index = i;
                }
                else if ((_g->rNEoffers[i]->R[1-me] == alto) && (_g->rNEoffers[i]->R[me] > _g->rNEoffers[index]->R[me])) {
                    index = i;
                }
            }
            currentValor[0] = _g->rNEoffers[index]->R[0];
            currentValor[1] = _g->rNEoffers[index]->R[1];
            break;
        default:
            printf("unknown targetType\n");
            exit(1);
            break;
    }
}

void TriggerStrat::setWelfareV() {
    int i;
    double most = -99999, swelf;
    int *best = new int[g->numSolutions];
    int numBest = 0;
    for (i = 0; i < g->numSolutions; i++) {
        if (!(g->gameSolutions[i]->puro))
            continue;
            
        swelf = g->gameSolutions[i]->R[0] + g->gameSolutions[i]->R[1];
        if (swelf > most) {
            most = swelf;
            numBest = 1;
            best[0] = i;
        }
        else if (swelf == most) {
            best[numBest] = i;
            numBest ++;
        }
    }
    printf("numBest = %i (%.2lf)\n", numBest, most);
    currentValor[0] = currentValor[1] = 0.0;
    for (i = 0; i < numBest; i++) {
        currentValor[0] += g->gameSolutions[best[i]]->R[0];
        currentValor[1] += g->gameSolutions[best[i]]->R[1];
    }
    currentValor[0] /= numBest;
    currentValor[1] /= numBest;
    
    delete[] best;
}
