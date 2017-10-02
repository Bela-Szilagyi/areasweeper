cmake_minimum_required(VERSION 3.8)
project(areaSweeper)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/Area.cpp src/Area.h)
add_executable(areaSweeper ${SOURCE_FILES} src/Area.cpp src/Area.h)