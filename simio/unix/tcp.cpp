//
// Created by Cheng on 2021/2/28.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sstream>
#include <cstring>
#include <unistd.h>

#include "simio.h"

using simio::sys::TcpSocket;

TcpSocket new_socket(int domain, uint32_t socket_type) {
    socket_type = socket_type | SOCK_NONBLOCK | SOCK_CLOEXEC;
    int socket_raw = ::socket(domain, socket_type, 0);
    if (socket_raw < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in create new socket with domain:" << domain << " type: " << socket_type
                     << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    // std::cout << "open socket: " << socket_raw << std::endl;
    return socket_raw;
}

int new_ip_socket(sockaddr addr, uint32_t socket_type) {
    return new_socket(addr.sa_family, socket_type);
}

TcpSocket simio::sys::new_v4_socket() {
    return new_socket(AF_INET, SOCK_STREAM);
}
TcpSocket simio::sys::new_v6_socket() {
    return new_socket(AF_INET6, SOCK_STREAM);
}
bool simio::sys::bind(TcpSocket socket, const SocketAddr &addr) {
    int ret = ::bind(socket, addr.get(), sizeof(*addr.get()));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " bind to " << addr << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}

bool simio::sys::connect(TcpSocket socket, const SocketAddr &addr) {
    int ret = ::connect(socket, addr.get(), sizeof(*addr.get()));
    if (ret < 0 && errno != EINPROGRESS) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " connect to " << addr << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}

std::pair<TcpSocket, simio::SocketAddr> simio::sys::accept(TcpSocket listener) {
    sockaddr addr{};
    socklen_t addr_size = sizeof(addr);
    int ret_socket =
        ::accept4(listener, &addr, &addr_size, SOCK_CLOEXEC | SOCK_NONBLOCK);
    if (ret_socket < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << listener << " accept. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    SocketAddr ret_addr(addr);
    return std::make_pair(ret_socket, ret_addr);
}
bool simio::sys::listen(TcpSocket socket, int backlog) {
    int ret = ::listen(socket, backlog);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set to listen. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;

}

bool simio::sys::close(TcpSocket socket) {

    int ret = ::close(socket);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " close. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}

int simio::sys::send(TcpSocket socket, const std::vector<char> &buf, int flag) {
    int ret = ::send(socket, buf.data(), buf.size(), flag);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set to listen. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return ret;
}
int simio::sys::recv(TcpSocket socket, std::vector<char> &buf, int flag) {
    int ret = ::recv(socket, buf.data(), buf.capacity(), flag);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set to listen. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return ret;
}

int simio::sys::send(TcpSocket socket, const std::string &buf, int flag) {
    int ret = ::send(socket, buf.data(), buf.size(), flag);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set to listen. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return ret;
}
int simio::sys::recv(TcpSocket socket, std::string &buf, int flag) {
    int ret = ::recv(socket, (void *) buf.c_str(), buf.capacity(), flag);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set to listen. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return ret;
}

bool simio::sys::set_reuseaddr(TcpSocket socket, bool reuseaddr) {
    int optval = reuseaddr ? 1 : 0;
    socklen_t optlen = sizeof(optval);
    int ret = ::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set reuse addr " << reuseaddr << ". " << "Returned errno "
                     << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
bool simio::sys::get_reuseaddr(TcpSocket socket) {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    int ret = ::getsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get reuse addr" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return optval != 0;
}
bool simio::sys::set_reuse_port(TcpSocket socket, bool reuseport) {
    int optval = reuseport ? 1 : 0;
    socklen_t optlen = sizeof(optval);
    int ret = ::setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &optval, optlen);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set reuse port " << reuseport << ". " << "Returned errno "
                     << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
bool simio::sys::get_reuse_port(TcpSocket socket) {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    int ret = ::getsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &optval, &optlen);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get reuse port" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return optval != 0;
}
simio::SocketAddr simio::sys::get_local_addr(TcpSocket socket) {
    sockaddr addr{};
    unsigned int length = sizeof(addr);
    int ret = ::getsockname(socket, &addr, &length);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get reuse port" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return SocketAddr(addr);
}
simio::SocketAddr simio::sys::get_peer_addr(TcpSocket socket) {
    sockaddr addr{};
    unsigned int length = sizeof(addr);
    int ret = ::getpeername(socket, &addr, &length);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get peer addr" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return SocketAddr(addr);
}
bool simio::sys::set_linger(TcpSocket socket, int dur) {
    linger val{};
    val.l_onoff = dur > 0 ? 1 : 0;
    val.l_linger = dur;
    int ret = ::setsockopt(socket, SOL_SOCKET, SO_LINGER, &val, sizeof(val));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set linger " << dur << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
int simio::sys::get_linger(TcpSocket socket) {
    linger val{};
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, SOL_SOCKET, SO_LINGER, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get linger " << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    if (val.l_onoff == 0) {
        return 0;
    } else {
        return val.l_linger;
    }
}
bool simio::sys::set_recv_buffer_size(TcpSocket socket, int size) {
    int ret = ::setsockopt(socket, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set recv buffer size " << size << ". " << "Returned errno "
                     << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
int simio::sys::get_recv_buffer_size(TcpSocket socket) {
    int val;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, SOL_SOCKET, SO_RCVBUF, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get recv buffer size " << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val;
}
bool simio::sys::set_send_buffer_size(TcpSocket socket, int size) {
    int ret = ::setsockopt(socket, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set send buffer size " << size << ". "
                     << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
int simio::sys::get_send_buffer_size(TcpSocket socket) {
    int val;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, SOL_SOCKET, SO_SNDBUF, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get send buffer size " << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val;
}

bool simio::sys::shutdown(TcpSocket socket, bool read, bool write) {
    std::cout << "shutdown socket: " << socket << std::endl;
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
    int ret = ::shutdown(socket, val);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " shutdown read: " << read << " write: " << write << ". "
                     << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}

bool simio::sys::set_nodelay(TcpSocket socket, bool nodelay) {
    int val = nodelay ? 1 : 0;
    int ret = ::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set nodelay " << nodelay << ". " << "Returned errno "
                     << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
bool simio::sys::get_nodelay(TcpSocket socket) {
    int val;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get nonblocking" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val;
}
bool simio::sys::set_nonblocking(TcpSocket socket, bool nonblocking) {
    int val = nonblocking ? 1 : 0;
    int ret = ::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set nonblocking " << nonblocking << ". "
                     << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}

bool simio::sys::set_ttl(TcpSocket socket, int ttl) {
    int ret = ::setsockopt(socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set ttl " << ttl << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
int simio::sys::get_ttl(TcpSocket socket) {
    int val;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, IPPROTO_IP, IP_TTL, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get ttl" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val;
}

bool simio::sys::set_keepalive(TcpSocket socket, bool keepalive) {
    int val = keepalive ? 1 : 0;
    int ret = ::setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set keepalive " << keepalive << ". " << "Returned errno "
                     << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
bool simio::sys::get_keepalive(TcpSocket socket) {
    int val;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get keepalive" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val != 0;
}
int simio::sys::set_keepalive_params(TcpSocket socket, TcpKeepalive keepalive) {
    return set_keepalive_time(socket, keepalive.time) &&
        set_keepalive_interval(socket, keepalive.interval) &&
        set_keepalive_retries(socket, keepalive.retries);
}
bool simio::sys::set_keepalive_time(TcpSocket socket, int time) {
    int ret = ::setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, &time, sizeof(time));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set keepalive time " << time << ". " << "Returned errno "
                     << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
int simio::sys::get_keepalive_time(TcpSocket socket) {
    if (!get_keepalive(socket)) {
        return 0;
    }

    int val = 0;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get keepalive time" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val;
}
bool simio::sys::set_keepalive_interval(TcpSocket socket, int interval) {
    int ret = ::setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set keepalive interval " << interval << ". "
                     << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;
}
int simio::sys::get_keepalive_interval(TcpSocket socket) {
    if (!get_keepalive(socket)) {
        return 0;
    }

    int val = 0;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get keepalive interval" << ". " << "Returned errno "
                     << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val;
}
bool simio::sys::set_keepalive_retries(TcpSocket socket, int retries) {
    int ret = ::setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &retries, sizeof(retries));
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " set keepalive retries " << retries << ". "
                     << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return true;

}
int simio::sys::get_keepalive_retries(TcpSocket socket) {
    if (!get_keepalive(socket)) {
        return 0;
    }

    int val = 0;
    unsigned int len = sizeof(val);
    int ret = ::getsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &val, &len);
    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in socket: " << socket << " get keepalive retries" << ". " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    return val;
}