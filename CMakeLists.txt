cmake_minimum_required(VERSION 3.10)
project(simio)

set(CMAKE_CXX_STANDARD 14)

add_library(simio library.h sys/unix/selector/epoll.cpp sys/unix/selector/epoll.h interest.h token.h sys/unix/waker.cpp sys/unix/waker.h sys/unix/net.h sys/unix/pipe.h waker.h poll.h event/event.h event/event_list.h event/source.h sys/target_os.h sys/unix/source_fd.h sys/unix/tcp.h net/tcp/socket.h net/tcp/address.h)