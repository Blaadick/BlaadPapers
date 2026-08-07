file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE QMLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS qml/*.qml)
file(GLOB_RECURSE RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS resource/*.svg)
file(GLOB_RECURSE SHADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS resource/shader/*)

find_package(Qt6 REQUIRED COMPONENTS QuickControls2 Widgets Concurrent Svg)

qt_standard_project_setup(REQUIRES 6.11)
qt_add_executable(blaadpapers ${SOURCES} ${HEADERS})

qt_add_qml_module(blaadpapers
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QMLS}
    RESOURCES ${RESOURCES}
)

target_link_libraries(blaadpapers PRIVATE shared Qt6::QuickControls2 Qt6::Widgets Qt6::Concurrent Qt6::Svg)
target_include_directories(blaadpapers PRIVATE include)
