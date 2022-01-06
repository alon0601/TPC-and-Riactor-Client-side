//
// Created by spl211 on 2.1.2022.
//

#include "../include/ServerListener.h"
#include <string>
#include <iostream>
#include <ConnectionHandler.h>

using std::string;
using namespace std;

ServerListener::ServerListener(ConnectionHandler &connect, std::atomic<int>& terminate) : _connect(connect), _terminate(terminate){

}

void ServerListener::run() {
    while (true) {
        string ans;
        char *byte = new char();
        if (!_connect.getBytes(byte, 2)) {
            cout << "Disconnected please connect and try again" << endl;
            _terminate = true;
            break;
        }
        cout<< "get new opcode " <<endl;
        auto opcode = bytesToShort(byte);
        if (opcode == 9) {
            cout << "enter noti" << endl;
            _connect.getBytes(&byte[2],1);
            char pmOrPost = byte[2];
            string pmOrPostS;
            if (pmOrPost == '\0')
                pmOrPostS = "PM";
            else
                pmOrPostS = "Post";
            string postingUser;
            _connect.getFrameAscii(postingUser,'\0');
            postingUser = postingUser.substr(0,postingUser.size()-1);
            string content;
            _connect.getFrameAscii(content,'\0');
            content = content.substr(0,content.size()-1);
            cout << "Notification " + pmOrPostS + " " + postingUser + " " + content << endl;
            cout << "exit noti" << endl;
        }
        if (opcode == 11) {
            cout << "enter erorr" << endl;
            _connect.getBytes(&byte[2],2);
            auto secondOpcode = bytesToShort(&byte[2]);
            cout << "ERROR " + std::to_string(secondOpcode) << std::endl;
        }
        if (opcode == 10) {
            cout << "enter ack " << endl;
            _connect.getBytes(&byte[2],2);
            auto secondOpcode = bytesToShort(&byte[2]);
            if (secondOpcode == 7 || secondOpcode == 8) {
                cout << "enter stats" << endl;
                _connect.getBytes(&byte[4],8);
                auto age = bytesToShort(&byte[4]);
                auto numPosts = bytesToShort(&byte[6]);
                auto numFollowers = bytesToShort(&byte[8]);
                auto numFollowing = bytesToShort(&byte[10]);
                cout << "ACK " + to_string(opcode) + " " + to_string(secondOpcode) + " age: " + to_string(age) + " nPosts: " + to_string(numPosts) + " nFollowers: " + to_string(numFollowers) + " nFollowing " + to_string(numFollowing)<< endl;
            } else {
                if(secondOpcode == 4) {
                    cout << "enter follow" << endl;
                    _connect.getFrameAscii(ans,'\0');
                    cout << "ACK " + to_string(opcode) + " " + to_string(secondOpcode) + " " + ans.substr(0,ans.size()-1) << endl;
                }
                else {
                    cout << "enter acks " << endl;
                    cout << "ACK " + to_string(opcode)+ " " + to_string(secondOpcode) << endl;
                }
                if (secondOpcode == 3) {
                    cout << "enter terminate" << endl;
                    _terminate = -1;
                    break;
                }
            }
        }
        delete byte;
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
