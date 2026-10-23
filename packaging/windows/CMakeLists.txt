find_package(Qt6 REQUIRED COMPONENTS Qml)

set(SPDX_LICENSES_DIR "" CACHE PATH "Directory with SPDX license texts")

install(TARGETS blaadpapers blaadpaperscli RUNTIME_DEPENDENCY_SET DEPENDENCIES RUNTIME DESTINATION .)
install(RUNTIME_DEPENDENCY_SET DEPENDENCIES
    DIRECTORIES ${QT6_INSTALL_PREFIX}/bin
    PRE_EXCLUDE_REGEXES "^api-ms-" "^ext-ms-"
    POST_EXCLUDE_REGEXES "[Ss]ystem32"
    RUNTIME DESTINATION .
)

qt_generate_deploy_qml_app_script(TARGET blaadpapers OUTPUT_SCRIPT QT_DEPLOY_SCRIPT NO_TRANSLATIONS)
install(CODE "
    set(QT_DEPLOY_BIN_DIR .)
    set(QT_DEPLOY_LIB_DIR .)
    set(QT_DEPLOY_PLUGINS_DIR plugins)
    set(QT_DEPLOY_QML_DIR qml)
")
install(SCRIPT ${QT_DEPLOY_SCRIPT})

install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION . RENAME LICENSE.txt)
install(CODE "
    set(MINGW_PREFIX \"${QT6_INSTALL_PREFIX}\")
    set(QT_PLUGINS_DIR \"${QT6_INSTALL_PREFIX}/${QT6_INSTALL_PLUGINS}\")
    set(QT_QML_DIR \"${QT6_INSTALL_PREFIX}/${QT6_INSTALL_QML}\")
    set(OBJDUMP \"${CMAKE_OBJDUMP}\")
    set(SPDX_LICENSES_DIR \"${SPDX_LICENSES_DIR}\")
")
install(SCRIPT Deploy.cmake)

set(CPACK_GENERATOR NSIS ZIP)
set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR Blaadick)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION}-windows-x86_64-setup)
set(CPACK_ARCHIVE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION}-windows-x86_64-portable)
set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CPackOptions.cmake)
set(CPACK_PACKAGE_EXECUTABLES blaadpapers ${PROJECT_NAME})
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)

set(CPACK_NSIS_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_MUI_ICON ${PROJECT_SOURCE_DIR}/shared/resource/windows/blaadpapers.ico)
set(CPACK_NSIS_MUI_UNIICON ${PROJECT_SOURCE_DIR}/shared/resource/windows/blaadpapers.ico)
set(CPACK_NSIS_INSTALLED_ICON_NAME blaadpapers.exe)
set(CPACK_NSIS_URL_INFO_ABOUT https://github.com/Blaadick/BlaadPapers)
set(CPACK_NSIS_MUI_FINISHPAGE_RUN blaadpapers.exe)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_EXECUTABLES_DIRECTORY .)
set(CPACK_NSIS_MANIFEST_DPI_AWARE ON)
set(CPACK_NSIS_COMPRESSOR "/SOLID lzma")
set(CPACK_NSIS_DEFINES "
VIProductVersion ${PROJECT_VERSION}.0
VIFileVersion ${PROJECT_VERSION}.0
VIAddVersionKey Comments \"${PROJECT_DESCRIPTION}\"
VIAddVersionKey CompanyName \"${CPACK_PACKAGE_VENDOR}\"
VIAddVersionKey FileDescription \"${PROJECT_NAME} Setup\"
VIAddVersionKey FileVersion ${PROJECT_VERSION}
VIAddVersionKey InternalName ${CPACK_PACKAGE_FILE_NAME}
VIAddVersionKey LegalCopyright \"Copyright (C) 2026 ${CPACK_PACKAGE_VENDOR}\"
VIAddVersionKey OriginalFilename ${CPACK_PACKAGE_FILE_NAME}.exe
VIAddVersionKey ProductName ${PROJECT_NAME}
VIAddVersionKey ProductVersion ${PROJECT_VERSION}
")
set(CPACK_NSIS_CREATE_ICONS_EXTRA [[
CreateShortCut "$DESKTOP\BlaadPapers.lnk" "$INSTDIR\blaadpapers.exe"
]])
set(CPACK_NSIS_DELETE_ICONS_EXTRA [[
Delete "$DESKTOP\BlaadPapers.lnk"
]])
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS [[
WriteRegStr HKLM "Software\Classes\blaadpapers" "" "URL:BlaadPapers Protocol"
WriteRegStr HKLM "Software\Classes\blaadpapers" "URL Protocol" ""
WriteRegStr HKLM "Software\Classes\blaadpapers\DefaultIcon" "" "$INSTDIR\blaadpapers.exe,0"
WriteRegStr HKLM "Software\Classes\blaadpapers\shell\open\command" "" '"$INSTDIR\blaadpaperscli.exe" "%1"'
]])
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS [[
DeleteRegKey HKLM "Software\Classes\blaadpapers"
]])

include(CPack)
