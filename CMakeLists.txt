cmake_minimum_required(VERSION 4.3)

project(BlaadPapers
    DESCRIPTION "Fast & clean wallpaper manager"
    VERSION 0.3.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(cli)
add_subdirectory(gui)
add_subdirectory(shared)
