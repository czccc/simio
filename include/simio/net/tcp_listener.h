//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_NET_TCP_LISTENER_H_
#define SIMIO_INCLUDE_SIMIO_NET_TCP_LISTENER_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>

#include "simio/sys.h"
#include "simio/io_source.h"

#include "simio/base/socket_addr.h"
#include "simio/net/tcp_socket.h"

namespace simio {

class TcpStream;

class TcpListener : public EventSource {
  public:
    explicit TcpListener(TcpSocketPtr &socket) : io_source_inner_(socket) {}

    TcpListener(const TcpListener &rhs) : io_source_inner_(rhs.io_source_inner_) {}
    TcpListener &operator=(const TcpListener &rhs) noexcept {
        if (this != &rhs) {
            io_source_inner_ = rhs.io_source_inner_;
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

    IOSource<TcpSocketPtr> io_source_inner_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_NET_TCP_LISTENER_H_
