cmake_minimum_required(VERSION 3.20.2)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled main.c BasicStruct.h Compress.h DeCompress.h utils.h)
target_link_libraries(untitled m)