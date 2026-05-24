cmake_minimum_required(VERSION 3.20)
project(cpp_sdl2)
set(CMAKE_CXX_STANDARD 11)
project(ons)
set(CMAKE_CXX_STANDARD 23)

# Homebrew prefix for finding packages
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

# SDL2
find_package(SDL2 REQUIRED)

# SDL2_image
find_package(SDL2_image REQUIRED)

# SDL2_ttf
find_package(SDL2_ttf REQUIRED)

# SDL2_mixer
find_package(SDL2_mixer REQUIRED)

find_package(lua REQUIRED)

find_package(fontconfig REQUIRED)
find_package(jpeg REQUIRED)
find_package(ogg REQUIRED)

find_package(BZip2 REQUIRED)
include_directories(${LUA_INCLUDE_DIRS})
include_directories(${BZ2_IMAGE_INCLUDE_DIRS})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${FONTCONFIG_IMAGE_INCLUDE_DIRS})
include_directories(${SMPEG2_IMAGE_INCLUDE_DIRS})
include_directories(${LIBJPEG_IMAGE_INCLUDE_DIRS})
include_directories(${LIBOGG_IMAGE_INCLUDE_DIRS})
include_directories(${JPEG_IMAGE_INCLUDE_DIRS})
include_directories(${OGG_IMAGE_INCLUDE_DIRS})
include_directories(${LIBVORBIS_DEV_IMAGE_INCLUDE_DIRS})

include_directories(${BZIP2_INCLUDE_DIRS})

file(GLOB ASTCPP ./*.cpp)
file(GLOB_RECURSE built ./builtin_dll/*.cpp)
file(GLOB_RECURSE smid ./smid/*.cpp)

set(Lutf16 ./coding2utf16.cpp ./sjis2utf16.cpp ./gbk2utf16.cpp)
set(Lsardec ./DirectReader.cpp ./SarReader.cpp ${Lutf16})
set(Lnsadec ${Lsardec} ./NsaReader.cpp ${Lutf16})
set(Lsarconv ${Lsardec} ./sarconv.cpp ./conv_shared.cpp resize_image.cpp)
set(Lnsaconv ${Lnsadec} ./nsaconv.cpp ./conv_shared.cpp resize_image.cpp)
list(REMOVE_ITEM ASTCPP ${CMAKE_CURRENT_SOURCE_DIR}/./nsaconv.cpp ${CMAKE_CURRENT_SOURCE_DIR}/./nsadec.cpp ${CMAKE_CURRENT_SOURCE_DIR}/./onscripter_main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/./sarconv.cpp ${CMAKE_CURRENT_SOURCE_DIR}/./sardec.cpp)

link_libraries(${LUA_LIBRARIES})

link_libraries(${JPEG_LIBRARIES})
link_libraries(${OGG_LIBRARIES})
link_libraries(${BZIP2_LIBRARIES})

# 添加可执行文件
# add_executable(nsadec ./nsadec.cpp ${Lnsadec})
# add_executable(nsaconv ./nsaconv.cpp ${Lnsaconv})
# add_executable(sarconv ./sarconv.cpp ${Lsarconv})
# add_executable(sardec ./sardec.cpp ${Lsardec})
# add_executable(ons ./onscripter_main.cpp ${ASTCPP})
add_library(ons SHARED ./onscripter_main.cpp ${ASTCPP})
target_link_libraries(ons ${PROJECT_SOURCE_DIR}/libsh.dylib)
target_link_libraries(ons SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)