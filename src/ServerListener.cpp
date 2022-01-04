//
// Created by spl211 on 2.1.2022.
//


#include "../include/ServerListener.h"
#include <string>
#include <iostream>
using std::string;
using namespace std;

ServerListener::ServerListener(ConnectionHandler &connect, std::atomic<bool>& terminate) : _connect(connect), _terminate(terminate){

}

void ServerListener::run() {
    while (true) {
        string ans;
        char *byte = new char();
        if (!_connect.getBytes(byte, 4)) {
            cout << "Disconnected please connect and try again" << endl;
            _terminate = true;
            break;
        }
        auto opcode = bytesToShort(byte);
        if (opcode == 9) {
            char pmOrPost = byte[3];
            string pmOrPostS;
            if (pmOrPost == 1)
                pmOrPostS = "Post";
            else
                pmOrPostS = "PM";
            while (_connect.getFrameAscii(ans, '0')) {
                ans.append(" ");
            }
            cout << "Notification " + ans << endl;
        }

        if (opcode == 11) {
            auto secondOpcode = bytesToShort(&byte[2]);
            cout << "ERROR " + std::to_string(secondOpcode) << std::endl;
        }

        if (opcode == 10) {
            auto secondOpcode = bytesToShort(&byte[2]);
            if (secondOpcode == 7 || secondOpcode == 8) {
                while (_connect.getLine(ans)) {
                    cout << "ACK " + ans << endl;
                }
            } else {
                if(secondOpcode == 4) {
                    _connect.getFrameAscii(ans,'\0');
                    cout << "ACK " + to_string(opcode) + " " + to_string(secondOpcode) + " " + ans << endl;
                }
                else {
                    cout << "ACK " + to_string(opcode)+ " " + to_string(secondOpcode) << endl;
                }
                if (secondOpcode == 3) {
                    _terminate = true;
                    break;
                }
            }
            cout << _terminate << endl;
            delete byte;
        }
    }
}


short ServerListener::bytesToShort(char* bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

bool ServerListener::isTerminate() {
    return _terminate;
}
//REGISTER MOMI 123 12-12-2021
//REGISTER MIMO 21 12-11-2011