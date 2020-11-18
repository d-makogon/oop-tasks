cmake_minimum_required(VERSION 3.0)
project(task2)

set(CMAKE_CXX_STANDARD 17)

set(SRC src/main.cpp src/main.cpp src/gameField.cpp src/gameField.h src/game.cpp src/game.h src/views/gameView.h src/views/consoleView.cpp src/views/consoleView.h src/views/sfmlView.cpp src/views/sfmlView.h src/gamers/gamer.h src/gamers/consoleGamer.cpp src/gamers/consoleGamer.h src/gamers/gamer.cpp)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(task2 ${SRC})

target_link_libraries(task2 sfml-graphics sfml-audio)