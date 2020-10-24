cmake_minimum_required(VERSION 3.17)
project(libpgs CXX)

set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)
set(CMAKE_PROJECT_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 14)

add_library(pgs-util STATIC
        pgs.cpp pgs.hpp
        )

target_compile_options(pgs-util BEFORE PUBLIC
        "-fPIC"
        )

add_library(pgs SHARED
        SegmentData.cpp SegmentData.hpp
        PresentationComposition.cpp PresentationComposition.hpp
        WindowDefinition.cpp WindowDefinition.hpp
        PaletteDefinition.cpp PaletteDefinition.hpp
        Segment.cpp Segment.hpp)

target_link_libraries(pgs
        pgs-util
        )

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
