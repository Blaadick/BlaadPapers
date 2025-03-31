cmake_minimum_required(VERSION 3.30)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VER_RAW)
string(STRIP ${VER_RAW} PROJECT_VERSION)

project(
    BlaadPapers
    DESCRIPTION "Super wallpaper manager"
    VERSION ${PROJECT_VERSION}
)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(blaadpapers)
add_subdirectory(blaadpapers-daemon)
