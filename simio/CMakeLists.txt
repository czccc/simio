aux_source_directory(./sys/unix SYS_UNIX_SRCS)
aux_source_directory(./net NET_SRCS)

#message("${SYS_UNIX_SRCS}")

add_library(simio ${SYS_UNIX_SRCS} ${NET_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})
