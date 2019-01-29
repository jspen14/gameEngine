#include "defs.h"
#include "client_socket.h"
#include "Player.h"
#include "RandomPlayer.h"
#include "ExpertAlg.h"
#include "GeneralizedFP.h"
#include "Human.h"

void playInteractionGame(char *playerString, int me, char *host);
Player *createPlayer(char *playerString, int me);
bool isBestResponse(int index, Game *g, int actuar[2]);

/* ***********************************
*
*   ./igPlayer [playertype] [playerNumber] [host]
*
************************************** */
int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc < 4) {
        printf("Not enough parameters.  Exiting\n");
        exit(1);
    }

    playInteractionGame(argv[1], atoi(argv[2]), argv[3]);

    return 0;
}

void playInteractionGame(char *playerString, int me, char *host) {
    char buf[10000];
    char partner[1024];
    double tact[2], dineros[2];
    int actuar[2];

    // create the player
    Player *player = createPlayer(playerString, me);

    // establish a connection to the server
    ClientSocket *cs = new ClientSocket(host, 3000+me);
    cs->SendMessage(playerString, strlen(playerString));
    bool cheapTalk = false;
    cs->ReadMessage(buf); // this message contains (1) cheap talk or not indicate and (2) the name of the associate (for logging)
    if (buf[0] == 'Y') {
        cheapTalk = true;
        strcpy(partner, buf+4);
    }
    else {
        strcpy(partner, buf+3);
    }

    strcpy(buf, "ready\n");
    cs->SendMessage(buf, strlen(buf));

    // play the game
    int g = 0;
    double score[2] = {0.0, 0.0};

    double mm = 0.0, lowne = 0.0, highne = 0.0, nbser = 0.0;//, seer = 0.0;
    double mx;
    int bestcount = 0, bestLog[1000];
    int hadPureNE = 0;

    while (true) {
        // read and process the next game
        cs->ReadMessage(buf);

        printf("\n\nGame %i\n", g);

        // agent reasons about the game
        if (!(player->orient2Game(buf)))
            break;

        //printf("Oriented\n"); fflush(stdout);

        if (cheapTalk) {
            //printf("before-game cheap talks\n");
            //printf("que pasa?\n"); fflush(stdout);
            // send a message
            player->produceStartCheapTalk(buf);
            //printf("produced\n"); fflush(stdout);
            cs->SendMessage(buf, strlen(buf));

            //printf("sent message: %s\n", buf); fflush(stdout);

            // receive a message
            cs->ReadMessage(buf);
            player->processStartCheapTalk(buf);

            //printf("received cheap talk: %s\n", buf);
        }

        // select and send an action
        int act = player->Move();
        sprintf(buf, "%i$ 0.000000", act);
        cs->SendMessage(buf, strlen(buf));

        printf("action sent: %s\n", buf); fflush(stdout);

        // receive the result
        cs->ReadMessage(buf);
        printf("Results: %s\n", buf); fflush(stdout);

        sscanf(buf, "%i %i %lf %lf %lf %lf", &(actuar[0]), &(actuar[1]), &(dineros[0]), &(dineros[1]), &(tact[0]), &(tact[1]));
        score[0] += dineros[0];
        score[1] += dineros[1];

            mm += player->games[g]->minmax[me]->mv;
            lowne += player->games[g]->lowne[me];
            highne += player->games[g]->highne[me];
            nbser += player->games[g]->nbs->sol->R[me];

            if (isBestResponse(me, player->games[g], actuar)) {
                bestcount ++;
                bestLog[g] = 1;
                printf("BEST RESPONSE\n");
            }
            else {
                bestLog[g] = 0;
                printf("NOT best response\n");
            }

        if (player->games[g]->numPureNEs > 0)
            hadPureNE ++;
/*
        if (me == 0) {
            if (player->games[g]->M[0][1-actuar[0]][actuar[1]] <= player->games[g]->M[0][actuar[0]][actuar[1]]) {
                bestcount ++;
                bestLog[g] = 1;
            }
            else
                bestLog[g] = 0;
        }
        else {
            if (player->games[g]->M[1][actuar[0]][1-actuar[1]] <= player->games[g]->M[1][actuar[0]][actuar[1]]) {
                bestcount ++;
                bestLog[g] = 1;
            }
            else
                bestLog[g] = 0;
        }
*/

/*
        double mx = dineros[me];
        if (me == 0) {
            if (player->games[g]->M[0][1-actuar[0]][actuar[1]] > mx)
                mx = player->games[g]->M[0][1-actuar[0]][actuar[1]];
        }
        else {
            if (player->games[g]->M[1][actuar[0]][1-actuar[1]] > mx)
                mx = player->games[g]->M[1][actuar[0]][1-actuar[1]];
        }
        seer += mx;
*/
        player->moveUpdate(actuar, dineros);

        if (cheapTalk) {
            //printf("after-game cheap talk\n");

            // send a message
            player->produceEndCheapTalk(buf);
            cs->SendMessage(buf, strlen(buf));

            //printf("sent after-game cheap talk: %s\n", buf); fflush(stdout);

            // receive a message
            cs->ReadMessage(buf);
            player->processEndCheapTalk(buf);
        }

        strcpy(buf, "ready\n");
        cs->SendMessage(buf, strlen(buf));

        g++;

        printf("Scores: (%lf, %lf)\n", score[0], score[1]);

        printf("\nMy Game Stats:\n");
        printf("\tMaxmin: %lf\n", mm);
        printf("\tLowNE: %lf\n", lowne);
        printf("\tHighNE: %lf\n", highne);
        printf("\tNBS: %lf\n", nbser);
        printf("\tPercent Had PureNE: %lf\n", hadPureNE / (double)g);
        printf("\tPercent BR = %lf\n", bestcount / (double)g);
    }

    delete player;

    sprintf(buf, "%lf %lf %lf %lf %lf %lf %lf\n", score[me], mm, lowne, highne, nbser, hadPureNE / (double)g, player->ignbs.nbs[me]);
    cs->SendMessage(buf, strlen(buf));

    delete cs;

    FILE *fpBR;
    if (me == 0)
        fpBR = fopen("BR_0.txt", "w");
    else
        fpBR = fopen("BR_1.txt", "w");

    for (int i = 0; i < g; i++) {
        fprintf(fpBR, "%i\n", bestLog[i]);
    }

    fclose(fpBR);
}

Player *createPlayer(char *playerString, int me) {
    if (!strcmp("random", playerString)) {
        return new RandomPlayer(me);
    }
    else if (!strcmp("bully", playerString)) {
        return new ExpertAlg(me, playerString);
    }
    else if (!strcmp("bullied", playerString)) {
        return new ExpertAlg(me, playerString);
    }
    else if (!strcmp("fair", playerString)) {
        return new ExpertAlg(me, playerString);
    }
    else if (!strcmp("maxmin", playerString)) {
        return new ExpertAlg(me, playerString);
    }
    else if (!strcmp("listener", playerString)) {
        return new ExpertAlg(me, playerString);
    }
    else if (!strcmp("exploiter", playerString)) {
        return new ExpertAlg(me, playerString);
    }
    else if (!strcmp("spp", playerString)) {
        return new ExpertAlg(me, playerString);
    }
    else if (!strcmp("GeF", playerString) || !strncmp("maximizer", playerString, 8)) {
        return new GeneralizedFP(playerString, me, false);
    }
    else if (!strcmp("human", playerString)) {
        return new Human(me);
    }
    else {
        printf("player type %s not found. Exiting\n", playerString);
        return NULL;
    }
}

bool isBestResponse(int index, Game *g, int actuar[2]) {
    double mejor;
    int i;

    if (index == 0) {
        mejor = g->M[0][actuar[0]][actuar[1]];

        for (i = 0; i < g->A[0]; i++) {
            if (i == actuar[0])
                continue;

            if (g->M[0][i][actuar[1]] > mejor)
                return false;
        }
    }
    else {
        mejor = g->M[1][actuar[0]][actuar[1]];

        for (i = 0; i < g->A[0]; i++) {
            if (i == actuar[1])
                continue;

            if (g->M[1][actuar[0]][i] > mejor)
                return false;
        }
    }

    return true;
}
