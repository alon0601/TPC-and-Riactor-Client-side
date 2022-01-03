//
// Created by spl211 on 02/01/2022.
//

#include <atomic>
#include <cstring>
#include "../include/connectionHandler.h"
#include "../include/InputSender.h"
using namespace  std;

inputSender::inputSender(ConnectionHandler& myCon, std::atomic<bool> &running): opCodeMap({}),myCon(myCon), shouldTerminate(shouldTerminate) {
    opCodeMap["REGISTER"] = 1;
    opCodeMap["LOGIN"] = 2;
    opCodeMap["LOGOUT"] = 3;
    opCodeMap["FOLLOW"] = 4;
    opCodeMap["POST"] = 5;
    opCodeMap["PM"] = 6;
    opCodeMap["LOGSTAT"] = 7;
    opCodeMap["STAT"] = 8;
    opCodeMap["BLOCK"] = 12;
}

void inputSender::changeToZero(char charArr[], int length) {
    for (signed int i = 0; i < length; i++) {
        if (charArr[i] == ' ')
            charArr[i] = '\0';
    }
}

void inputSender::parse(std::string line, char bytes[]) {
    int commendCode = line.find_first_of(" ");
    short opCode = opCodeMap[line.substr(0,commendCode)];
    line = line.substr(commendCode + 1);
    bytes[0] = ((opCode >> 8) & 0xFF);
    bytes[1] = (opCode & 0xFF);

    if (opCode != 4 && opCode != 6){
        for (int i = 0; i < line.size(); ++i) {
            bytes[i+2] = line[i];
        }
        if( opCode != 5) { //make space to zero only if it's not post/pm
            changeToZero(bytes, strlen(bytes));
        }
        if (opCode != 2 && opCode != 3 && opCode != 7){
            bytes[line.size() + 2] = '\0';
        }
    }
    else if (opCode == 4){
        bytes[2] = line[0] - '0';
        for (int i = 1; i < line.size(); ++i) {
            bytes[i+2] = line[i];
        }
    }
    else{ //need to check if works
        std::string user(line.substr(0, line.find_first_of(" ")));
        for(unsigned int i = 0; i < user.size();i++){
            bytes[2+i] = user[i];
        }
        bytes[2 + user.size()] = '\0';//put a "0" after the username
        std::string pm = line.substr(user.size() + 1, line.size());//save the content of the pm
        for(unsigned int i = 0;i < pm.size(); i++){
            bytes[i + 3 + user.size()] = pm[i];
        }
        bytes[line.size() + 2] = '\0';
    }
}

int inputSender::makeSize( std::string line) {
    int commendCode = line.find_first_of(" ");
    short opCode = opCodeMap[line.substr(0,commendCode)];
    line = line.substr(commendCode + 1);
    if (opCode == 1 || opCode == 4 || opCode == 6 || opCode == 8 || opCode == 12){
        return line.size() + 3;
    }
    else{
        return line.size() + 2;
    }
}

void inputSender::run() {
    while (!shouldTerminate) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = makeSize(line);
        char bytes[len];
        parse(line, bytes);
        if (!myCon.sendBytes(bytes, len)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
    }
}


