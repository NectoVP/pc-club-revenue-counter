cmake_minimum_required(VERSION 3.12)

add_library(events
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
    ClientEndOfDay.h
    ClientEndOfDay.cpp
)

target_link_libraries(events PUBLIC output)