CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: EchoClient
	g++ -o bin/echoExample bin/ConnectionHandler.o bin/echoClient.o $(LDFLAGS)

EchoClient: bin/ConnectionHandler.o bin/echoClient.o

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

.PHONY: clean
clean:
	rm -f bin/*