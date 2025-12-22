cmake_minimum_required(VERSION 3.20)
project(user)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(.)
include_directories(..)
aux_source_directory(. SRC)

add_library(user SHARED ${SRC})

# add_executable(user ${SRC})
target_link_libraries(user ${PROJECT_SOURCE_DIR}/libsh.so)

# target_link_libraries(user ${PROJECT_SOURCE_DIR}/build/libuser.dylib)