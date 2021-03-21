aux_source_directory(./unix SYS_UNIX_SRCS)
aux_source_directory(./net NET_SRCS)

#message("${SYS_UNIX_SRCS}")

add_library(simio ${SYS_UNIX_SRCS} ${NET_SRCS} ../include/simio/reactor/worker.h ../include/simio/reactor/task.h ../include/simio/reactor/acceptor.h ../include/simio/base/channel.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})
