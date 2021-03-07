//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_NET_TCP_STREAM_H_
#define SIMIO_INCLUDE_SIMIO_NET_TCP_STREAM_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>

#include "simio/base/noncopyable.h"
#include "simio/base/socket_addr.h"
#include "simio/net/tcp_socket.h"
#include "simio/sys.h"
#include "simio/io_source.h"

namespace simio {

class TcpStream : public EventSource {
  public:

    explicit TcpStream(TcpSocketPtr &socket) : io_source_inner_(socket) {}
    TcpStream(const TcpStream &rhs) noexcept: io_source_inner_(rhs.io_source_inner_) {}
    TcpStream &operator=(const TcpStream &rhs) noexcept {
        if (this != &rhs) {
            io_source_inner_ = rhs.io_source_inner_;
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

    // int as_raw_fd() const { return inner_.get_inner()->as_raw_fd(); }

    IOSource<TcpSocketPtr> io_source_inner_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_NET_TCP_STREAM_H_
