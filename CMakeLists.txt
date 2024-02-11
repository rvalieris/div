cmake_minimum_required(VERSION 3.16)
project(div VERSION 0.1.8 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Svg Widgets SvgWidgets)

qt_standard_project_setup()

qt_add_executable(div WIN32 MACOSX_BUNDLE
    src/div.cpp
    src/imagewidget.cpp
    src/main.cpp
    src/qarchive.cpp
    src/thumbnailengine.cpp
    src/thumbnailmodel.cpp
    src/thumbnailview.cpp
)
target_compile_definitions(div PRIVATE
    APP_VERSION="0.1.9"
)

target_link_libraries(div PRIVATE
    Qt::Svg
    Qt::Widgets
    Qt::SvgWidgets
    archive
)


# Resources:
set(resources_resource_files
    "resources/folder.svg"
    "resources/muon.svg"
)

qt_add_resources(div "resources"
    PREFIX
        "/"
    BASE
        "resources"
    FILES
        ${resources_resource_files}
)

install(TARGETS div
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET div
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
