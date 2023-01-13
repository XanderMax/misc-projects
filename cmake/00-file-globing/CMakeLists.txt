cmake_minimum_required(VERSION 3.16)

project(file-globing)

file(GLOB_RECURSE FOLDER_FILES ${PROJECT_SOURCE_DIR} *.file)

foreach(COLLECTED_FILE ${FOLDER_FILES})
    message(STATUS ${FOLDER_FILES})
endforeach(COLLECTED_FILE)

# Next two lines organize folder files and main.cpp into subfolders in IDE
source_group(FolderFiles FILES ${FOLDER_FILES})
source_group(Main FILES ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(file-globing
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${FOLDER_FILES}
)
