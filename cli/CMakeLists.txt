file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)

add_executable(blaadpaperscli ${SOURCES} ${HEADERS})

target_link_libraries(blaadpaperscli PRIVATE shared)
target_include_directories(blaadpaperscli PRIVATE include)

if(WIN32)
    set(BINARY_NAME blaadpaperscli)
    set(FILE_DESCRIPTION "${PROJECT_NAME} CLI")
    configure_file(${PROJECT_SOURCE_DIR}/shared/resource/windows/blaadpapers.rc.in blaadpaperscli.rc @ONLY)
    target_sources(blaadpaperscli PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/blaadpaperscli.rc)
endif()
