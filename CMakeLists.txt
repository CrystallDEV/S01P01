cmake_minimum_required(VERSION 3.8)
project(S02P01)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c priorityqueue.h)
add_executable(S02P01 ${SOURCE_FILES})