cmake_minimum_required(VERSION 3.18)
project(smash)

set(CMAKE_CXX_STANDARD 11)

add_executable(smash smash.c commands.c commands.h signals.c signals.h)