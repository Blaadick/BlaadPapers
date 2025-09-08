cmake_minimum_required(VERSION 4.1)

file(READ VERSION VER_RAW)
string(STRIP ${VER_RAW} PROJECT_VERSION)

project(BlaadPapers
    DESCRIPTION "Super wallpaper manager"
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(QT_FORCE_CMP0156_TO_VALUE NEW)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Quick Qml)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE QMLS RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS qml/*.qml)

qt_standard_project_setup(REQUIRES 6.9)
qt_add_executable(blaadpapers ${SOURCES} ${HEADERS})

qt_add_qml_module(blaadpapers
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QMLS}
)

target_link_libraries(blaadpapers PRIVATE Qt6::Core Qt6::Concurrent Qt6::Quick Qt6::Qml)
target_include_directories(blaadpapers PRIVATE include)
target_compile_definitions(blaadpapers PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")
