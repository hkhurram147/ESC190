cmake_minimum_required(VERSION 3.17)
project(project_1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(project_1
        testing.c)
