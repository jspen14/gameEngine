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


    sprintf(message,message_fmt,userIDArg); //argv[1]

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

    sprintf(message,message_fmt,gameIDArg); //argv[1]

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

    sprintf(message,message_fmt,gameIDArg,playerNumArg,optionArg); //argv[1]

    response = httpCall(message);

    return response;
}

string getSubmittedStatus(string gameIDStr, string playerNumStr){
    char *message_fmt = (char *) "GET /api/AIsubmittedStatus/%s/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char *playerNumArg = (char *) playerNumStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg,playerNumArg); //argv[1]

    response = httpCall(message);

    return response;
}

 string getRoundEarnings(string gameIDStr, string playerNumStr){
  char *message_fmt = (char *) "GET /api/AIroundEarnings/%s/%s HTTP/1.0\r\n\r\n";
  string response;
  char *gameIDArg = (char *) gameIDStr.c_str();
  char *playerNumArg = (char *) playerNumStr.c_str();
  char message[1024];

  sprintf(message,message_fmt,gameIDArg,playerNumArg); //argv[1]

  response = httpCall(message);

  return response;

}

string getOtherPlayersOption(string gameIDStr, string otherPlayerNumStr){
  char *message_fmt = (char *) "GET /api/AIotherPlayersOption/%s/%s HTTP/1.0\r\n\r\n";
  string response;
  char *gameIDArg = (char *) gameIDStr.c_str();
  char *otherPlayerNumArg = (char *) otherPlayerNumStr.c_str();
  char message[1024];

  sprintf(message,message_fmt,gameIDArg,otherPlayerNumArg); //argv[1]

  response = httpCall(message);

  return response;
}

string getReadyStatus(string gameIDStr, string playerNumStr){
    char *message_fmt = (char *) "GET /api/AIreadyStatus/%s/%s HTTP/1.0\r\n\r\n";
    string response;
    char *gameIDArg = (char *) gameIDStr.c_str();
    char *playerNumArg = (char *) playerNumStr.c_str();
    char message[1024];

    sprintf(message,message_fmt,gameIDArg,playerNumArg); //argv[1]

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

int computeRiskyOption(string toParse, int whichPlayer) {
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
    string userIDCheck = (char *) "Name already in use!";
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
    vector<string> myEarnings;
    vector<string> myChoices;
    vector<string> theirEarnings;
    vector<string> theirChoices;

    // Get userID
    userIDStr = stripHeader(getUserID(argv));

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

    //Round Play
    while(!done){
      // getRound
      roundNumStr = stripHeader(getRound(gameIDStr));
      cout << "Round: " << roundNumStr << endl;
      cout << "------------------------------------------------" << endl;

      // getMatrix
      roundMatrixStr = stripHeader(getMatrix(gameIDStr));

      // See how this works
      cout << "Matrix: " << roundMatrixStr << endl;

      // Make Decision
      roundOptionInt = rand() % 2; 
      roundOptionStr = to_string(roundOptionInt);
      myChoices.push_back(roundOptionStr);
      cout << "Option A: " << roundOptionStr << endl;

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

        sleep (3);
      }

      // Get earnings here
      if (whichStr == "1"){
        // Get earnings
        myEarnings.push_back(stripHeader(getRoundEarnings(gameIDStr, "1")));
        theirEarnings.push_back(stripHeader(getRoundEarnings(gameIDStr, "2")));

        // Get other player's choice
        theirChoices.push_back(stripHeader(getOtherPlayersOption(gameIDStr, "2")));
      }
      else if (whichStr == "2"){
        // Get earnings
        myEarnings.push_back(stripHeader(getRoundEarnings(gameIDStr, "2")));
        theirEarnings.push_back(stripHeader(getRoundEarnings(gameIDStr, "1")));

        //Get other player's choice
        theirChoices.push_back(stripHeader(getOtherPlayersOption(gameIDStr, "1")));
      }

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

        sleep(3);
      }

      gameIsDoneStr = stripHeader(getGameStatus(gameIDStr));

      if (gameIsDoneStr == "true"){
        done = true;
      }

      sleep(3);

    }

    // Print information here if that's what we decide to do.
    cout << "Thanks for playing! " << endl;

    return 0;
}
