#ifndef GAME_H
#define GAME_H

#include "defs.h"
#include "minimaxLog.h"
#include "Solution.h"
#include "NBS.h"

class Game {
public:
    Game();
    Game(char *entry);
    ~Game();
    
    void instantiateGame(char *tipoString, char *payoffString);
    void printGame();
    void createString(char str[1024]);
    void computeRNEoffers();
    
    int A[2];
    double ***M;
    
    char *tipoString;
    char *payoffString;
    
    minimaxLog *minmax[2];
    minimaxLog *attack[2];

    Solution **gameSolutions;
    int numSolutions;
    
    NBS *nbs;
    Solution **rNEoffers;
    int numrNEoffers;
    
    // other things maybe I consider later?
    //  - Max value
    //  - Min value
    //  - Risk adverse solutions
    //  - Dominant strategies
    //  - pure one-shot NE
};


#endif