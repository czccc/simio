//
// Created by Cheng on 2021/3/4.
//

#include "simio.h"

simio::TcpStream simio::TcpStream::connect(simio::SocketAddr addr) {
    TcpSocket socket = TcpSocket::new_for_addr(addr);
    return TcpStream(socket);
}
simio::TcpStream simio::TcpStream::from_socket(const simio::TcpSocket &socket) {
    return TcpStream(socket);
}
simio::SocketAddr simio::TcpStream::peer_addr() {
    SocketAddr addr = sys::get_peer_addr(as_raw_fd());
    return addr;
}
simio::SocketAddr simio::TcpStream::local_addr() {
    SocketAddr addr = sys::get_local_addr(as_raw_fd());
    return addr;
}
bool simio::TcpStream::shutdown(bool read, bool write) {
    return sys::shutdown(as_raw_fd(), read, write);
}

bool simio::TcpStream::set_nodelay(bool nodelay) {
    return sys::set_nodelay(as_raw_fd(), nodelay);
}
bool simio::TcpStream::get_nodelay() {
    return sys::get_nodelay(as_raw_fd());
}
bool simio::TcpStream::set_nonblocking(bool nonblocking) {
    return sys::set_nonblocking(as_raw_fd(), nonblocking);
}
