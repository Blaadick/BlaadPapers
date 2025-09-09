cmake_minimum_required(VERSION 4.1)

project(BlaadPapers
    DESCRIPTION "Super wallpaper manager"
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(QT_FORCE_CMP0156_TO_VALUE NEW)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Quick Qml ShaderTools)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE QMLS RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS qml/*.qml)
file(GLOB_RECURSE SHADERS RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS shader/*)

qt_standard_project_setup(REQUIRES 6.9)
qt_add_executable(blaadpapers ${SOURCES} ${HEADERS})

qt_add_qml_module(blaadpapers
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QMLS}
)

qt_add_shaders(blaadpapers
    PREFIX "/"
    FILES ${SHADERS}
)

target_link_libraries(blaadpapers PRIVATE Qt6::Core Qt6::Concurrent Qt6::Quick Qt6::Qml)
target_include_directories(blaadpapers PRIVATE include)
target_compile_definitions(blaadpapers PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    PROJECT_VERSION="${PROJECT_VERSION}"
)
