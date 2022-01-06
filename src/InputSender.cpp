//
// Created by spl211 on 02/01/2022.
//

#include <atomic>
#include <cstring>
#include "../include/InputSender.h"
using namespace  std;

InputSender::InputSender(ConnectionHandler& myCon, std::atomic<int> &running): opCodeMap({}),myCon(myCon), shouldTerminate(running) {
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
    if(opCode == 3){
        shouldTerminate = 0;
    }
    if(opCode == 4){
        bytes[2] = line[0];
        for(int i = 2;i < line.size();i++){
            bytes[i + 1] = line[i];
        }
        bytes[len-2] = '\0';
    }
    if (opCode == 5 || opCode == 12){
        for (int i = 0; i < line.size(); ++i) {
            bytes[i+2] = line[i];
        }
        bytes[len-2] = '\0';
    }

    if(opCode == 6){
        int i = 0;
        int fond = line.find_first_of(" ");
        while(i<fond){
            bytes[i+2] = line[i];
            i++;
        }
        bytes[i+2] = '\0';
        i++;
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
}

int InputSender::makeSize( std::string line) {
    int commendCode = line.find_first_of(" ");
    short opCode = opCodeMap[line.substr(0,commendCode)];
    line = line.substr(commendCode + 1);
    if (opCode == 1 || opCode == 5 || opCode == 6 || opCode == 8 || opCode == 12){
        return line.size() + 4;
    }
    else{
        return line.size() + 3;
    }
}

void InputSender::run() {
    while (shouldTerminate == 1) {
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
        while(shouldTerminate == 0){
        }
        if(shouldTerminate == -1){
            break;
        }
    }
}


