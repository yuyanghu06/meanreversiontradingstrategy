cmake_minimum_required(VERSION 3.29)
project(meanreversiontradingstrategy)

set(CMAKE_CXX_STANDARD 20)

add_executable(meanreversiontradingstrategy main.cpp
        dataCalculations.h
        dataCalculations.cpp
        tradeSession.h
        tradeSession.cpp
)

include_directories("/Users/yuyang/Documents/CLion Projects/libs/eigen-3.4.0")
include_directories("/Users/yuyang/Documents/CLion Projects/libs/boost_1_86_0")

