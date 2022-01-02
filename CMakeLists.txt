cmake_minimum_required(VERSION 3.20)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        include/ServerListener.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        src/ServerListener.cpp)
