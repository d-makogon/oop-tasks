cmake_minimum_required(VERSION 3.0)
project(task2)

set(CMAKE_CXX_STANDARD 17)
#add_subdirectory(../googletest build)

#include_directories(../googletest/include)
set(SRC src/main.cpp src/worker.h src/workers.cpp src/workers.h src/workflowParser.cpp src/workflowParser.h src/workflowExecutor.cpp src/workflowExecutor.h src/argsParser.cpp src/argsParser.h)
add_executable(task2 ${SRC})
#target_link_libraries(task2 gtest gtest_main)