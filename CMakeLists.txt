cmake_minimum_required(VERSION 3.25.0)
project(PcClubCounter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp)

enable_testing()

target_link_libraries(${PROJECT_NAME} PUBLIC src)