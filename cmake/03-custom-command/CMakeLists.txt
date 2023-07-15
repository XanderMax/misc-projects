cmake_minimum_required(VERSION 3.16)

project(find-python)

find_package(Python3 COMPONENTS Interpreter)

message(STATUS "PROJECT_SOURCE_DIR is \"${PROJECT_SOURCE_DIR}\"")

if(NOT DEFINED Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Python3 not found")
else()
    message(STATUS "Python3 path is ${Python3_EXECUTABLE}")
endif()

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/main.cpp
    COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/script.py ${PROJECT_SOURCE_DIR}/main.cpp.in ${CMAKE_BINARY_DIR}/main.cpp
    DEPENDS ${PROJECT_SOURCE_DIR}/main.cpp.in ${PROJECT_SOURCE_DIR}/script.py
)

add_custom_target(
    GenerateMainCpp
    ALL
    DEPENDS ${CMAKE_BINARY_DIR}/main.cpp
)

add_executable(
    MainExec
    ${CMAKE_BINARY_DIR}/main.cpp
)

add_dependencies(
    MainExec
    GenerateMainCpp
)