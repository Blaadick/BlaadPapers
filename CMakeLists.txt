cmake_minimum_required(VERSION 3.30)
project(BlaadPapers)

set(CMAKE_CXX_STANDARD 26)

add_executable(blaadpapers src/main.cpp
    src/Wallpaper.cpp
    src/Wallpaper.hpp
)
