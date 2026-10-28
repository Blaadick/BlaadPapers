file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)

find_package(CURL REQUIRED)
find_package(yyjson REQUIRED CONFIG)
find_package(imageinfo REQUIRED CONFIG)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFmpeg REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil libswscale)

add_library(shared STATIC ${SOURCES} ${HEADERS})
target_include_directories(shared PUBLIC include)
target_link_libraries(shared PUBLIC
    CURL::libcurl
    yyjson::yyjson
    imageinfo::imageinfo
    PkgConfig::FFmpeg
)

if(NOT WIN32)
    pkg_check_modules(LibSecret REQUIRED IMPORTED_TARGET libsecret-1)
    target_link_libraries(shared PUBLIC PkgConfig::LibSecret)
endif()

target_compile_definitions(shared PUBLIC
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_NAME="${PROJECT_NAME}">
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}">
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_VERSION="${PROJECT_VERSION}">
    $<$<COMPILE_LANGUAGE:CXX>:PROJECT_USER_AGENT="${PROJECT_NAME}/${PROJECT_VERSION} \(${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}\)">
)
