cmake_minimum_required(VERSION 3.16)

project(example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.3 COMPONENTS Quick Gui REQUIRED)
qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

add_executable(example
    main.cpp
    types.h
)

target_link_libraries(example PRIVATE Qt6::Gui Qt6::Quick)
