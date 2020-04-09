###################################################################################
#                                                                                 #
# NAME: CMakeLists.txt                                                            #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################

cmake_minimum_required(VERSION 3.12)
project(unity
    VERSION     2.5.0
    LANGUAGES   C
    DESCRIPTION "C Unit testing framework."
)

# Main target ------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::framework ALIAS ${PROJECT_NAME})

# Includes ---------------------------------------------------------------------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Configuration ----------------------------------------------------------------
configure_file(cmake/templates/${PROJECT_NAME}_version.h.in 
               ${PROJECT_NAME}_version.h 
               @ONLY
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/unity.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

set(${PROJECT_NAME}_PUBLIC_HEADERS src/unity.h
                                   src/unity_internals.h
                                   ${CMAKE_CURRENT_BINARY_DIR}/unity_version.h
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES 
        C_STANDARD          11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS        OFF
        PUBLIC_HEADER       "${${PROJECT_NAME}_PUBLIC_HEADERS}"
        EXPORT_NAME         framework
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        $<$<C_COMPILER_ID:Clang>:-Wcast-align>
        $<$<C_COMPILER_ID:Clang>:-Wcast-qual>
        $<$<C_COMPILER_ID:Clang>:-Wconversion>
        $<$<C_COMPILER_ID:Clang>:-Wexit-time-destructors>
        $<$<C_COMPILER_ID:Clang>:-Wglobal-constructors>
        $<$<C_COMPILER_ID:Clang>:-Wmissing-noreturn>
        $<$<C_COMPILER_ID:Clang>:-Wmissing-prototypes>
        $<$<C_COMPILER_ID:Clang>:-Wno-missing-braces>
        $<$<C_COMPILER_ID:Clang>:-Wold-style-cast>
        $<$<C_COMPILER_ID:Clang>:-Wshadow>
        $<$<C_COMPILER_ID:Clang>:-Wweak-vtables>
        $<$<C_COMPILER_ID:GNU>:-Waddress>
        $<$<C_COMPILER_ID:GNU>:-Waggregate-return>
        $<$<C_COMPILER_ID:GNU>:-Wformat-nonliteral>
        $<$<C_COMPILER_ID:GNU>:-Wformat-security>
        $<$<C_COMPILER_ID:GNU>:-Wformat>
        $<$<C_COMPILER_ID:GNU>:-Winit-self>
        $<$<C_COMPILER_ID:GNU>:-Wmissing-declarations>
        $<$<C_COMPILER_ID:GNU>:-Wmissing-include-dirs>
        $<$<C_COMPILER_ID:GNU>:-Wno-multichar>
        $<$<C_COMPILER_ID:GNU>:-Wno-parentheses>
        $<$<C_COMPILER_ID:GNU>:-Wno-type-limits>
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:GNU>:-Wunreachable-code>
        $<$<C_COMPILER_ID:GNU>:-Wwrite-strings>
        -Wpointer-arith
        -Wall
        -Werror
)

write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
    VERSION       ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion 
)

## Target installation
install(TARGETS   ${PROJECT_NAME}
    EXPORT        ${PROJECT_NAME}Targets
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT     library
)

## Target's cmake files: targets export
install(EXPORT  ${PROJECT_NAME}Targets
    NAMESPACE   ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

## Target's cmake files: config and version config for find_package()
install(FILES   ${PROJECT_NAME}Config.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
