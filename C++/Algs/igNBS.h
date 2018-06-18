#ifndef IGNBS_H
#define IGNBS_H

#include "defs.h"
#include "Game.h"

#define MAXPOINTS   1000000

class igNBS {
public:
    igNBS();
    ~igNBS();
    
    void updateWithGame(Game *_g);
    bool isParetoOptimal(Game *_g, int a1, int a2);
    void addResult(double p1, double p2);
    void cleanPoints();
    void removePoint(int index);
    void computeNBS();
    
    double getBullyValue(int bullyIndex, double targetValue);
    
    double points[MAXPOINTS][2];
    double oldPoints[MAXPOINTS][2];
    int numPoints, numOldPoints;
    
    double maxminSum[2];
    double nbs[2];
};


#endif