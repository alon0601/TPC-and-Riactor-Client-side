cmake_minimum_required(VERSION 3.20)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(src)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        include/InputSender.h
        include/ServerListener.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        src/InputSender.cpp
        src/ServerListener.cpp)
