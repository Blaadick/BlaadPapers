cmake_minimum_required(VERSION 3.30)
project(BlaadPapers)

set(CMAKE_CXX_STANDARD 26)

add_executable(BlaadPapers main.cpp
    Wallpaper.cpp
    Wallpaper.hpp
)
