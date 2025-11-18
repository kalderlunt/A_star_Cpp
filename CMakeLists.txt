cmake_minimum_required(VERSION 4.0)
project(A_star_Cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(A_star_Cpp main.cpp
        "A Star/AStar.cpp"
        "A Star/AStar.h")
