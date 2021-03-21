
add_executable(tcp_ping_pong tcp_ping_pong.cpp)
target_link_libraries(tcp_ping_pong simio pthread)

add_executable(tcp_discard tcp_discard.cpp)
target_link_libraries(tcp_discard simio pthread)

add_executable(tcp_echo tcp_echo.cpp)
target_link_libraries(tcp_echo simio pthread)
