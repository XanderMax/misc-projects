cmake_minimum_required(VERSION 3.16)

project(map-file-generation)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")

if (WIN32) # https://stackoverflow.com/a/71592676/5405069
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(WIN32)

add_library(map_file_generation_lib0 SHARED mylib0.cpp)
add_library(map_file_generation_lib1 SHARED mylib1.cpp)

add_executable(map_file_generation_exe
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${FOLDER_FILES}
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_link_options(map_file_generation_exe PRIVATE "-Wl,-map,map_file_generation_exe.map")
    target_link_options(map_file_generation_lib0 PRIVATE "-Wl,-map,map_file_generation_lib0.map")
    target_link_options(map_file_generation_lib1 PRIVATE "-Wl,-map,map_file_generation_lib1.map")
endif()

target_link_libraries(map_file_generation_exe 
    map_file_generation_lib0
    map_file_generation_lib1
)
