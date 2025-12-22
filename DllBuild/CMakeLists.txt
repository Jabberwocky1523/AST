cmake_minimum_required(VERSION 3.20)
project(AST)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# add_subdirectory(std)

include_directories(${PROJECT_SOURCE_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR} SRC)
# include_directories(${PROJECT_SOURCE_DIR}/lib)

# add_executable(AST ${SRC})
# target_link_libraries(AST ${PROJECT_SOURCE_DIR}/lib/libcomgo.so)
# target_link_libraries(AST STD)
add_library(sh SHARED ${SRC})