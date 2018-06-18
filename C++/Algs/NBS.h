#ifndef NBS_H
#define NBS_H

#include "defs.h"
#include "Solution.h"

typedef struct Point {
	double x, y;
} Point;

class NBS {
public:
    
    NBS();
    NBS(double ***M, int A[2]);
    ~NBS();
    
    void GetDefend(double alpha[NumDecisionPointsMax], double payoff[NumDecisionPointsMax][NumDecisionPointsMax]);
    void GetAttack(double alpha[NumDecisionPointsMax], double payoff[NumDecisionPointsMax][NumDecisionPointsMax]);
    void GetGuarantee(double delta[NumDecisionPointsMax], double alpha[NumDecisionPointsMax],
                      double payoff[NumDecisionPointsMax][NumDecisionPointsMax], double *g);
    double ComputeBargain(Point tp1, Point tp2, int *one, int *two);
    void RecurseMinimax(double *weights, double *alpha, double *currentmin, double payoff[NumDecisionPointsMax][NumDecisionPointsMax], int index);
    void RecurseMaximin(double *weights, double *alpha, double *currentmax, double payoff[NumDecisionPointsMax][NumDecisionPointsMax], int index);
    int Normalize(double *m, double *n);
    
    double alpha1[NumDecisionPointsMax], alpha2[NumDecisionPointsMax];
    double delta1[NumDecisionPointsMax], delta2[NumDecisionPointsMax];
    double g1, g2;
    
    double payoff1[NumDecisionPointsMax][NumDecisionPointsMax];
    
    double payoff2[NumDecisionPointsMax][NumDecisionPointsMax];
    
    double A1[NumDecisionPointsMax][NumDecisionPointsMax], A2[NumDecisionPointsMax][NumDecisionPointsMax];
    
    int NumDecisionPointsActual;
    
    Solution *sol;
    //int theCounts[2];
    //int theAcciones1[2], theAcciones2[2];
};

#endif