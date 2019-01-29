#include "client_socket.h"
#include <iostream>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_BUFFER		1024

ClientSocket::ClientSocket(char *servername, int port) {
	long nHostAddress;
	struct hostent* pHostInfo;   

    hSocket = socket(AF_INET, SOCK_STREAM, 0);	
	
    if(hSocket == SOCKET_ERROR) {
        printf("\nCould not make a socket\n");
		throw "Could not create a socket";
    }

    // get IP address from name 
    pHostInfo = gethostbyname(servername);
    // copy address into long 
    memcpy(&nHostAddress, pHostInfo->h_addr, pHostInfo->h_length);	
	
    // fill address struct 
    Address.sin_addr.s_addr = nHostAddress;	
    Address.sin_port = htons(port);
    Address.sin_family = AF_INET;

    printf("\nConnecting to %s on port %d ... ", servername, port);

    // connect to host 
    while (connect(hSocket, (struct sockaddr*)&Address, sizeof(Address)) == SOCKET_ERROR) {
        printf("\nCould not connect to host on port %i ... will keep trying\n", port);
		
        usleep(2000000);
        //hSocket = SOCKET_ERROR;
		//throw "Could not connect to host";
        
        long nHostAddress;
        struct hostent* pHostInfo;
        
        hSocket = socket(AF_INET, SOCK_STREAM, 0);
        
        if(hSocket == SOCKET_ERROR) {
            printf("\nCould not make a socket\n");
            throw "Could not create a socket";
        }
        
        // get IP address from name
        pHostInfo = gethostbyname(servername);
        // copy address into long
        memcpy(&nHostAddress, pHostInfo->h_addr, pHostInfo->h_length);
        
        // fill address struct
        Address.sin_addr.s_addr = nHostAddress;
        Address.sin_port = htons(port);
        Address.sin_family = AF_INET;
        
        printf("\nConnecting to %s on port %d ... ", servername, port);
    }
	printf("completed\n");
}

int ClientSocket::SendMessage(char *message, int len) {
	int bsent = 0;

//	printf("message sent: %s\n", message);
	if ((bsent = send(hSocket, message, len, 0)) == SOCKET_ERROR) {
		printf("error sending socket message\n");
        close(hSocket);
		exit(0);
//		quit = true;
	}

	return bsent;
}

int ClientSocket::ReadMessage(char *message) {
	int NumBytes = 0;

	memset(message, 0, MAX_BUFFER);
	NumBytes = recv(hSocket, message, 1024, 0);
	
	if (NumBytes == SOCKET_ERROR) {
        close(hSocket);
		exit(1);
//		quit = true;
//		fprintf(stderr,"Socket Error\n");
//		throw "Error Reading From Socket";
	}
    message[NumBytes] = '\0';

	return NumBytes;
}
