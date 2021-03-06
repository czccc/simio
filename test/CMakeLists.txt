
add_executable(test_socket_addr test_socket_addr.cpp)

add_executable(test_tcp_socket test_tcp_socket.cpp)
target_link_libraries(test_tcp_socket simio pthread)

add_executable(test_unix_socket test_unix_socket.cpp)
target_link_libraries(test_unix_socket simio pthread)


add_executable(test_tcp_connect test_tcp_connect.cpp)
target_link_libraries(test_tcp_connect simio pthread)

#aux_source_directory(. TEST_CASE_CPP)
#message("${TEST_CASE_CPP}")
#foreach (test_case in TEST_CASE_CPP)
#    message("${test_case}")
#endforeach ()