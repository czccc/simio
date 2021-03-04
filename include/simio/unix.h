//
// Created by Cheng on 2021/3/4.
//

#ifndef SIMIO_INCLUDE_SIMIO_UNIX_H_
#define SIMIO_INCLUDE_SIMIO_UNIX_H_

#include <sys/epoll.h>
#include <vector>
#include <atomic>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

#include "base.h"

namespace simio {

namespace sys {

class Selector;

class Waker {
  public:
    Waker(const Selector &s, Token token);
    ~Waker() = default;

    bool wake();
    bool reset() const;

  private:
    int fd;
};

using Event = epoll_event;
using EventList = std::vector<Event>;

class Selector {
  public:

    Selector();
    ~Selector();

    // Selector(Selector &rhs);
    // Selector &operator=(Selector const &rhs);

    // Selector(Selector &&rhs) noexcept;
    // Selector &operator=(Selector &&rhs) noexcept;

    int select(EventList events, int timeout) const;

    void event_register(int fd, Token token, const Interest &interest) const;

    void event_reregister(int fd, Token token, const Interest &interest) const;

    void event_deregister(int fd) const;

    bool register_waker();

    int get_id() const {
        return id;
    }

  private:
    static std::atomic<int> next_id;
    int id;
    int ep;
    std::atomic<bool> has_waker;

};

Token get_token(Event event) {
    return event.data.u64;
}

bool is_readable(Event event) {
    return (event.events & EPOLLIN) || (event.events & EPOLLPRI);
}

bool is_writable(Event event) {
    return event.events & EPOLLOUT;
}

bool is_priority(Event event) {
    return event.events & EPOLLPRI;
}

bool is_error(Event event) {
    return event.events & EPOLLERR;
}

bool is_read_closed(Event event) {
    return event.events & EPOLLHUP || (event.events & EPOLLIN && event.events & EPOLLHUP);
}

bool is_write_closed(Event event) {
    return event.events & EPOLLHUP || (event.events & EPOLLOUT && event.events & EPOLLERR) ||
        (event.events & EPOLLERR);
}

class IOSourceState {
  public:
    IOSourceState() = default;

    template<typename T>
    using IOCallback = std::function<int(T)>;

    template<typename T>
    int do_io(IOCallback<T> f, T &io) {
        return f(io);
    }
};

using TcpSocket = int;

TcpSocket new_v4_socket();
TcpSocket new_v6_socket();
int bind(TcpSocket socket, const sockaddr *addr);
int connect(TcpSocket socket, const sockaddr *addr);
std::pair<TcpSocket, const sockaddr *> accept(TcpSocket listener);
int listen(TcpSocket socket, int backlog);
int close(TcpSocket socket);
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

#endif //SIMIO_INCLUDE_SIMIO_UNIX_H_
