cmake_minimum_required(VERSION 3.20)
project(rt)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR} SRC)

add_library(rt SHARED ${SRC})
target_link_libraries(rt ${PROJECT_SOURCE_DIR}/libre.dylib)
target_link_libraries(rt ${PROJECT_SOURCE_DIR}/libsh.dylib)