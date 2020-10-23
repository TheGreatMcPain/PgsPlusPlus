cmake_minimum_required(VERSION 3.17)
project(libpgs CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(pgs SHARED
        PresentationComposition.cpp PresentationComposition.hpp)

#include(CTest)
#if(BUILD_TESTING)
#    add_subdirectory(test)
#endif()
