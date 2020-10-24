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
        Segment.cpp Segment.hpp ObjectDefinition.cpp ObjectDefinition.hpp)

target_link_libraries(pgs
        pgs-util
        )


