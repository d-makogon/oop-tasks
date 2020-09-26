cmake_minimum_required(VERSION 3.0)
project(task1)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(googletest)

include_directories(googletest/include)
set(SRC src/main.cpp src/tritSet.cpp src/tritSet.h src/trit.cpp src/trit.h)
add_executable(task1 ${SRC})
target_link_libraries(task1 gtest gtest_main)