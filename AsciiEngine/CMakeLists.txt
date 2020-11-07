# CMakeList.txt : CMake project for AsciiEngine, include source and define
# project specific logic here.
#
#https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/


cmake_minimum_required (VERSION 3.8)
project (libAsciiEngine VERSION 1.0.0 LANGUAGES CXX)


add_library(asciiengine
    src/AsciiRender.cpp
)

add_library(AsciiEngine::asciiengine ALIAS asciiengine)

target_include_directories(asciiengine
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#target_compile_options(AsciiEngine PRIVATE)
target_compile_features(asciiengine PRIVATE cxx_std_17)

include(GNUInstallDirs)

install(TARGETS asciiengine
    EXPORT asciiengine-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(asciiengine PROPERTIES EXPORT_NAME AsciiEngine)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT asciiengine-targets
    FILE
        AsciiEngineTargets.cmake
    NAMESPACE
        AsciiEngine::
    DESTINATION
        ${INSTALL_CONFIGDIR}/cmake/AsciiEngine
)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/AsciiEngine)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/AsciiEngineConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/AsciiEngineConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AsciiEngineConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AsciiEngineConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)
export(EXPORT asciiengine-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/AsciiEngineTargets.cmake
    NAMESPACE AsciiEngine::)

export(PACKAGE AsciiEngine)
