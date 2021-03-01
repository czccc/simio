//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_NET_H
#define SIMIO_NET_H

#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>

int new_socket(int domain, uint32_t socket_type) {
    socket_type = socket_type | SOCK_NONBLOCK | SOCK_CLOEXEC;
    int socket_raw = socket(domain, socket_type, 0);
    return socket_raw;
}

int new_ip_socket(sockaddr addr, uint32_t socket_type) {
    // TODO: IPv6?
    return new_socket(addr.sa_family, socket_type);
}

#endif //SIMIO_NET_H
