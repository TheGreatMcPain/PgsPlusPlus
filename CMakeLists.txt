#  libpgs: A C++ library for reading Presentation Graphics Stream (PGS) subtitles.
#  Copyright (C) 2020  Brenden Davidson
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
# 
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
#  USA

cmake_minimum_required(VERSION 3.15)
project(pgs++ LANGUAGES CXX VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PROJECT_DESCRIPTION "A C++ library for reading Presentation Graphics Stream (PGS) subtitle data.")

# Basic CPack setup
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Brenden Davidson")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/iAmSomeone2/PgsPlusPlus")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)

# RPM specific settings
set(CPACK_RPM_PACKAGE_RELEASE 1)
set(CPACK_RPM_COMPONENT_INSTALL ON) # Allow splitting lib from headers
set(CPACK_RPM_PACKAGE_RELEASE_DIST ON)
set(CPACK_RPM_PACKAGE_SUMMARY ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_RPM_PACKAGE_LICENSE "LGPLv2.1")
set(CPACK_RPM_PACKAGE_VENDOR ${CPACK_PACKAGE_VENDOR})
set(CPACK_RPM_PACKAGE_URL ${CPACK_PACKAGE_HOMEPAGE_URL})
set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md")

# CPack RPM grouping settings
set(CPACK_RPM_MAIN_COMPONENT "BASE")
set(CPACK_RPM_BASE_PACKAGE_NAME "pgs++")
set(CPACK_RPM_DEVEL_PACKAGE_NAME "pgs++-devel")
set(CPACK_RPM_BASE_FILE_NAME RPM-DEFAULT)
set(CPACK_RPM_DEVEL_FILE_NAME RPM-DEFAULT)

# Using Conan CMake generator
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

# Extra language level requirements are determined using 'target_compile_features'

# Set up parent project detection
set(HAS_PARENT False)
if(NOT ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(HAS_PARENT True)
endif()

option(BUILD_SHARED_LIBS "Build pgs++ as a shared library instead of static." ON)
option(EXPORT_HEADERS "Header files will be installed in addition to the library." ON)
option(BUILD_TESTING "Build package tests" OFF)

# Set options in the event Pgs++ is a subproject
if(${HAS_PARENT})
    set(BUILD_SHARED_LIBS off)
    set(EXPORT_HEADERS off)
    set(BUILD_TESTING off)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    include(GNUInstallDirs)
endif()

add_subdirectory(src)
include_directories(src)
add_subdirectory(include)

include(CTest)
if (BUILD_TESTING)
    add_subdirectory(./test)
endif ()

install(FILES "./AUTHORS" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT core)
install(FILES "./LICENSE" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT core)

# Set up package config docs

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/pgs++ConfigVersion.cmake"
        COMPATIBILITY SameMinorVersion
)

export(EXPORT pgs++Targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/pgs++Targets.cmake"
        NAMESPACE pgs++::
        )

set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/pgs++)

configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/pgs++Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/pgs++Config.cmake"
        INSTALL_DESTINATION ${ConfigPackageLocation}
        )

install(EXPORT pgs++Targets
        FILE
        pgs++Targets.cmake
        NAMESPACE
        pgs++::
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        cmake_config
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/pgs++Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/pgs++ConfigVersion.cmake"
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        cmake_config
)

# Set up packaging scripts
add_subdirectory(dist)

# Finalize CPack set up
include(CPack)

cpack_add_component(core
        DISPLAY NAME "Core library"
        DESCRIPTION "The core Pgs++ shared library"
        REQUIRED
        GROUP BASE)
cpack_add_component(headers
        DISPLAY NAME "Headers"
        DESCRIPTION "Header files needed for building programs using Pgs++"
        DISABLED
        DEPENDS CORE
        GROUP DEVEL)
cpack_add_component(cmake_config
        DISPLAY NAME "CMake configuration files"
        DESCRIPTION "Files used for importing Pgs++ with CMake"
        DEPENDS CORE
        GROUP DEVEL)

cpack_add_component_group(base
        DISPLAY NAME "Pgs++")
cpack_add_component_group(devel
        DISPLAY NAME "Pgs++ Development Files")