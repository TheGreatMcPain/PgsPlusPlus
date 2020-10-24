cmake_minimum_required(VERSION 3.17)
project(libpgs CXX)

set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)
set(CMAKE_PROJECT_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(./test)
endif()