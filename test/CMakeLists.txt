# add google test
add_subdirectory(googletest)
include_directories(googletest/googletest/include googletest/googletest/src)
include_directories(googletest/googlemock/include googletest/googlemock/src)

# base
add_subdirectory(base)


# for test all
aux_source_directory(./base TEST_SIMIO_BASE_SRCS)

add_executable(simio_test_all ${TEST_SIMIO_BASE_SRCS})
target_link_libraries(simio_test_all gtest gtest_main simio pthread)
add_test(NAME simio_test_all COMMAND ${CMAKE_BINARY_DIR}/simio_test_all)
