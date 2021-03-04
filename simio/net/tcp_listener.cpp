//
// Created by Cheng on 2021/3/4.
//

#include "simio.h"

simio::TcpListener simio::TcpListener::bind(simio::SocketAddr addr) {
    TcpSocket socket = TcpSocket::new_for_addr(addr);
    socket.set_reuse_addr(true);
    socket.bind(addr.get());
    socket.listen(1024);
    return TcpListener(socket);
}
simio::TcpListener simio::TcpListener::from_socket(const simio::TcpSocket &socket) {
    return TcpListener(socket);
}

std::pair<simio::TcpSocket, simio::SocketAddr> simio::TcpListener::accept() {
    // TODO
    // return std::pair<TcpSocket, SocketAddr>({});
}

simio::SocketAddr simio::TcpListener::local_addr() {
    SocketAddr addr = sys::get_local_addr(as_raw_fd());
    return addr;
}
bool simio::TcpListener::set_ttl(int ttl) {
    return sys::set_ttl(as_raw_fd(), ttl);
}
int simio::TcpListener::get_ttl() {
    return sys::get_ttl(as_raw_fd());
}
bool simio::TcpListener::set_nonblocking(bool nonblocking) {
    return sys::set_nonblocking(as_raw_fd(), nonblocking);
}