cmake_minimum_required(VERSION 3.16)

project(bool_option)

message(STATUS MY_BOOL_OPTION=${MY_BOOL_OPTION})

if(MY_BOOL_OPTION)
    message(STATUS "MY_BOOL_OPTION is ON")
else()
    message(STATUS "MY_BOOL_OPTION is OFF")
endif()
