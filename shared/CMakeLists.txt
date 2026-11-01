file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE MODULES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.cppm)

find_package(BSU REQUIRED CONFIG)
find_package(CURL REQUIRED)
find_package(yyjson REQUIRED CONFIG)
find_package(imageinfo REQUIRED CONFIG)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFmpeg REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil libswscale)

add_library(shared STATIC ${SOURCES})
target_sources(shared PUBLIC FILE_SET CXX_MODULES BASE_DIRS include FILES ${MODULES})
target_link_libraries(shared PUBLIC
    BSU::bsu
    CURL::libcurl
    yyjson::yyjson
    imageinfo::imageinfo
    PkgConfig::FFmpeg
)

if(WIN32)
    # TODO Remove when MinGW GCC emits COMDAT for local statics of inline functions from modules
    target_link_options(shared PUBLIC -Wl,--allow-multiple-definition)
else()
    # TODO Remove when Qt stops adding it: matches Qt's flag so the GUI reuses the std and blaadpapers BMIs instead of rebuilding them
    target_compile_options(shared PUBLIC -mno-direct-extern-access)
endif()

target_compile_definitions(shared PUBLIC
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_NAME="${PROJECT_NAME}">
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}">
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_VERSION="${PROJECT_VERSION}">
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_USER_AGENT="${PROJECT_NAME}/${PROJECT_VERSION} \(${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}\)">
)
