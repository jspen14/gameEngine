/*
Function Call Parameters
argv[0] filename
argv[1] port number

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
  perror(msg);
  exit(1);

}

int main(int argc, char *argv[]){
  if(argc < 2){
    fprintf(stderr, "ERROR: One or More Arguments is Missing");
    // Is there going to be an error because the editor doesnt recognize stderr?
    exit(1);
  }

  int sockfd, newsockfd, portnum, n;
  char buffer[255];

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t cli_len;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    error("ERROR: Initializing Socket Failed");
  }
  else{
    printf("Waiting For Clients to Connnect\n");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));

  portnum = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portnum);

  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
    error("Binding Failed");
  }

  listen(sockfd, 5); // This is the number of active clients that be connected at a time
  cli_len = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);

  if(newsockfd < 0){
    error("ERROR: Failure to Accept Client");
  }

  while(1){
      bzero(buffer, 256); //bzero zeroes out the buffer
      n = read(newsockfd, buffer, 255); // Must be a corresponding write function in the client
      if(n < 0){
        error("ERROR: Failure to Read Correctly");
      }
      printf("Client: %s", buffer);
      bzero(buffer,255);
      fgets(buffer, 255, stdin); // Check this out


      n = write(newsockfd, buffer, strlen(buffer));
      if(n < 0){
        error("ERROR: Failure to Write Properly");
      }

      int i = strncmp("Bye", buffer, 3);
      if (i == 0){
        break;
      }
  }

  close(newsockfd);
  close(sockfd);

  return 0;
}
