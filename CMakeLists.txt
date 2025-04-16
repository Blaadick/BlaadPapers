cmake_minimum_required(VERSION 3.31)

file(READ "VERSION" VER_RAW)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

string(STRIP ${VER_RAW} PROJECT_VERSION)

project(
    BlaadPapers
    DESCRIPTION "Super wallpaper manager"
    VERSION ${PROJECT_VERSION}
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(blaadpapers ${SOURCES} ${HEADERS})
add_compile_definitions(VERSION="${PROJECT_VERSION}")
target_include_directories(blaadpapers PRIVATE include lib)
target_link_libraries(blaadpapers PRIVATE Qt::Core Qt::Gui Qt::Widgets)
