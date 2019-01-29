#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

#define SOCKET_ERROR -1
#define QUEUE_SIZE 1
#define SOCKET int

class ClientSocket {
	public:
		ClientSocket(char *servername, int port);
		int SendMessage(char *, int len);	
		int ReadMessage(char *);
		
		SOCKET hSocket;
	private:
		struct sockaddr_in Address;
};

#endif
