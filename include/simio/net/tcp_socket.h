//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_NET_TCP_SOCKET_H_
#define SIMIO_INCLUDE_SIMIO_NET_TCP_SOCKET_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>

#include "simio/base/noncopyable.h"
#include "simio/base/socket_addr.h"
#include "simio/sys.h"
#include "simio/io_source.h"

namespace simio {

class TcpSocket;

using TcpSocketPtr = std::shared_ptr<TcpSocket>;

class TcpSocket : noncopyable {
  public:
    friend class IOSource<TcpSocketPtr>;

    explicit TcpSocket(sys::TcpSocket socket) : socket_(socket) {}
    TcpSocket(TcpSocket &&rhs) noexcept: socket_(rhs.socket_) {}
    TcpSocket &operator=(TcpSocket &&rhs) noexcept {
        if (this != &rhs) { socket_ = rhs.socket_; }
        return *this;
    }
    ~TcpSocket() { close(); }

    friend std::ostream &operator<<(std::ostream &output,
                                    const TcpSocket &D) {
        output << "TcpSocketPtr(" << D.socket_ << ")";
        return output;
    }

    static TcpSocketPtr new_v4() { return std::make_shared<TcpSocket>(sys::new_v4_socket()); }
    static TcpSocketPtr new_v6() { return std::make_shared<TcpSocket>(sys::new_v6_socket()); }
    static TcpSocketPtr new_for_addr(const SocketAddr &addr) { return addr.is_ipv4() ? new_v4() : new_v6(); }

    bool bind(const SocketAddr &addr) const { return sys::bind(socket_, addr); }
    bool connect(const SocketAddr &addr) const { return sys::connect(socket_, addr); }
    bool listen(int backlog) const { return sys::listen(socket_, backlog); }
    std::pair<TcpSocketPtr, simio::SocketAddr> accept() const {
        auto ret = sys::accept(socket_);
        TcpSocketPtr p = std::make_shared<TcpSocket>(ret.first);
        return std::make_pair(p, ret.second);
    }

    int send(const std::vector<char> &buf, int flag = 0) const { return sys::send(socket_, buf, flag); }
    int recv(std::vector<char> &buf, int flag = 0) const { return sys::recv(socket_, buf, flag); }

    int send(const std::string &buf, int flag = 0) const { return sys::send(socket_, buf, flag); }
    int recv(std::string &buf, int flag = 0) const { return sys::recv(socket_, buf, flag); }

    bool close() const { return sys::close(socket_); }
    bool shutdown(bool read, bool write) const { return sys::shutdown(socket_, read, write); }

    bool set_reuse_addr(bool reuseaddr) const { return sys::set_reuseaddr(socket_, reuseaddr); }
    bool get_reuse_addr() const { return sys::get_reuseaddr(socket_); }

    bool set_reuse_port(bool reuseport) const { return sys::set_reuse_port(socket_, reuseport); }
    bool get_reuse_port() const { return sys::get_reuse_port(socket_); }

    SocketAddr get_local_addr() const {
        SocketAddr addr = sys::get_local_addr(socket_);
        return addr;
    }
    SocketAddr get_peer_addr() const {
        SocketAddr addr = sys::get_peer_addr(socket_);
        return addr;
    }

    bool set_ttl(int ttl) const { return sys::set_ttl(socket_, ttl); }
    int get_ttl() const { return sys::get_ttl(socket_); }

    bool set_linger(int dur) const { return sys::set_linger(socket_, dur); }
    int get_linger() const { return sys::get_linger(socket_); }

    bool set_nodelay(bool nodelay) const { return sys::set_nodelay(socket_, nodelay); }
    int get_nodelay() const { return sys::get_nodelay(socket_); }

    bool set_nonblocking(bool nonblocking) const { return sys::set_nonblocking(socket_, nonblocking); }

    bool set_recv_buffer_size(int size) const { return sys::set_recv_buffer_size(socket_, size); }
    int get_recv_buffer_size() const { return sys::get_recv_buffer_size(socket_); }

    bool set_send_buffer_size(int size) const { return sys::set_send_buffer_size(socket_, size); }
    int get_send_buffer_size() const { return sys::get_send_buffer_size(socket_); }

    bool set_keepalive(bool keepalive) const { return sys::set_keepalive(socket_, keepalive); }
    bool get_keepalive() const { return sys::get_keepalive(socket_); }

    int set_keepalive_params(sys::TcpKeepalive keepalive) const {
        return sys::set_keepalive_params(socket_,
                                         keepalive);
    }

    bool set_keepalive_time(int time) const { return sys::set_keepalive_time(socket_, time); }
    int get_keepalive_time() const { return sys::get_keepalive_time(socket_); }

    bool set_keepalive_interval(int interval) const { return sys::set_keepalive_interval(socket_, interval); }
    int get_keepalive_interval() const { return sys::get_keepalive_interval(socket_); }

    bool set_keepalive_retries(int retries) const { return sys::set_keepalive_retries(socket_, retries); }
    int get_keepalive_retries() const { return sys::get_keepalive_retries(socket_); }
  private:
    int as_raw_fd() const { return (int) socket_; }
    sys::TcpSocket socket_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_NET_TCP_SOCKET_H_
