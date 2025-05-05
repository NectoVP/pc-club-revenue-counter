cmake_minimum_required(VERSION 3.12)
project(events)

add_library(${PROJECT_NAME} 
    EventHandler.h
    EventHandler.cpp
    ClientArrive.h
    ClientArrive.cpp
    ClientWait.h
    ClientWait.cpp
    ClientSit.h
    ClientSit.cpp
    ClientLeft.h
    ClientLeft.cpp
)