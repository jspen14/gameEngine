#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <time.h> /* time */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <iostream>
#include <string>
#include <vector>
#include "defs.h"
#include "client_socket.h"
#include "Player.h"
#include "RandomPlayer.h"
#include "ExpertAlg.h"


using namespace std;

void error(const char *msg) { perror(msg); exit(0); }

string httpCall(char message[1024]){
  int portno = 3000;
  char *host = (char *) "127.0.0.1"; // This will eventually need to change

  struct hostent *server;
  struct sockaddr_in serv_addr;
  int sockfd, bytes, sent, received, total;
  char response[4096];
  string responseStr;
  /* create the socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");

  /* lookup the ip address */
  server = gethostbyname(host);
  if (server == NULL) error("ERROR, no such host");

  /* fill in the structure */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

  /* connect the socket */
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
      error("ERROR connecting");

  /* send the request */
  total = strlen(message);
  sent = 0;
  do {
      bytes = write(sockfd,message+sent,total-sent);
      if (bytes < 0)
          error("ERROR writing message to socket");
      if (bytes == 0)
          break;
      sent+=bytes;
  } while (sent < total);

  /* receive the response */
  memset(response,0,sizeof(response));
  total = sizeof(response)-1;
  received = 0;
  do {
      bytes = read(sockfd,response+received,total-received);
      if (bytes < 0)
          error("ERROR reading response from socket");
      if (bytes == 0)
          break;
      received+=bytes;
  } while (received < total);

  if (received == total)
      error("ERROR storing complete response from socket");

  /* close the socket */

  close(sockfd);

  /* process response */

  responseStr = response;

  return responseStr;
}

string stripHeader(string withHeader){
  string withoutHeader;
  int numChars = 0;
  int len = withHeader.length();


  for (int i = 0; i < len; i++){
    if (withHeader[i] == '^' && withHeader[i+1] == '^' && withHeader[i+2] == '^'){
      numChars = len - (i+3);
      withoutHeader = &withHeader[len - numChars];
    }
  }

  return withoutHeader;
}

string getUserID(char *argv[]){
  char *message_fmt = (char *) "POST /api/AIlogin/%s HTTP/1.0\r\n\r\n";
  string response;
  char message[1024];

  sprintf(message,message_fmt,argv[1]);
  response = httpCall(message);

  return response;
}

string getInGameStatus(string userIDStr){
  char *message_fmt = (char *) "GET /api/AIinGameStatus/%s HTTP/1.0\r\n\r\n";
  string response;
  char *userIDArg = (char *) userIDStr.c_str();
  char message[1024];

  sprintf(message,message_fmt,userIDArg);

  response = httpCall(message);

  return response;
}

string getWhich(string userIDStr){
    char *message_fmt = (char *) "GET /api/AIwhich/%s HTTP/1.0\r\n\r\n";
    string response;
    char *userIDArg = (char *) userIDStr.c_str();
    char message[1024];


    sprintf(message,message_fmt,userIDArg);

    response = httpCall(message);

    return response;
}

string getRound(string gameIDStr){
    char *message_fmt = (char *) "GET /api/AIround/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg);

    response = httpCall(message);

    return response;
}

string getMatrix(string gameIDStr){
    char *message_fmt = (char *) "GET /api/AImatrix/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg);

    response = httpCall(message);

    return response;
}

string sendCheapTalk(string gameIDStr, string userIDStr, string cheapTalkMessageStr){
  char *message_fmt = (char *) "POST /api/AIcheapTalk/%s/%s/%s HTTP/1.0\r\n\r\n";
  string response;
  // This is version specific
    // The intent behind these next couple lines of code is to take the newLine characters off the string

  string revisedStr = "";
  revisedStr.push_back(cheapTalkMessageStr[0]);
  revisedStr.push_back(cheapTalkMessageStr[1]);

  char *cheapTalkMessageArg = (char *) revisedStr.c_str();
  char *gameIDArg = (char *) gameIDStr.c_str();
  char *userIDArg = (char *) userIDStr.c_str();

  char message[1024];

  sprintf(message,message_fmt,gameIDArg, userIDArg, cheapTalkMessageArg);

  response = httpCall(message);

  return response;
}

string submitRoundOption(string gameIDStr, string playerNumStr, string optionStr){
    char *message_fmt = (char *) "POST /api/AIsetRoundOption/%s/%s/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char *playerNumArg = (char *) playerNumStr.c_str();
    char *optionArg = (char *) optionStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg,playerNumArg,optionArg);

    response = httpCall(message);

    return response;
}

string getSubmittedStatus(string gameIDStr, string playerNumStr){
    char *message_fmt = (char *) "GET /api/AIsubmittedStatus/%s/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char *playerNumArg = (char *) playerNumStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg,playerNumArg);

    response = httpCall(message);

    return response;
}

string getRoundEarnings(string gameIDStr, string playerNumStr){
  char *message_fmt = (char *) "GET /api/AIroundEarnings/%s/%s HTTP/1.0\r\n\r\n";
  string response;
  char *gameIDArg = (char *) gameIDStr.c_str();
  char *playerNumArg = (char *) playerNumStr.c_str();
  char message[1024];

  sprintf(message,message_fmt,gameIDArg,playerNumArg);

  response = httpCall(message);

  return response;
}

string getOtherPlayersOption(string gameIDStr, string otherPlayerNumStr){
  char *message_fmt = (char *) "GET /api/AIotherPlayersOption/%s/%s HTTP/1.0\r\n\r\n";
  string response;
  char *gameIDArg = (char *) gameIDStr.c_str();
  char *otherPlayerNumArg = (char *) otherPlayerNumStr.c_str();
  char message[1024];

  sprintf(message,message_fmt,gameIDArg,otherPlayerNumArg);

  response = httpCall(message);

  return response;
}

string getReadyStatus(string gameIDStr, string playerNumStr){
    char *message_fmt = (char *) "GET /api/AIreadyStatus/%s/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char *playerNumArg = (char *) playerNumStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg,playerNumArg);

    response = httpCall(message);

    return response;
}

string getGameStatus(string gameIDStr){
    char *message_fmt = (char *) "GET /api/AIgameIsDone/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg);

    response = httpCall(message);

    return response;
}

vector< vector<int> > parseString(string toParse){
    vector< vector<int> > rows;
    vector<int> row;

    for (int i = 0; i < toParse.size(); i++){

        if (toParse[i] == ']'){
            if (row.size() != 0){
                rows.push_back(row);
            }
            row.clear();
        }
        else if (isdigit(toParse[i])){
            if (isdigit(toParse[i+1])){
              cout << "NUM: " << i << " " << (toParse[i] - '0')*10 + (toParse[i+1]-'0') << endl;

              row.push_back((toParse[i] - '0')*10 + (toParse[i+1]-'0'));
              i++;
            }
            else{
                row.push_back(toParse[i]-'0');
            }
        }

    }
    return rows;
}

Player *createPlayer(char * playerString, int me) {
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
    else {
        printf("player type %s not found. Exiting\n", playerString);
        return NULL;
    }
}


int main(int argc,char *argv[])
{
    // Error check
    if (argc < 3) { puts("Parameters: <message>"); exit(0); }

    // Seed rand
    srand(time(NULL));

    // Significant Data for GamePlay
    string userIDCheck = "Name already in use!";
    string inGameStatus = "false";
    string inGameStatusCheck = "false";
    string userIDStr = "";
    string gameIDStr = "";
    string whichStr = "";
    string roundNumStr = "";
    string roundMatrixStr = "";
    string roundOptionStr = "";
    string submittedStatusStr = "";
    string readyStatusStr = "";
    string gameIsDoneStr = "";
    int roundOptionInt = 0;
    bool done = false;
    bool waiting = true;
    bool cheapTalk = true;
    vector<string> myEarnings;
    vector<string> myChoices;
    vector<string> theirEarnings;
    vector<string> theirChoices;

    // Get userID
    userIDStr = stripHeader(getUserID(argv));

    // system("open http://localhost:3000"); // figure out how to change default browser on this
    // system("open http://localhost:3000/#/admin");
    // Set userID

    if (userIDStr == "undefined"){
      cout << "This user is not currently enrolled in the system." << endl;
      return 0;
    }
    else if (userIDStr == userIDCheck){
      cout << "User is already active in system." << endl;
      return 0;
    }

    cout << "Waiting to be added to game ..." << endl;

    // Check inGameStatus
    while(inGameStatus == inGameStatusCheck){

      inGameStatus = stripHeader(getInGameStatus(userIDStr));
      sleep(3);
    }

    // Set gameID
    gameIDStr = inGameStatus;   // Conversion from char* to string

    // Set Which Player (1 || 2)
    whichStr = stripHeader(getWhich(userIDStr));

    cout << "Successfully added to game " << gameIDStr << " as player " << whichStr << "." << endl << endl;

/********************************************************************/
/***Run algorithm***/
    char buf[10000];
    char partner[1024];
    double tact[2], dineros[2];
    int actuar[2];
    int act;

    // create the player
    string playerString = argv[2];
    int me = 0;
    if (whichStr == "2")
      me = 1;
    Player *player = createPlayer((char *)playerString.c_str(), me);

    // play the game
    int g = 0;
    double score[2] = {0.0, 0.0};

    //Round Play
    while(!done){
        // getRound
        roundNumStr = stripHeader(getRound(gameIDStr));
        cout << "Round: " << g << endl;
        cout << "------------------------------------------------" << endl;

        // getMatrix
        roundMatrixStr = stripHeader(getMatrix(gameIDStr));
        // See how this works

        cout << "Matrix: " << roundMatrixStr << endl;



        printf("\n\nGame %i\n", g);


        //agent reasons about the game
        if (!(player->orient2Game((char *)roundMatrixStr.c_str())))
            break;

        //printf("Oriented\n"); fflush(stdout);

        if (cheapTalk) {
          printf("before-game cheap talks\n");
          player->produceStartCheapTalk(buf);

          string cheapTalkMessage(buf);

          cout << "CTM: " << cheapTalkMessage << endl;

          sendCheapTalk(gameIDStr, userIDStr, cheapTalkMessage);

          // receive a message

          //player->processStartCheapTalk(buf);

          //printf("received cheap talk: %s\n", buf);
        }


        // select and send an action
        int act = player->Move();
        roundOptionStr = to_string(act);
        myChoices.push_back(roundOptionStr);

        cout << "Round Option: " << roundOptionStr << endl;
        /*
        sprintf(buf, "%i$ 0.000000", act);
        cs->SendMessage(buf, strlen(buf));

        printf("action sent: %s\n", buf); fflush(stdout);

        // receive the result
        cs->ReadMessage(buf);
        printf("Results: %s\n", buf); fflush(stdout);

        sscanf(buf, "%i %i %lf %lf %lf %lf", &(actuar[0]), &(actuar[1]), &(dineros[0]), &(dineros[1]), &(tact[0]), &(tact[1]));
        */

        /*
        if (cheapTalk) {
          printf("after-game cheap talk\n");

          // send a message
          player->produceEndCheapTalk(buf);
          cs->SendMessage(buf, strlen(buf));

          printf("sent after-game cheap talk: %s\n", buf); fflush(stdout);

          // receive a message
          cs->ReadMessage(buf);
          player->processEndCheapTalk(buf);
        }

        strcpy(buf, "ready\n");
        cs->SendMessage(buf, strlen(buf));
        */

        // Send Decision
        submitRoundOption(gameIDStr, whichStr, roundOptionStr);

        // Reset Waiting Bool
        waiting = true;

        // Wait for other player to play
        while (waiting){
            submittedStatusStr = stripHeader(getSubmittedStatus(gameIDStr, whichStr));

            if (submittedStatusStr == "true"){
                //cout << "Ready to continue. " << endl;
                waiting = false;
            }

            sleep (1);
        }

        /********************************************************************/

        /*
            - put this in a while loop
            - make a goodToContinue bool
            - reset bool true at the beginning of the if statement (at the beginning of each iteration of for loop)
            - if any of the values return as undefined, change the bool to false so that it'll query the server again
              - this should work because there are the correct amount of entries in these arrays on the server
            - if we have to repeat, sleep for 2 seconds or so.
        */

        do {
          waiting = false;
          string temp = "" ;

          if (whichStr == "1"){

            temp = stripHeader(getRoundEarnings(gameIDStr, "1"));
            if (temp != "undefined"){
              myEarnings.push_back(temp);
            }
            else {
              waiting = true;
            }

            temp = stripHeader(getRoundEarnings(gameIDStr, "2"));
            if(temp != "undefined"){
              theirEarnings.push_back(temp);
            }
            else{
              waiting = true;
            }

            temp = stripHeader(getOtherPlayersOption(gameIDStr, "2"));
            if(temp != "undefined"){
              theirChoices.push_back(temp);
            }
            else{
              waiting = true;
            }

          }
          else if (whichStr == "2"){
            temp = stripHeader(getRoundEarnings(gameIDStr, "2"));
            if (temp != "undefined"){
              myEarnings.push_back(temp);
            }
            else {
              waiting = true;
            }

            temp = stripHeader(getRoundEarnings(gameIDStr, "1"));
            if(temp != "undefined"){
              theirEarnings.push_back(temp);
            }
            else{
              waiting = true;
            }

            temp = stripHeader(getOtherPlayersOption(gameIDStr, "1"));
            if(temp != "undefined"){
              theirChoices.push_back(temp);
            }
            else{
              waiting = true;
            }

          }

          if(waiting){
            sleep(1);
          }
        } while(waiting);


        actuar[me] = act;
        actuar[1-me] = stoi(theirChoices[theirChoices.size()-1]);

        // Have an error check here that sees if there is a nulled value
        if( myEarnings[myEarnings.size()-1] == "undefined"){
          cout << "yes 2" << endl;
        }


        cout << "My: " << myEarnings[myEarnings.size()-1] << endl;
        cout << "Their: " << theirEarnings[theirEarnings.size()-1] << endl;

        dineros[me] = stod(myEarnings[myEarnings.size()-1]);
        dineros[1-me] = stod(theirEarnings[theirEarnings.size()-1]);

        score[0] += dineros[0];
        score[1] += dineros[1];

        player->moveUpdate(actuar, dineros);
        g++;

        printf("Scores: (%lf, %lf)\n", score[0], score[1]);


        cout << "My Payoff: " << myEarnings[myEarnings.size()-1] << endl;
        cout << "Their Payoff: " << theirEarnings[theirEarnings.size()-1] << endl;
        cout <<  "------------------------------------------------" << endl << endl;
        // Update readyStatus
        waiting = true;

        while(waiting){
          readyStatusStr = stripHeader(getReadyStatus(gameIDStr, whichStr));

          if (readyStatusStr == "true"){
            waiting = false;
          }

          sleep(1);
        }

        gameIsDoneStr = stripHeader(getGameStatus(gameIDStr));

        if (gameIsDoneStr == "true"){
          done = true;
        }

    }

    delete player;
    printf("se acabo\n");

    // Print information here if that's what we decide to do.
    cout << "Thanks for playing! " << endl;

    return 0;
}
