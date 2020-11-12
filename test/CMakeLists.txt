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

file(COPY res/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res/)

find_package(GTest REQUIRED)

# PGS Test

add_executable(pgs-test PgsTest.cpp)

target_include_directories(pgs-test PUBLIC
        ../
        )

target_include_directories(pgs-test PRIVATE SYSTEM
        ${GTest_INCLUDE_DIRS}
        )

target_link_libraries(pgs-test PRIVATE
        pgs
        GTest::GTest
        GTest::Main
        )

add_test(NAME "PgsTest" COMMAND pgs-test WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR}/)

# Subtitle Test

find_package(OpenMP)
if(OPENMP_FOUND)
    message("OpenMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
include_directories(SYSTEM ${OpenMP_INCLUDE_PATH})

find_package(PkgConfig REQUIRED)

pkg_check_modules(magick REQUIRED GraphicsMagick++)

add_executable(subtitle-test SubtitleTest.cpp)

target_include_directories(subtitle-test PUBLIC
        ../
        )

target_include_directories(subtitle-test PRIVATE SYSTEM
        ${GTest_INCLUDE_DIRS}
        ${magick_INCLUDE_DIRS}
        )

target_link_libraries(subtitle-test PRIVATE
        pgs
        GTest::GTest
        GTest::Main
        ${magick_LIBRARIES}
        ${OpenMP_LIBRARIES}
        )

target_compile_features(subtitle-test PUBLIC
        cxx_std_17
        cxx_auto_type
        )

target_compile_options(subtitle-test PRIVATE
        ${OpenMP_CXX_FLAGS})

add_test(NAME "SubtitleTest" COMMAND subtitle-test WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR}/)
