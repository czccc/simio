//
// Created by Cheng on 2021/2/28.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "simio.h"

using simio::sys::TcpSocket;

// using namespace simio;

// int KEEPALIVE_TIME = TCP_KEEPIDLE;


TcpSocket new_socket(int domain, uint32_t socket_type) {
    socket_type = socket_type | SOCK_NONBLOCK | SOCK_CLOEXEC;
    int socket_raw = ::socket(domain, socket_type, 0);
    return socket_raw;
}

int new_ip_socket(sockaddr addr, uint32_t socket_type) {
    // TODO: IPv6?
    return new_socket(addr.sa_family, socket_type);
}

TcpSocket simio::sys::new_v4_socket() {
    return new_socket(AF_INET, SOCK_STREAM);
}
TcpSocket simio::sys::new_v6_socket() {
    return new_socket(AF_INET6, SOCK_STREAM);
}
int simio::sys::bind(TcpSocket socket, const sockaddr *addr) {
    return ::bind(socket, addr, sizeof(*addr));
}

int simio::sys::connect(TcpSocket socket, const sockaddr *addr) {
    return ::connect(socket, addr, sizeof(*addr));
}

std::pair<TcpSocket, const sockaddr *> simio::sys::accept(TcpSocket listener) {

}
TcpSocket simio::sys::listen(TcpSocket socket, int backlog) {
    ::listen(socket, backlog);
    return socket;
}
int simio::sys::close(TcpSocket socket) {
    // TODO: close
    return 0;
}
bool simio::sys::set_reuseaddr(TcpSocket socket, bool reuseaddr) {
    int optval = reuseaddr ? 1 : 0;
    socklen_t optlen = sizeof(optval);
    return ::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) == 0;
}
bool simio::sys::get_reuseaddr(TcpSocket socket) {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    ::getsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen);
    return optval != 0;
}
bool simio::sys::set_reuse_port(TcpSocket socket, bool reuseport) {
    int optval = reuseport ? 1 : 0;
    socklen_t optlen = sizeof(optval);
    return ::setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &optval, optlen) == 0;
}
bool simio::sys::get_reuse_port(TcpSocket socket) {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    ::getsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &optval, &optlen);
    return optval != 0;
}
simio::SocketAddr simio::sys::get_local_addr(TcpSocket socket) {
    sockaddr *addr{};
    unsigned int length;
    ::getsockname(socket, addr, &length);
    if (addr->sa_family == AF_INET) {
        auto *sin = reinterpret_cast<sockaddr_in *>(addr);
        return SocketAddr(*sin);
    } else if (addr->sa_family == AF_INET6) {
        auto *sin = reinterpret_cast<sockaddr_in6 *>(addr);
        return SocketAddr(*sin);
    }
}
simio::SocketAddr simio::sys::get_peer_addr(TcpSocket socket) {
    sockaddr *addr{};
    unsigned int length;
    ::getpeername(socket, addr, &length);
    if (addr->sa_family == AF_INET) {
        auto *sin = reinterpret_cast<sockaddr_in *>(addr);
        return SocketAddr(*sin);
    } else if (addr->sa_family == AF_INET6) {
        auto *sin = reinterpret_cast<sockaddr_in6 *>(addr);
        return SocketAddr(*sin);
    }
}
bool simio::sys::set_linger(TcpSocket socket, int dur) {
    linger val{};
    val.l_onoff = dur > 0 ? 1 : 0;
    val.l_linger = dur;
    return ::setsockopt(socket, SOL_SOCKET, SO_LINGER, &val, sizeof(val)) == 0;
}
int simio::sys::get_linger(TcpSocket socket) {
    linger val{};
    unsigned int len;
    ::getsockopt(socket, SOL_SOCKET, SO_LINGER, &val, &len);
    if (val.l_onoff == 0) {
        return 0;
    } else {
        return val.l_linger;
    }
}
bool simio::sys::set_recv_buffer_size(TcpSocket socket, int size) {
    return ::setsockopt(socket, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) == 0;
}
int simio::sys::get_recv_buffer_size(TcpSocket socket) {
    int val;
    unsigned int len;
    ::getsockopt(socket, SOL_SOCKET, SO_RCVBUF, &val, &len);
    return val;
}
bool simio::sys::set_send_buffer_size(TcpSocket socket, int size) {
    return ::setsockopt(socket, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) == 0;
}
int simio::sys::get_send_buffer_size(TcpSocket socket) {
    int val;
    unsigned int len;
    ::getsockopt(socket, SOL_SOCKET, SO_SNDBUF, &val, &len);
    return val;
}

bool simio::sys::shutdown(TcpSocket socket, bool read, bool write) {
    int val;
    if (read && write) {
        val = SHUT_RDWR;
    } else if (read) {
        val = SHUT_RD;
    } else if (write) {
        val = SHUT_WR;
    } else {
        return false;
    }
    return ::shutdown(socket, val) == 0;
}

bool simio::sys::set_nodelay(TcpSocket socket, bool nodelay) {
    int val = nodelay ? 1 : 0;
    return ::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == 0;
}
bool simio::sys::get_nodelay(TcpSocket socket) {
    int val;
    unsigned int len;
    ::getsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &val, &len);
    return val;
}
bool simio::sys::set_nonblocking(TcpSocket socket, bool nonblocking) {
    int val = nonblocking ? 1 : 0;
    return ::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == 0;
}

bool simio::sys::set_ttl(TcpSocket socket, int ttl) {
    return ::setsockopt(socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == 0;
}
int simio::sys::get_ttl(TcpSocket socket) {
    int val;
    unsigned int len;
    ::getsockopt(socket, IPPROTO_IP, IP_TTL, &val, &len);
    return val;
}

bool simio::sys::set_keepalive(TcpSocket socket, bool keepalive) {
    int val = keepalive ? 1 : 0;
    return ::setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) == 0;
}
bool simio::sys::get_keepalive(TcpSocket socket) {
    int val;
    unsigned int len;
    ::getsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &val, &len);
    return val != 0;
}
int simio::sys::set_keepalive_params(TcpSocket socket, TcpKeepalive keepalive) {
    return set_keepalive_time(socket, keepalive.time) &&
        set_keepalive_interval(socket, keepalive.interval) &&
        set_keepalive_retries(socket, keepalive.retries);
}
bool simio::sys::set_keepalive_time(TcpSocket socket, int time) {
    return ::setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, &time, sizeof(time)) == 0;
}
int simio::sys::get_keepalive_time(TcpSocket socket) {
    if (!get_keepalive(socket)) {
        return 0;
    }

    int val;
    unsigned int len;
    ::getsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, &val, &len);
    return val;
}
bool simio::sys::set_keepalive_interval(TcpSocket socket, int interval) {
    return ::setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval)) == 0;
}
int simio::sys::get_keepalive_interval(TcpSocket socket) {
    if (!get_keepalive(socket)) {
        return 0;
    }

    int val;
    unsigned int len;
    ::getsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &val, &len);
    return val;
}
bool simio::sys::set_keepalive_retries(TcpSocket socket, int retries) {
    return ::setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &retries, sizeof(retries)) == 0;

}
int simio::sys::get_keepalive_retries(TcpSocket socket) {
    if (!get_keepalive(socket)) {
        return 0;
    }

    int val;
    unsigned int len;
    ::getsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &val, &len);
    return val;
}