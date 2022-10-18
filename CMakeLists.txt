cmake_minimum_required(VERSION 3.23)
project(CasseBrick C)

set(CMAKE_C_STANDARD 99)

add_executable(CasseBrick main.c keyboardHandler.c collisionChecker.c)
