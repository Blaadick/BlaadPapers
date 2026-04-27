file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)

add_executable(blaadpaperscli ${SOURCES} ${HEADERS})

target_link_libraries(blaadpaperscli PRIVATE shared)
target_include_directories(blaadpaperscli PRIVATE include)
target_compile_definitions(blaadpaperscli PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    PROJECT_VERSION="${PROJECT_VERSION}"
)
