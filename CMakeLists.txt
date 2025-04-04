cmake_minimum_required(VERSION 3.30)

file(READ "VERSION" VER_RAW)
file(GLOB_RECURSE SOURCES "src/*.cpp")

string(STRIP ${VER_RAW} PROJECT_VERSION)

project(
    BlaadPapers
    DESCRIPTION "Super wallpaper manager"
    VERSION ${PROJECT_VERSION}
)

set(CMAKE_CXX_STANDARD 26)

include_directories(include lib)
add_compile_definitions(VERSION="${PROJECT_VERSION}")
add_executable(blaadpapers ${SOURCES}
    src/OptionExecutor.cpp
    include/OptionExecutor.hpp)
