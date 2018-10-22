#include "Human.h"

Human::Human() {
    printf("incomplete Human constructor\n");
    exit(1);
}

Human::Human(int _me) {
    me = _me;
}

Human::~Human() {
}

void Human::Reset() {
}

bool Human::orient2Game(char *gameString) {
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

void Human::produceStartCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void Human::processStartCheapTalk(char buf[10000]) {
}

void Human::produceEndCheapTalk(char buf[10000]) {
    strcpy(buf, "--\n");
}

void Human::processEndCheapTalk(char buf[10000]) {
}

int Human::Move() {
    //games[currentTime]->printGame();

    printf("\nEnter an action: ");
    
    int a = -1;
    std::string actc;
    while (a < 0) {
        //sscanf("%s", actc);
        std::cin >> actc;
        a = atoi(actc.c_str());
        if ((a < 0) || (a >= games[currentTime]->A[me]))
            a = -1;
    }
    printf("Chose a = %i\n", a);
    
    return a;
}

void Human::moveUpdate(int actions[2], double dineros[2]) {
    //printf("move updated\n"); fflush(stdout);
    
    currentTime++;
}
