//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_SYS_UNIX_TCP_H_
#define SIMIO_SYS_UNIX_TCP_H_

#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "net.h"

namespace simio {
namespace sys {
namespace tcp {

int KEEPALIVE_TIME = TCP_KEEPIDLE;
using TcpSocket = int;

TcpSocket new_v4_socket() {
    return new_socket(AF_INET, SOCK_STREAM);
}
TcpSocket new_v6_socket() {
    return new_socket(AF_INET6, SOCK_STREAM);
}
int bind(TcpSocket socket, const sockaddr *addr) {
    return bind(socket, addr, sizeof(*addr));
}

int connect(TcpSocket socket, sockaddr *addr) {
    return connect(socket, addr, sizeof(*addr));
}

int listen(TcpSocket socket, int backlog) {

}

}
}
}

#endif //SIMIO_SYS_UNIX_TCP_H_
