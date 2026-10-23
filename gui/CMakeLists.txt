file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE QMLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS qml/*.qml)
file(GLOB_RECURSE ICONS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS resource/icon/*.svg resource/icon/*.theme)

find_package(Qt6 REQUIRED COMPONENTS QuickControls2 Concurrent Svg)

qt_standard_project_setup(REQUIRES 6.12)
qt_add_executable(blaadpapers WIN32 ${SOURCES} ${HEADERS})
qt_add_qml_module(blaadpapers
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QMLS}
)

qt_add_resources(blaadpapers icons
    PREFIX /icons
    BASE resource/icon
    FILES ${ICONS}
)

target_link_libraries(blaadpapers PRIVATE shared Qt6::QuickControls2 Qt6::Concurrent Qt6::Svg)
target_include_directories(blaadpapers PRIVATE include)

if(WIN32)
    set(BINARY_NAME blaadpapers)
    set(FILE_DESCRIPTION ${PROJECT_NAME})
    configure_file(${PROJECT_SOURCE_DIR}/shared/resource/windows/blaadpapers.rc.in blaadpapers.rc @ONLY)
    target_sources(blaadpapers PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/blaadpapers.rc)
endif()
