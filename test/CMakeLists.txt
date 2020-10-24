file(COPY res/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res/)

find_package(GTest REQUIRED)


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
