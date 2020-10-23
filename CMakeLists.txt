cmake_minimum_required(VERSION 3.17)
project(libPGS C)

set(CMAKE_C_STANDARD 11)

add_library(pgs SHARED pgs.c pgs.h)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
