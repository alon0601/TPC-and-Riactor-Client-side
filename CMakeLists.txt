cmake_minimum_required(VERSION 3.20)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

set(Boost_INCLUDE_DIR /cygdrive/c/cygwin64/usr/include)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Boost_Echo_Client include/connectionHandler.h include/InputSender.h include/ServerListener.h
        src/connectionHandler.cpp src/echoClient.cpp src/InputSender.cpp src/ServerListener.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})