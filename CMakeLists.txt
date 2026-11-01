cmake_minimum_required(VERSION 4.4)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")

project(BlaadPapers
    DESCRIPTION "Fast & clean wallpaper manager"
    VERSION 0.5.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_MODULE_STD ON)

add_subdirectory(cli)
add_subdirectory(gui)
add_subdirectory(shared)

if(WIN32)
    add_subdirectory(packaging/windows)
endif()
