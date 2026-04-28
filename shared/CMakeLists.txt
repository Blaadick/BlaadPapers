file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFmpeg REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil libswscale)
pkg_check_modules(VIPS REQUIRED vips)

add_library(shared STATIC ${SOURCES} ${HEADERS})
target_include_directories(shared PUBLIC include ${VIPS_INCLUDE_DIRS})
target_link_libraries(shared PUBLIC Qt6::Core PkgConfig::FFmpeg ${VIPS_LIBRARIES})
