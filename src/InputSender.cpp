//
// Created by spl211 on 02/01/2022.
//

#include <atomic>
#include <cstring>
#include "../include/connectionHandler.h"
#include "../include/InputSender.h"
using namespace  std;

InputSender::InputSender(ConnectionHandler& myCon, std::atomic<bool> &running): opCodeMap({}),myCon(myCon), shouldTerminate(running) {
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

void InputSender::changeToZero(char charArr[], int length) {
    for (signed int i = 0; i < length; i++) {
        if (charArr[i] == ' ')
            charArr[i] = '\0';
    }
}

void InputSender::parse(std::string line, char bytes[]) {
    int commendCode = line.find_first_of(" ");
    short opCode = opCodeMap[line.substr(0,commendCode)];
    int len = makeSize(line);
    line = line.substr(commendCode + 1);
    bytes[0] = ((opCode >> 8) & 0xFF);
    bytes[1] = (opCode & 0xFF);
    if (opCode == 1){
        for (int i = 0; i < line.size(); ++i) {
            bytes[i+2] = line[i];
        }
        changeToZero(bytes,len);
        bytes[len-2] = '\0';
    }

    if (opCode == 2){
        for(int i=0;i<line.size();i++){
            bytes[i+2] = line[i];
        }
        changeToZero(bytes,len);
    }

    if(opCode == 4){
        bytes[2] = line[0];
        for(int i = 2;i < line.size();i++){
            bytes[i + 1] = line[i];
        }
        bytes[len-2] = '\0';
    }
    if (opCode == 5){
        bytes[len-2] = '\0';
    }

    if(opCode == 6){
        int i = 0;
        size_t fond = line.find_first_of(" ");
        while(i<fond){
            bytes[i+2] = line[i];
            i++;
        }
        bytes[i+2] = 0;
        while(i<line.size()) {
            bytes[i + 2] = line[i];
            i++;
        }
        bytes[i+2] = '\0';
    }

    if(opCode == 8){
        int i = 0;
        while(i < line.size()){
            bytes[i+2] = line[i];
            i++;
        }
        bytes[i+2] = '\0';
    }

    if (opCode == 12){
        bytes[len-2] = '\0';
    }
//    if (opCode != 4 && opCode != 6){
//        for (int i = 0; i < line.size(); ++i) {
//            bytes[i+2] = line[i];
//        }
//        if( opCode != 5) { //make space to zero only if it's not post/pm
//            changeToZero(bytes, len);
//        }
//        if (opCode != 2 && opCode != 3 && opCode != 7){
//            bytes[line.size() + 2] = '\0';
//        }
//    }
//    else if (opCode == 4){
//        bytes[2] = line[0] - '0';
//        for (int i = 1; i < line.size(); ++i) {
//            bytes[i+2] = line[i];
//        }
//    }
//    else{ //need to check if works
//        std::string user(line.substr(0, line.find_first_of(" ")));
//        for(unsigned int i = 0; i < user.size();i++){
//            bytes[2+i] = user[i];
//        }
//        bytes[2 + user.size()] = '\0';//put a "0" after the username
//        std::string pm = line.substr(user.size() + 1, line.size());//save the content of the pm
//        for(unsigned int i = 0;i < pm.size(); i++){
//            bytes[i + 3 + user.size()] = pm[i];
//        }
//        bytes[line.size() + 2] = '\0';
//    }
}

int InputSender::makeSize( std::string line) {
    int commendCode = line.find_first_of(" ");
    short opCode = opCodeMap[line.substr(0,commendCode)];
    line = line.substr(commendCode + 1);
    if (opCode == 1 || opCode == 6 || opCode == 8 || opCode == 12){
        return line.size() + 4;
    }
    else{
        return line.size() + 3;
    }
}

void InputSender::run() {
    while (!shouldTerminate) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = makeSize(line);
        char bytes[len];
        parse(line, bytes);
        bytes[len-1] = ';';
        if (!myCon.sendBytes(bytes, len)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
    }
}


