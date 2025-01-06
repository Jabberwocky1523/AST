project(AST)
include_directories(${PROJECT_SOURCE_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR} SRC)


add_executable(AST ${SRC})
