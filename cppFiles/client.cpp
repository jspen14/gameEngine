/*
Args
  filename server_ipaddress portnum

argv[1] filename
argv[2] server_ipaddress
argv[3] portnum

*/

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
  perror(msg);
  exit(0);
}


int main(int argc, const char * argv[]) {

    int sockfd, portnum, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3){
      fprintf(stderr, "usage %s hostname port\n",argv[0]);
      exit(1);
    }

  // Initialize Socket
    portnum = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
      error("ERROR: Initializing Socket Failed");
    }

  // Connect Host
    server = gethostbyname(argv[1]);
    if (server == NULL){
      fprintf(stderr, "ERROR: Failed to Find Matching Host");
    }

  // bzero Function
    bzero((char *) &serv_addr, sizeof(serv_addr)); // This sets all socket structures to null values
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); // copies n bytes from server to serv_addr
    serv_addr.sin_port = htons(portnum); //Converts portnum in BigEndian format aka "Network Byte Order"
    if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
      error("Connection Failed");
    }

    printf("Send a message to the server! \n");
    while(1){
      bzero(buffer,255);
      fgets(buffer, 255, stdin);
      n = write(sockfd, buffer, strlen(buffer));
      if(n < 0){
        error("ERROR: Failed to Write Correctly");
      }

      bzero(buffer, 255);
      n = read(sockfd, buffer, 255);
      if( n < 0){
        error("ERROR: Failed to Read Correctly");
      }
      printf("Server: %s", buffer);

      int i = strncmp("Bye", buffer , 3);

      if(i == 0){
        break;
      }

    }

    close(sockfd);


    return 0;
}
