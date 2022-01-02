cmake_minimum_required(VERSION 3.20)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Boost_Echo_Client/include)

add_executable(Boost_Echo_Client
        Boost_Echo_Client/include/connectionHandler.h
        Boost_Echo_Client/src/connectionHandler.cpp
        Boost_Echo_Client/src/echoClient.cpp)
