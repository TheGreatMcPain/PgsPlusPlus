file(COPY res/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res/)

add_executable(pgs-test pgs_test.c)

target_include_directories(pgs-test PUBLIC
        ../
        )


target_link_libraries(pgs-test PRIVATE
        pgs
        )

add_test(NAME "PGS_Test" COMMAND pgs-test WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR}/)
