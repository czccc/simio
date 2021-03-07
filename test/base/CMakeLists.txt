
add_executable(simio_base_test test_interest.cpp test_waker_event.cpp)
target_link_libraries(simio_base_test gtest gtest_main simio pthread)
add_test(NAME simio_base_test COMMAND ${CMAKE_BINARY_DIR}/simio_base_test)
