file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)

find_package(yyjson REQUIRED CONFIG)
find_package(CURL REQUIRED)
find_package(imageinfo REQUIRED CONFIG)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFmpeg REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil libswscale)
pkg_check_modules(VIPS REQUIRED IMPORTED_TARGET vips-cpp)

add_library(shared STATIC ${SOURCES} ${HEADERS})
target_include_directories(shared PUBLIC include)
target_link_libraries(shared PUBLIC yyjson::yyjson CURL::libcurl imageinfo::imageinfo PkgConfig::FFmpeg PkgConfig::VIPS)
target_compile_definitions(shared PUBLIC
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    PROJECT_VERSION="${PROJECT_VERSION}"
    PROJECT_USER_AGENT="${PROJECT_NAME}/${PROJECT_VERSION}"
)
