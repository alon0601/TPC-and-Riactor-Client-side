//
// Created by spl211 on 2.1.2022.
//

#ifndef BOOST_ECHO_CLIENT_SERVERLISTENER_H
#define BOOST_ECHO_CLIENT_SERVERLISTENER_H

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "ConnectionHandler.h"

using boost::asio::ip::tcp;

class ServerListener {
private:
    ConnectionHandler& _connect;
    std::atomic<bool>& _terminate;
public:
    void run();
    bool isTerminate();
    ServerListener(ConnectionHandler& connect,std::atomic<bool>& terminate);
    short bytesToShort(char* bytesArr);
};


#endif //BOOST_ECHO_CLIENT_SERVERLISTENER_H
