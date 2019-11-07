cmake_minimum_required(VERSION 3.15)
project(Akinator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Akinator src/main.cpp src/Akinator.h src/Akinator.cpp src/Text.h src/Text.cpp src/Settings.h src/Node.h)