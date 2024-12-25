cmake_minimum_required(VERSION 3.28)
project(Zachet C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES 2nallocator.c)

#add_library(Zachet SHARED ${SOURCE_FILES})
add_executable(Zachet main.c)