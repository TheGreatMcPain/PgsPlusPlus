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

include(GenerateExportHeader)

set(PGS++_HEADERS
        PgsUtil.hpp SegmentData.hpp PresentationComposition.hpp
        WindowDefinition.hpp PaletteDefinition.hpp Segment.hpp
        ObjectDefinition.hpp Subtitle.hpp)

add_library(pgs++
        PgsUtil.cpp SegmentData.cpp PresentationComposition.cpp
        WindowDefinition.cpp PaletteDefinition.cpp Segment.cpp
        ObjectDefinition.cpp Subtitle.cpp)

generate_export_header(pgs++)

set_property(TARGET pgs++ PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET pgs++ PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET pgs++ PROPERTY INTERFACE_pgs++_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET pgs++ APPEND PROPERTY COMPATIBLE_INTERFACE_STRING pgs++_MAJOR_VERSION)

target_include_directories(pgs++ INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include/pgs++>"
        )

target_compile_features(pgs++ PUBLIC
        cxx_auto_type
        cxx_constexpr
        cxx_noexcept
        cxx_nullptr
        cxx_range_for
        )

# Set up install instructions

install(TARGETS pgs++ EXPORT pgs++Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT core
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT core
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pgs++)

install(FILES ${PGS++_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pgs++ COMPONENT headers)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pgs++_export.h"
        DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/pgs++
        COMPONENT
        headers
)
