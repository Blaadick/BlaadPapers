file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(blaadpapers ${SOURCES})
target_include_directories(blaadpapers PRIVATE include ${CMAKE_SOURCE_DIR}/lib)
target_compile_definitions(blaadpapers PRIVATE VERSION="${PROJECT_VERSION}")
