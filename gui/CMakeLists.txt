file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE QMLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS qml/*.qml)
file(GLOB_RECURSE ICONS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS resource/icon/*.svg)

find_package(Qt6 REQUIRED COMPONENTS QuickControls2 Concurrent Svg)

qt_standard_project_setup(REQUIRES 6.12)
qt_add_executable(blaadpapers ${SOURCES} ${HEADERS})
qt_add_qml_module(blaadpapers
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QMLS}
    RESOURCES ${ICONS}
)

target_link_libraries(blaadpapers PRIVATE shared Qt6::QuickControls2 Qt6::Concurrent Qt6::Svg)
target_include_directories(blaadpapers PRIVATE include)
