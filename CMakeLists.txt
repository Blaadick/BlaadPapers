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

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(blaadpapers ${SOURCES} ${HEADERS} "src/gui/MainWindow.ui")
target_link_libraries(blaadpapers PRIVATE Qt::Core Qt::Widgets)
target_include_directories(blaadpapers PRIVATE include lib)
target_compile_definitions(blaadpapers PRIVATE VERSION="${PROJECT_VERSION}")
