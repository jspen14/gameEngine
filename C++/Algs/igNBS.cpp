#include "igNBS.h"

igNBS::igNBS() {
    numPoints = numOldPoints = 0;
    maxminSum[0] = maxminSum[1] = 0.0;
}

igNBS::~igNBS() {
    printf("igNBS destructor\n");
    
    FILE *fp = fopen("PointDump.csv", "w");
    fprintf(fp, "p1,p2\n");
    for (int i = 0; i < numOldPoints; i++) {
        fprintf(fp, "%lf,%lf\n", oldPoints[i][0], oldPoints[i][1]);
    }
    fclose(fp);
}
    
void igNBS::updateWithGame(Game *_g) {
    //printf("updateWithGame\n"); fflush(stdout);

    maxminSum[0] += _g->minmax[0]->mv;
    maxminSum[1] += _g->minmax[1]->mv;

    int i, j;
    
    numPoints = 0;

    // update points with possible new results
    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            if (isParetoOptimal(_g, i, j)) {
                addResult(_g->M[0][i][j], _g->M[1][i][j]);
            }
        }
    }
    
    cleanPoints();

    // copy points to old points
    numOldPoints = 0;
    for (i = 0; i < numPoints; i++) {
        if (points[i][0] > -999999) {
            oldPoints[numOldPoints][0] = points[i][0];
            oldPoints[numOldPoints][1] = points[i][1];
            numOldPoints++;
        }
    }

    printf("numPoints = %i\n", numOldPoints);
    //for (i = 0; i < numOldPoints; i++) {
    //    printf("%.1lf, %.1lf (%.1lf)\n", oldPoints[i][0], oldPoints[i][1], oldPoints[i][0] + oldPoints[i][1]);
    //}
    printf("minimax: %.2lf, %.2lf\n", maxminSum[0], maxminSum[1]);
    computeNBS();
    printf("nbs: %.2lf, %.2lf\n", nbs[0], nbs[1]);
}

bool igNBS::isParetoOptimal(Game *_g, int a1, int a2) {
    int i, j;
    
    for (i = 0; i < _g->A[0]; i++) {
        for (j = 0; j < _g->A[1]; j++) {
            if ((i != a1) || (j != a2)) {
                if ((_g->M[0][i][j] > _g->M[0][a1][a2]) && (_g->M[1][i][j] > _g->M[1][a1][a2]))
                    return false;
            }
        }
    }
    
    return true;
}

void igNBS::addResult(double p1, double p2) {
    int i;

    if (numOldPoints == 0) {
        points[numPoints][0] = p1;
        points[numPoints][1] = p2;
        
        numPoints++;
    }
    else {
        double v1, v2;
        for (i = 0; i < numOldPoints; i++) {
            points[numPoints][0] = oldPoints[i][0] + p1;
            points[numPoints][1] = oldPoints[i][1] + p2;
            
            numPoints++;
            
            if (numPoints == MAXPOINTS) {
                printf("hit the max number of points\n");
                exit(1);
            }
        }
    }
}

void igNBS::cleanPoints() {
    int i, j;
    
    for (i = 0; i < numPoints; i++) {
        for (j = 0; j < numPoints; j++) {
            if (i == j)
                continue;
            if ((points[i][0] <= points[j][0]) && (points[i][1] <= points[j][1])) {
                removePoint(i);
            }
/*            else if ((points[i][0] == points[j][0]) && (points[i][1] < points[j][1])) {
                removePoint(i);
            }
            else if ((points[i][0] < points[j][0]) && (points[i][1] == points[j][1])) {
                removePoint(i);
            }
            else if ((points[i][0] == points[j][0]) && (points[i][1] == points[j][1])) {
                removePoint(i);
            }*/
        }
    }
}

void igNBS::removePoint(int index) {
    int i;
    
    points[index][0] = points[index][1] = -999999;
/*
    for (i = index; i < numPoints-1; i++) {
        points[i][0] = points[i+1][0];
        points[i][1] = points[i+1][1];
    }
    
    numPoints --;
*/
}

void igNBS::computeNBS() {
    int i;
    double max = -9999, theProd;
    for (i = 0; i < numOldPoints; i++) {
        theProd = (oldPoints[i][0] - maxminSum[0]) * (oldPoints[i][1] - maxminSum[1]);
        if (theProd > max) {
            max = theProd;
            nbs[0] = oldPoints[i][0];
            nbs[1] = oldPoints[i][1];
        }
    }
}

double igNBS::getBullyValue(int bullyIndex, double targetValue) {
    int i;
    double alto = -99999;
    for (i = 0; i < numOldPoints; i++) {
        if (oldPoints[i][1-bullyIndex] >= targetValue) {
            if (oldPoints[i][bullyIndex] > alto)
                alto = oldPoints[i][bullyIndex];
        }
    }

    return alto;
}
