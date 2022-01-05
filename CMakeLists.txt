cmake_minimum_required(VERSION 3.20)
project(Boost_Echo_Client)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(include)

add_executable(Boost_Echo_Client
        include/ConnectionHandler.h
        include/InputSender.h
        include/ServerListener.h
        src/ConnectionHandler.cpp
        src/echoClient.cpp
        src/InputSender.cpp
        src/ServerListener.cpp)
