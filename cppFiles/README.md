# SocketProgrammingExample

## Build Set-Up

# Build Server
g++ server.cpp -o server

# Build Client
g++ client.cpp -o clients

## Run Program

# Start Server -- ./server [portNum]
./server 8000

# Start Client -- ./client [IP_Address] [portnum]
./client 127.0.0.1 8000
