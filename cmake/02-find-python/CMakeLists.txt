cmake_minimum_required(VERSION 3.16)

project(find-python)

find_package(Python3 COMPONENTS Interpreter)

if(NOT DEFINED Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Python3 not found")
else()
    message(STATUS "Python3 path is ${Python3_EXECUTABLE}")
endif()

execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "print(\"This has been printed by python\")")