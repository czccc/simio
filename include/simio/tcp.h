//
// Created by Cheng on 2021/3/4.
//

#ifndef SIMIO_INCLUDE_SIMIO_TCP_H_
#define SIMIO_INCLUDE_SIMIO_TCP_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include "unix.h"
#include "io_source.h"

namespace simio {

class TcpSocket {
  public:
    explicit TcpSocket(sys::TcpSocket socket) : socket(socket) {}
    ~TcpSocket() {
        sys::close(socket);
    }

    static TcpSocket new_v4() { return TcpSocket(sys::new_v4_socket()); }
    static TcpSocket new_v6() { return TcpSocket(sys::new_v6_socket()); }
    static TcpSocket new_for_addr(SocketAddr addr) {
        if (addr.is_ipv4()) {
            return new_v4();
        } else {
            return new_v6();
        }
    }

    int as_raw_fd() const {
        return (int) socket;
    }

    int bind(const sockaddr *addr) const {
        return sys::bind(socket, addr);
    }
    int connect(const sockaddr *addr) const {
        // TODO: return TcpStream
        return sys::connect(socket, addr);
    }
    int listen(int backlog) const {
        // TODO: return TcpListener
        return sys::listen(socket, backlog);
    }
    bool set_reuse_addr(bool reuseaddr) const {
        return sys::set_reuseaddr(socket, reuseaddr);
    }
    bool get_reuse_addr() const {
        return sys::get_reuseaddr(socket);
    }
    bool set_reuse_port(bool reuseport) const {
        return sys::set_reuse_port(socket, reuseport);
    }
    bool get_reuse_port() const {
        return sys::get_reuse_port(socket);
    }
    // SocketAddr get_local_addr(TcpSocket socket);

    bool set_linger(int dur) const {
        return sys::set_linger(socket, dur);
    }
    int get_linger() const {
        return sys::get_linger(socket);
    }
    bool set_recv_buffer_size(int size) const {
        return sys::set_recv_buffer_size(socket, size);
    }
    int get_recv_buffer_size() const {
        return sys::get_recv_buffer_size(socket);
    }
    bool set_send_buffer_size(int size) const {
        return sys::set_send_buffer_size(socket, size);
    }
    int get_send_buffer_size() const {
        return sys::get_send_buffer_size(socket);
    }
    bool set_keepalive(bool keepalive) const {
        return sys::set_keepalive(socket, keepalive);
    }
    bool get_keepalive() const {
        return sys::get_keepalive(socket);
    }
    int set_keepalive_params(sys::TcpKeepalive keepalive) const {
        return sys::set_keepalive_params(socket, keepalive);
    }
    bool set_keepalive_time(int time) const {
        return sys::set_keepalive_time(socket, time);
    }
    int get_keepalive_time() const {
        return sys::get_keepalive_time(socket);
    }
    bool set_keepalive_interval(int interval) const {
        return sys::set_keepalive_interval(socket, interval);
    }
    int get_keepalive_interval() const {
        return sys::get_keepalive_interval(socket);
    }
    bool set_keepalive_retries(int retries) const {
        return sys::set_keepalive_retries(socket, retries);
    }
    int get_keepalive_retries() const {
        return sys::get_keepalive_retries(socket);
    }
  private:
    sys::TcpSocket socket;
};

class TcpStream {
  public:
    static TcpStream connect(SocketAddr addr);
    static TcpStream from_socket(const TcpSocket &socket);

    SocketAddr peer_addr();
    SocketAddr local_addr();
    bool shutdown(bool read, bool write);
    bool set_nodelay(bool nodelay);
    bool get_nodelay();
    bool set_nonblocking(bool nonblocking);
  private:
    explicit TcpStream(const TcpSocket &socket) : inner(socket) {}
    TcpStream(TcpStream &&rhs) noexcept: inner(std::move(rhs.inner)) {}
    TcpStream &operator=(TcpStream &&rhs) noexcept {
        if (this != &rhs) {
            inner = std::move(rhs.inner);
        }
        return *this;
    }

    int as_raw_fd() { return inner.get_inner().as_raw_fd(); }

    IOSource<TcpSocket> inner;
};

class TcpListener {
  public:
    static TcpListener bind(SocketAddr addr);
    static TcpListener from_socket(const TcpSocket &socket);

    std::pair<TcpSocket, SocketAddr> accept();
    SocketAddr local_addr();
    bool set_ttl(int ttl);
    int get_ttl();
    bool set_nonblocking(bool nonblocking);

  private:
    explicit TcpListener(const TcpSocket &socket) : inner(socket) {}
    TcpListener(TcpListener &&rhs) noexcept: inner(std::move(rhs.inner)) {}
    TcpListener &operator=(TcpListener &&rhs) noexcept {
        if (this != &rhs) {
            inner = std::move(rhs.inner);
        }
        return *this;
    }

    int as_raw_fd() { return inner.get_inner().as_raw_fd(); }

    IOSource<TcpSocket> inner;
};

}

#endif //SIMIO_INCLUDE_SIMIO_TCP_H_
