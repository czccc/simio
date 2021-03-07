//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_UNIX_SOCKET_H_
#define SIMIO_INCLUDE_SIMIO_UNIX_SOCKET_H_

#include <sys/epoll.h>
#include <vector>
#include <atomic>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

#include "simio/base/token.h"
#include "simio/base/interest.h"
#include "simio/base/socket_addr.h"

namespace simio {
namespace sys {

int read(int fd, void *buf, size_t count);
int read_vectored(int fd, struct iovec *iov, int iovcnt);
int write(int fd, const void *buf, size_t count);
int write_vectored(int fd, const struct iovec *iov, int iovcnt);

using TcpSocket = int;

// int peek(int fd, void *buf, size_t count);
// int recv(int fd, void *buf, size_t count);
// int peek_from(int fd, void *buf, size_t count, SocketAddr &addr);
// int recv_from(int fd, void *buf, size_t count, SocketAddr &addr);
// int flush(int fd);

int send(TcpSocket socket, const std::vector<char> &buf, int flag = 0);
int recv(TcpSocket socket, std::vector<char> &buf, int flag = 0);
int send(TcpSocket socket, const std::string &buf, int flag = 0);
int recv(TcpSocket socket, std::string &buf, int flag = 0);

TcpSocket new_v4_socket();
TcpSocket new_v6_socket();
bool bind(TcpSocket socket, const SocketAddr &addr);
bool connect(TcpSocket socket, const SocketAddr &addr);
std::pair<TcpSocket, SocketAddr> accept(TcpSocket listener);
bool listen(TcpSocket socket, int backlog);
bool close(TcpSocket socket);
bool set_reuseaddr(TcpSocket socket, bool reuseaddr);
bool get_reuseaddr(TcpSocket socket);
bool set_reuse_port(TcpSocket socket, bool reuseport);
bool get_reuse_port(TcpSocket socket);
SocketAddr get_local_addr(TcpSocket socket);
SocketAddr get_peer_addr(TcpSocket socket);
bool set_linger(TcpSocket socket, int dur);
int get_linger(TcpSocket socket);
bool set_recv_buffer_size(TcpSocket socket, int size);
int get_recv_buffer_size(TcpSocket socket);
bool set_send_buffer_size(TcpSocket socket, int size);
int get_send_buffer_size(TcpSocket socket);

bool shutdown(TcpSocket socket, bool read, bool write);
bool set_nodelay(TcpSocket socket, bool nodelay);
bool get_nodelay(TcpSocket socket);
bool set_nonblocking(TcpSocket socket, bool nonblocking);
bool set_ttl(TcpSocket socket, int ttl);
int get_ttl(TcpSocket socket);

struct TcpKeepalive {
    int time;
    int interval;
    int retries;
};

bool set_keepalive(TcpSocket socket, bool keepalive);
bool get_keepalive(TcpSocket socket);
int set_keepalive_params(TcpSocket socket, TcpKeepalive keepalive);
bool set_keepalive_time(TcpSocket socket, int time);
int get_keepalive_time(TcpSocket socket);
bool set_keepalive_interval(TcpSocket socket, int interval);
int get_keepalive_interval(TcpSocket socket);
bool set_keepalive_retries(TcpSocket socket, int retries);
int get_keepalive_retries(TcpSocket socket);

}
}

#endif //SIMIO_INCLUDE_SIMIO_UNIX_SOCKET_H_
