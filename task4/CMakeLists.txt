cmake_minimum_required(VERSION 3.16)
project(task4)

set(CMAKE_CXX_STANDARD 20)

add_executable(task4 main.cpp include/csvParser.h include/tuplePrint.h src/csvParser.cpp)