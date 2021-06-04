cmake_minimum_required(VERSION 3.17)
project(project_2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(project_2
        c_img.c)
