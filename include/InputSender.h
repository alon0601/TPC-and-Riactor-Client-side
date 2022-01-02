//
// Created by spl211 on 02/01/2022.
//
#include <iostream>
#include <unordered_map>
#include "connectionHendler.h"

#ifndef ASSIGNMENT3_SERVER_INPUTSENDER_H
#define ASSIGNMENT3_SERVER_INPUTSENDER_H


class inputSender {
private:
    std::unordered_map<std::string,short> opCodeMap;
    ConnectionHandler& myCon;
    std::atomic<bool>& shouldTerminate;
    void changeToZero(char charArr[], int length);
    int makeSize(std::string line);

public:
    inputSender(ConnectionHandler &myCon,std::atomic<bool>& shouldTerminate);
    void parse(std::string line, char bytes[]);
    void run();

};


#endif //ASSIGNMENT3_SERVER_INPUTSENDER_H