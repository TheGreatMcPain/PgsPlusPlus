add_custom_target(include-dir SOURCES pgs.hpp)

if(${EXPORT_HEADERS})
    install(FILES pgs.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pgs++ COMPONENT headers)
endif()
