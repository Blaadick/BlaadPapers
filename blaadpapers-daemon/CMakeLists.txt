file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(blaadpapers-daemon ${SOURCES})
target_include_directories(blaadpapers-daemon PRIVATE ${CMAKE_SOURCE_DIR}/lib)
