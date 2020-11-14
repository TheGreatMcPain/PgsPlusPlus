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

add_library(pgs
        pgs.cpp pgs.hpp
        SegmentData.cpp SegmentData.hpp
        PresentationComposition.cpp PresentationComposition.hpp
        WindowDefinition.cpp WindowDefinition.hpp
        PaletteDefinition.cpp PaletteDefinition.hpp
        Segment.cpp Segment.hpp
        ObjectDefinition.cpp ObjectDefinition.hpp
        Subtitle.cpp Subtitle.hpp)

target_compile_features(pgs PUBLIC
        cxx_auto_type
        cxx_constexpr
        cxx_noexcept
        cxx_nullptr
        cxx_range_for
        )

# Set up install instructions

include(GNUInstallDirs)

if (${BUILD_SHARED_LIBS} == on)
        install(TARGETS pgs LIBRARY)
else()
        install(TARGETS pgs ARCHIVE)
endif()
