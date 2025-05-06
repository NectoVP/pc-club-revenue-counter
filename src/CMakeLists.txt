cmake_minimum_required(VERSION 3.12)

add_subdirectory(output)
add_subdirectory(events)

add_library(src
    Validator.h
    Validator.cpp
    ValidationResultHandler.h
    ValidationResultHandler.cpp
    RevenueCounter.h
    RevenueCounter.cpp
    Utility.h
    Utility.cpp
)


target_link_libraries(src PUBLIC output events)