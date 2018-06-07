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


using namespace std;

void error(const char *msg) { perror(msg); exit(0); }

char * httpCall(char message[1024]){
  int portno = 3000;
  char *host = (char *) "127.0.0.1"; // This will eventually need to change

  struct hostent *server;
  struct sockaddr_in serv_addr;
  int sockfd, bytes, sent, received, total;
  char response[4096];


  /* fill in the parameters */
  // sprintf(message,message_fmt,argv[1]);
  //printf("Request:\n%s\n",message);

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

  //printf("Response:\n%s\n\n",response);

  return response;

}

char * stripHeader(char * withHeader){
  char * withoutHeader;
  int numChars = 0;
  int len = strlen(withHeader);


  for (int i = 0; i < strlen(withHeader); i++){
    if (withHeader[i] == '^' && withHeader[i+1] == '^' && withHeader[i+2] == '^'){
      numChars = len - (i+3);
      withoutHeader = &withHeader[len - numChars];
    }
  }

  return withoutHeader;
}

char * getUserID(char *argv[]){
  char *message_fmt = (char *) "POST /api/AIlogin/%s HTTP/1.0\r\n\r\n";
  char * response;
  char message[1024];

  sprintf(message,message_fmt,argv[1]);

  response = httpCall(message);

  return response;
}

char * getInGameStatus(char *userID){
  char *message_fmt = (char *) "GET /api/AIinGameStatus/%s HTTP/1.0\r\n\r\n";
  char *response;
  char message[1024];

  sprintf(message,message_fmt,userID);

  response = httpCall(message);

  return response;
}

char * getWhich(char *userID){
    char *message_fmt = (char *) "GET /api/AIwhich/%s HTTP/1.0\r\n\r\n";
    char *response;
    char message[1024];

    sprintf(message,message_fmt,userID); //argv[1]

    response = httpCall(message);

    return response;
}

char * getRound(char *gameID){
    char *message_fmt = (char *) "GET /api/AIround/%s HTTP/1.0\r\n\r\n";
    char *response;
    char message[1024];

    sprintf(message,message_fmt,gameID);

    response = httpCall(message);

    return response;
}

char * getMatrix(char *gameID){
    char *message_fmt = (char *) "GET /api/AImatrix/%s HTTP/1.0\r\n\r\n";
    char *response;
    char message[1024];

    sprintf(message,message_fmt,gameID); //argv[1]

    response = httpCall(message);

    return response;
}

char * submitRoundOption(char *gameID, char *playerNum, char *option){
    char *message_fmt = (char *) "POST /api/AIsetRoundOption/%s/%s/%s HTTP/1.0\r\n\r\n";
    char *response;
    char message[1024];

    sprintf(message,message_fmt,gameID,playerNum,option); //argv[1]

    response = httpCall(message);

    return response;
}

char * getSubmittedStatus(char *gameID, char *playerNum){
    char *message_fmt = (char *) "GET /api/AIsubmittedStatus/%s/%s HTTP/1.0\r\n\r\n";
    char *response;
    char message[1024];

    sprintf(message,message_fmt,gameID,playerNum); //argv[1]

    response = httpCall(message);

    return response;
}

char * getRoundEarnings(char *gameID, char *playerNum){
  char *message_fmt = (char *) "GET /api/AIroundEarnings/%s/%s HTTP/1.0\r\n\r\n";
  char *response;
  char message[1024];

  sprintf(message,message_fmt,gameID,playerNum); //argv[1]

  response = httpCall(message);

  return response;

}

char * getOtherPlayersOption(char *gameID, char *otherPlayerNum){
  char *message_fmt = (char *) "GET /api/AIotherPlayersOption/%s/%s HTTP/1.0\r\n\r\n";
  char *response;
  char message[1024];

  sprintf(message,message_fmt,gameID,otherPlayerNum); //argv[1]

  response = httpCall(message);

  return response;
}

char * getReadyStatus(char *gameID, char *playerNum){
    char *message_fmt = (char *) "GET /api/AIreadyStatus/%s/%s HTTP/1.0\r\n\r\n";
    char *response;
    char message[1024];

    sprintf(message,message_fmt,gameID,playerNum); //argv[1]

    response = httpCall(message);

    return response;
}

char * getGameStatus(char *gameID){
    char *message_fmt = (char *) "GET /api/AIgameIsDone/%s HTTP/1.0\r\n\r\n";
    char *response;
    char message[1024];

    sprintf(message,message_fmt,gameID);

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

vector<int> getChoices(vector< vector<int> > rows, int whichPlayer){
    vector<int> choices;
    int option1 = 0;
    int option2 = 0;

    if(whichPlayer == 1){
        whichPlayer = 0;
        option1 = rows[0][whichPlayer]+rows[1][whichPlayer];
        option2 = rows[2][whichPlayer] + rows[3][whichPlayer];
    }
    else{ // This function is working
        whichPlayer = 1;
        option1 = rows[0][whichPlayer]+rows[2][whichPlayer];
        option2 = rows[1][whichPlayer] + rows[3][whichPlayer];
    }

    choices.push_back(option1);
    choices.push_back(option2);
    return choices;
}

int computeBestOption(string toParse, int whichPlayer) {
    vector< vector<int> > rowsOfMatrices;
    vector<int> choices;
    int myChoice = 0;

    rowsOfMatrices = parseString(toParse);
    choices = getChoices(rowsOfMatrices, whichPlayer);

    // This is the max possible payoff strategy
    if(choices[0]>choices[1]){
        myChoice = 0;
    }
    else {
        myChoice = 1;
    }
    // Make an algorithm that analyzes what the other player is likely to do and then determines your possible outcome based upon that

        cout << myChoice << " @ $" << choices[myChoice] << endl;

    return myChoice;
}


int main(int argc,char *argv[])
{
    // Error check
    if (argc < 2) { puts("Parameters: <message>"); exit(0); }

    // Seed rand
    srand(time(NULL));

    // Significant Data for GamePlay
    char * userIDarg = stripHeader(getUserID(argv));
    char * userIDCheck = (char *) "Name already in use!";
    char * inGameStatus = (char *) "false";
    char * inGameStatusCheck = (char *) "false";
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
    vector<string> myEarnings;
    vector<string> myChoices;
    vector<string> theirEarnings;
    vector<string> theirChoices;

    // Get userID
    if (strcmp(userIDarg, userIDCheck) == 0){
      cout << "User is already active in system." << endl; // This isn't working (most likely because server code is commented out)
      return 0;
    }

    cout << "Waiting to be added to game ..." << endl;
    // Set userID
    userIDStr = userIDarg;    // Conversion from char* to string

    // Check inGameStatus
    while(strcmp(inGameStatus,inGameStatusCheck) == 0){
      inGameStatus = stripHeader(getInGameStatus((char *) userIDStr.c_str()));
      sleep(3);
    }

    // Set gameID
    gameIDStr = inGameStatus;   // Conversion from char* to string

    // Set Which Player (1 || 2)
    whichStr = stripHeader(getWhich((char *) userIDStr.c_str()));

    cout << "Successfully added to game " << gameIDStr << " as player " << whichStr << "." << endl << endl;

    //Round Play
    while(!done){
      // getRound
      roundNumStr = stripHeader(getRound((char *) gameIDStr.c_str()));
      cout << "Round: " << roundNumStr << endl;
      cout << "------------------------------------------------" << endl;

      // getMatrix
      roundMatrixStr = stripHeader(getMatrix((char *) gameIDStr.c_str()));

      // See how this works

      cout << "Matrix: " << roundMatrixStr << endl;

      // Make Decision
      roundOptionInt = computeBestOption(roundMatrixStr, stoi(whichStr));
      roundOptionStr = to_string(roundOptionInt);
      myChoices.push_back(roundOptionStr);
      cout << "Option: " << roundOptionStr << endl;

      // Send Decision
      submitRoundOption((char *) gameIDStr.c_str(), (char *) whichStr.c_str(), (char *) roundOptionStr.c_str());

      // Reset Waiting Bool
      waiting = true;

      // Wait for other player to play
      while (waiting){
        submittedStatusStr = stripHeader(getSubmittedStatus((char *) gameIDStr.c_str(), (char *) whichStr.c_str()));

        if (submittedStatusStr == "true"){
          //cout << "Ready to continue. " << endl;
          waiting = false;
        }

        sleep (3);
      }

      // Get earnings here
      if (whichStr == "1"){
        // Get earnings
        myEarnings.push_back(stripHeader(getRoundEarnings((char *) gameIDStr.c_str(), (char *) "1")));
        theirEarnings.push_back(stripHeader(getRoundEarnings((char *) gameIDStr.c_str(), (char *) "2")));

        // Get other player's choice
        theirChoices.push_back(stripHeader(getOtherPlayersOption((char *) gameIDStr.c_str(), (char *) "2")));
      }
      else if (whichStr == "2"){
        // Get earnings
        myEarnings.push_back(stripHeader(getRoundEarnings((char *) gameIDStr.c_str(), (char *) "2")));
        theirEarnings.push_back(stripHeader(getRoundEarnings((char *) gameIDStr.c_str(), (char *) "1")));

        //Get other player's choice
        theirChoices.push_back(stripHeader(getOtherPlayersOption((char *) gameIDStr.c_str(), (char *) "1")));
      }

      cout << "My Payoff: " << myEarnings[myEarnings.size()-1] << endl;
      cout << "Their Payoff: " << theirEarnings[theirEarnings.size()-1] << endl;
      cout <<  "------------------------------------------------" << endl << endl;
      // Update readyStatus
      waiting = true;

      while(waiting){
        readyStatusStr = stripHeader(getReadyStatus((char *) gameIDStr.c_str(), (char *) whichStr.c_str()));

        if (readyStatusStr == "true"){
          waiting = false;
        }

        sleep(3);
      }

      gameIsDoneStr = stripHeader(getGameStatus((char *) gameIDStr.c_str()));

      if (gameIsDoneStr == "true"){
        done = true;
      }

      sleep(3);

    }

    // Print information here if that's what we decide to do.
    cout << "Thanks for playing! " << endl;

    return 0;
}
