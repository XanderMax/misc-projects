cmake_minimum_required(VERSION 3.16)

project(qmlmodule VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.3 COMPONENTS Quick Gui Qml REQUIRED)
qt_standard_project_setup(REQUIRES 6.3)

set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

add_executable(app
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/classifier.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/classifier.h
    ${CMAKE_CURRENT_LIST_DIR}/qml/res.qrc
)

target_link_libraries(app PRIVATE Qt6::Gui Qt6::Quick Qt6::Qml)