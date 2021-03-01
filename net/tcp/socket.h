//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_NET_TCP_SOCKET_H_
#define SIMIO_NET_TCP_SOCKET_H_

#include "../../sys/unix/tcp.h"
#include "address.h"

namespace simio {
namespace net {

class TcpSocket {
  public:
    explicit TcpSocket(sys::tcp::TcpSocket socket) : socket(socket) {}
    static TcpSocket new_v4() { return TcpSocket(sys::tcp::new_v4_socket()); }
    static TcpSocket new_v6() { return TcpSocket(sys::tcp::new_v4_socket()); }
    /// @brief
    /// @param addr: 
    /// @return
    static TcpSocket new_for_addr(SocketAddr addr) {
        if (addr.is_ipv4()) {
            return new_v4();
        } else {
            return new_v6();
        }
    }

    // static

    void bind(SocketAddr addr) const {
        sys::tcp::bind(this->socket, addr.get());
    }
  private:
    sys::tcp::TcpSocket socket;
};

struct TcpKeepalive {
    int time;
    int interval;
    int retries;
};

}
}

#endif //SIMIO_NET_TCP_SOCKET_H_
