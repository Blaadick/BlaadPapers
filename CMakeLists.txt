cmake_minimum_required(VERSION 4.0)

file(READ "VERSION" VER_RAW)
string(STRIP ${VER_RAW} PROJECT_VERSION)

project(
    BlaadPapers
    DESCRIPTION "Super wallpaper manager"
    VERSION ${PROJECT_VERSION}
)

set(CMAKE_CXX_STANDARD 26)

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Widgets)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp)

qt_standard_project_setup()
qt_add_executable(blaadpapers ${SOURCES} ${HEADERS})
target_link_libraries(blaadpapers PRIVATE stdc++ Qt6::Core Qt6::Widgets)
target_include_directories(blaadpapers PRIVATE include lib)
target_compile_definitions(blaadpapers PRIVATE VERSION="${PROJECT_VERSION}")
