//
// Created by Cheng on 2021/3/4.
//

#ifndef SIMIO_INCLUDE_SIMIO_TCP_H_
#define SIMIO_INCLUDE_SIMIO_TCP_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>
#include "unix.h"
#include "io_source.h"

namespace simio {

class TcpStream;

class TcpListener;

class TcpSocket;

using TcpSocketPtr = std::shared_ptr<TcpSocket>;

class TcpSocket : noncopyable {
  public:
    friend class IOSource<TcpSocketPtr>;

    explicit TcpSocket(sys::TcpSocket socket) : socket(socket) {}
    TcpSocket(TcpSocket &&rhs) noexcept: socket(rhs.socket) {}
    TcpSocket &operator=(TcpSocket &&rhs) noexcept {
        if (this != &rhs) { socket = rhs.socket; }
        return *this;
    }
    ~TcpSocket() { close(); }

    friend std::ostream &operator<<(std::ostream &output,
                                    const TcpSocket &D) {
        output << "TcpSocketPtr(" << D.socket << ")";
        return output;
    }

    static TcpSocketPtr new_v4() { return std::make_shared<TcpSocket>(sys::new_v4_socket()); }
    static TcpSocketPtr new_v6() { return std::make_shared<TcpSocket>(sys::new_v6_socket()); }
    static TcpSocketPtr new_for_addr(const SocketAddr &addr) { return addr.is_ipv4() ? new_v4() : new_v6(); }

    bool bind(const SocketAddr &addr) const { return sys::bind(socket, addr); }
    bool connect(const SocketAddr &addr) const { return sys::connect(socket, addr); }
    bool listen(int backlog) const { return sys::listen(socket, backlog); }
    std::pair<TcpSocketPtr, simio::SocketAddr> accept() const {
        auto ret = sys::accept(socket);
        TcpSocketPtr p = std::make_shared<TcpSocket>(ret.first);
        return std::make_pair(p, ret.second);
    }

    int send(const std::vector<char> &buf, int flag = 0) const { return sys::send(socket, buf, flag); }
    int recv(std::vector<char> &buf, int flag = 0) const { return sys::recv(socket, buf, flag); }

    int send(const std::string &buf, int flag = 0) const { return sys::send(socket, buf, flag); }
    int recv(std::string &buf, int flag = 0) const { return sys::recv(socket, buf, flag); }

    bool close() const { return sys::close(socket); }
    bool shutdown(bool read, bool write) const { return sys::shutdown(socket, read, write); }

    bool set_reuse_addr(bool reuseaddr) const { return sys::set_reuseaddr(socket, reuseaddr); }
    bool get_reuse_addr() const { return sys::get_reuseaddr(socket); }

    bool set_reuse_port(bool reuseport) const { return sys::set_reuse_port(socket, reuseport); }
    bool get_reuse_port() const { return sys::get_reuse_port(socket); }

    SocketAddr get_local_addr() const {
        SocketAddr addr = sys::get_local_addr(socket);
        return addr;
    }
    SocketAddr get_peer_addr() const {
        SocketAddr addr = sys::get_peer_addr(socket);
        return addr;
    }

    bool set_ttl(int ttl) const { return sys::set_ttl(socket, ttl); }
    int get_ttl() const { return sys::get_ttl(socket); }

    bool set_linger(int dur) const { return sys::set_linger(socket, dur); }
    int get_linger() const { return sys::get_linger(socket); }

    bool set_nodelay(bool nodelay) const { return sys::set_nodelay(socket, nodelay); }
    int get_nodelay() const { return sys::get_nodelay(socket); }

    bool set_nonblocking(bool nonblocking) const { return sys::set_nonblocking(socket, nonblocking); }

    bool set_recv_buffer_size(int size) const { return sys::set_recv_buffer_size(socket, size); }
    int get_recv_buffer_size() const { return sys::get_recv_buffer_size(socket); }

    bool set_send_buffer_size(int size) const { return sys::set_send_buffer_size(socket, size); }
    int get_send_buffer_size() const { return sys::get_send_buffer_size(socket); }

    bool set_keepalive(bool keepalive) const { return sys::set_keepalive(socket, keepalive); }
    bool get_keepalive() const { return sys::get_keepalive(socket); }

    int set_keepalive_params(sys::TcpKeepalive keepalive) const { return sys::set_keepalive_params(socket, keepalive); }

    bool set_keepalive_time(int time) const { return sys::set_keepalive_time(socket, time); }
    int get_keepalive_time() const { return sys::get_keepalive_time(socket); }

    bool set_keepalive_interval(int interval) const { return sys::set_keepalive_interval(socket, interval); }
    int get_keepalive_interval() const { return sys::get_keepalive_interval(socket); }

    bool set_keepalive_retries(int retries) const { return sys::set_keepalive_retries(socket, retries); }
    int get_keepalive_retries() const { return sys::get_keepalive_retries(socket); }
  private:
    int as_raw_fd() const { return (int) socket; }
    sys::TcpSocket socket;
};

class TcpStream : public EventSource {
  public:

    explicit TcpStream(TcpSocketPtr &socket) : inner(socket) {}
    TcpStream(const TcpStream &rhs) noexcept: inner(rhs.inner) {}
    TcpStream &operator=(const TcpStream &rhs) noexcept {
        if (this != &rhs) {
            inner = rhs.inner;
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &output,
                                    const TcpStream &ss) {
        std::string str_local = ss.local_addr().build_str();
        std::string str_peer = ss.peer_addr().build_str();
        output << "TcpStream(" << str_local << "<->" << str_peer << ")";
        return output;
    }

    static TcpStream connect(SocketAddr addr);
    static TcpStream from_socket(TcpSocketPtr &socket);

    void event_register(Registry *registry, Token token, Interest interest) override;
    void event_reregister(Registry *registry, Token token, Interest interest) override;
    void event_deregister(Registry *registry) override;

    SocketAddr peer_addr() const;
    SocketAddr local_addr() const;

    bool shutdown(bool read, bool write);
    bool set_nodelay(bool nodelay);
    bool get_nodelay();
    bool set_nonblocking(bool nonblocking);
    bool set_ttl(bool ttl);
    bool get_ttl();

    int send(const std::vector<char> &buf);
    int send(const std::string &buf);
    int recv(std::vector<char> &buf);
    int recv(std::string &buf);
    // int peek(std::vector<char> &buf);
    // int read(std::vector<char> &buf);
    // int write(const std::vector<char> &buf);
    // int read_vectored(std::vector<struct iovec> &buf);
    // int write_vectored(const std::vector<struct iovec> &buf);

  private:

    // int as_raw_fd() const { return inner.get_inner()->as_raw_fd(); }

    IOSource<TcpSocketPtr> inner;
};

class TcpListener : public EventSource {
  public:
    explicit TcpListener(TcpSocketPtr &socket) : inner(socket) {}

    TcpListener(const TcpListener &rhs) : inner(rhs.inner) {}
    TcpListener &operator=(const TcpListener &rhs) noexcept {
        if (this != &rhs) {
            inner = rhs.inner;
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &output,
                                    const TcpListener &ss) {
        std::string str = ss.local_addr().build_str();
        output << "TcpListener(" << str << ")";
        return output;
    }

    static TcpListener bind(SocketAddr addr);
    static TcpListener bind(const std::string &addr);
    static TcpListener from_socket(TcpSocketPtr socket);

    void event_register(Registry *registry, Token token, Interest interest) override;
    void event_reregister(Registry *registry, Token token, Interest interest) override;
    void event_deregister(Registry *registry) override;

    std::pair<TcpStream, SocketAddr> accept();
    SocketAddr local_addr() const;
    bool set_ttl(int ttl);
    int get_ttl();
    bool set_nonblocking(bool nonblocking);

  private:

    IOSource<TcpSocketPtr> inner;
};

}

#endif //SIMIO_INCLUDE_SIMIO_TCP_H_
