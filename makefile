CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system  -pthread

all: EchoClient
	g++ -o bin/EchoClient bin/ConnectionHandler.o bin/echoClient.o bin/InputSender.o bin/ServerListener.o $(LDFLAGS)

EchoClient: bin/ConnectionHandler.o bin/echoClient.o bin/InputSender.o bin/ServerListener.o

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/InputSender.o: src/InputSender.cpp
	g++ $(CFLAGS) -o bin/InputSender.o src/InputSender.cpp

bin/ServerListener.o: src/ServerListener.cpp
	g++ $(CFLAGS) -o bin/ServerListener.o src/ServerListener.cpp

.PHONY: clean
clean:
	rm -f bin/*

