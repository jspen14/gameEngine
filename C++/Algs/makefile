PROGRAM = igPlayer
cpp_FILES = main.cpp client_socket.cpp Game.cpp Player.cpp RandomPlayer.cpp GeneralizedFP.cpp Human.cpp ExpertAlg.cpp minimaxLog.cpp Solution.cpp NBS.cpp Expert.cpp TriggerStrat.cpp SelectionMechanism.cpp RandomSelector.cpp PlusPlusSelector.cpp igNBS.cpp MaxMinExpert.cpp ListenerExpert.cpp ExploiterExpert.cpp
c_FILES =
CC = g++

CFLAGS = -O -g
LIBs = 

OBJECTS = $(c_FILES:.c=.o) $(cpp_FILES:.cpp=.o)

.SUFFIXES: .o .cpp .c

.cpp.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES) -Wno-deprecated

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES)

$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $(INCLUDES) $(OBJECTS) $(LIBS)

clean:
	rm *.o

rebuild: clean
	make
