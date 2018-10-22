#ifndef SOLUTION_H
#define SOLUTION_H

#include "defs.h"

class Solution {
public:
    Solution();
    Solution(int a1, int a2, double p1, double p2);
    Solution(int a1[2], int a2[2], double p1[2], double p2[2], double w[2]);
    ~Solution();
    
    void print();

    bool puro;
    double R[2];
    double Valores[2][2];
    int actions[2][2];
    double weights[2];
};

#endif