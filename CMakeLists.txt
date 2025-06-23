cmake_minimum_required(VERSION 4.0)
include(FetchContent)

file(READ VERSION VER_RAW)
string(STRIP ${VER_RAW} PROJECT_VERSION)

project(
    BlaadPapers
    DESCRIPTION "Super wallpaper manager"
    VERSION ${PROJECT_VERSION}
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Quick Qml)

fetchcontent_declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
fetchcontent_makeavailable(json)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp)

qt_standard_project_setup(REQUIRES 6.9)
qt_add_executable(blaadpapers ${SOURCES} ${HEADERS})
target_link_libraries(blaadpapers PRIVATE Qt6::Widgets Qt6::Gui Qt6::Quick Qt6::Qml nlohmann_json::nlohmann_json)
target_include_directories(blaadpapers PRIVATE include lib)
target_compile_definitions(blaadpapers PRIVATE VERSION="${PROJECT_VERSION}")
