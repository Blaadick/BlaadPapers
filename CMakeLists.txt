cmake_minimum_required(VERSION 3.30)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VER_RAW)
string(STRIP ${VER_RAW} VERSION)

project(blaadpapers VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_compile_definitions(VERSION="${VERSION}")

add_executable(
    blaadpapers
    src/main.cpp
    src/Wallpaper.cpp
)

target_include_directories(blaadpapers PRIVATE include lib)
