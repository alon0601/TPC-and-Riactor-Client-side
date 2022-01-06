#include <stdlib.h>
#include <iostream>
#include <thread>
#include <boost/thread.hpp>
#include "ConnectionHandler.h"
#include "../include/InputSender.h"
#include "../include/ServerListener.h"

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    std::atomic<int>* shouldTerminate = new std::atomic<int>(1);
    InputSender inputer(connectionHandler, *shouldTerminate);
    ServerListener listener(connectionHandler,*shouldTerminate);

    std::thread writer (&InputSender::run, inputer);
    std::thread reader (&ServerListener::run, listener);

    reader.join();

    writer.join();
    delete shouldTerminate;
    return 0;
}
