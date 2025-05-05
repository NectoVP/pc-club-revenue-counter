cmake_minimum_required(VERSION 3.12)
project(src)

add_library(${PROJECT_NAME}
    Validator.h
    Validator.cpp
    ValidationResultHandler.h
    ValidationResultHandler.cpp
    RevenueCounter.h
    RevenueCounter.cpp
    Utility.h
    Utility.cpp
)

add_subdirectory(events)

target_link_libraries(${PROJECT_NAME} PUBLIC events)