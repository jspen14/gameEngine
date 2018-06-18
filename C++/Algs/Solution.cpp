#include "Solution.h"

Solution::Solution() {}

Solution::Solution(int a1, int a2, double p1, double p2) {
    puro = true;
    
    actions[0][0] = actions[1][0] = a1;
    actions[0][1] = actions[1][1] = a2;
    Valores[0][0] = Valores[1][0] = p1;
    Valores[0][1] = Valores[1][1] = p2;
    
    weights[0] = 1.0;
    weights[1] = 0.0;

    R[0] = p1;
    R[1] = p2;
}

Solution::Solution(int a1[2], int a2[2], double p1[2], double p2[2], double w[2]) {
    puro = false;
    
    actions[0][0] = a1[0];
    actions[1][0] = a1[1];
    actions[0][1] = a2[0];
    actions[1][1] = a2[1];
    
    Valores[0][0] = p1[0];
    Valores[1][0] = p1[1];
    Valores[0][1] = p2[0];
    Valores[1][1] = p2[1];
    
    weights[0] = w[0];
    weights[1] = w[1];
    
	R[0] = (p1[0] * w[0]) + (p1[1] * w[1]);
	R[1] = (p2[0] * w[0]) + (p2[1] * w[1]);
}

Solution::~Solution() {}

void Solution::print() {
    if (puro)
        printf("(%i, %i) : %.2lf, %.2lf\n", actions[0][0], actions[0][1], R[0], R[1]);
    else
        printf("(%i, %i) - (%i, %i) : %.2lf, %.2lf\n", actions[0][0], actions[0][1], actions[1][0], actions[1][1], R[0], R[1]);
}