//
// Created by Cheng on 2021/3/4.
//


#include "simio/net/tcp_stream.h"

simio::TcpStream simio::TcpStream::connect(simio::SocketAddr addr) {
    TcpSocketPtr socket = TcpSocket::new_for_addr(addr);
    try {
        socket->connect(addr);
    } catch (std::runtime_error &e) {

    }
    return TcpStream(socket);
}

simio::TcpStream simio::TcpStream::from_socket(simio::TcpSocketPtr &socket) {
    return TcpStream(socket);
}

simio::SocketAddr simio::TcpStream::peer_addr() const {
    return io_source_inner_.get_inner()->get_peer_addr();
}
simio::SocketAddr simio::TcpStream::local_addr() const {
    return io_source_inner_.get_inner()->get_local_addr();
}
bool simio::TcpStream::shutdown(bool read, bool write) {
    return io_source_inner_.get_inner()->shutdown(read, write);
}

bool simio::TcpStream::set_nodelay(bool nodelay) {
    return io_source_inner_.get_inner()->set_nodelay(nodelay);
}
bool simio::TcpStream::get_nodelay() {
    return io_source_inner_.get_inner()->get_nodelay();
}
bool simio::TcpStream::set_nonblocking(bool nonblocking) {
    return io_source_inner_.get_inner()->set_nonblocking(nonblocking);
}

bool simio::TcpStream::set_ttl(bool ttl) {
    return io_source_inner_.get_inner()->set_ttl(ttl);
}
bool simio::TcpStream::get_ttl() {
    return io_source_inner_.get_inner()->get_ttl();
}
//
// int simio::TcpStream::peek(std::vector<char> &buf) {
//     auto call_back = [&](int fd) -> int { return sys::peek(fd, (void *) buf.data(), buf.size()); };
//     return inner_.do_io<decltype(call_back), int>(call_back);
// }
int simio::TcpStream::send(const std::vector<char> &buf) {
    auto call_back = [&](const simio::TcpSocketPtr &socket_ptr) -> int { return socket_ptr->send(buf); };
    return io_source_inner_.do_io<decltype(call_back), int>(call_back);
}
int simio::TcpStream::send(const std::string &buf) {
    auto call_back = [&](const simio::TcpSocketPtr &socket_ptr) -> int { return socket_ptr->send(buf); };
    return io_source_inner_.do_io<decltype(call_back), int>(call_back);
}
int simio::TcpStream::recv(std::vector<char> &buf) {
    auto call_back = [&](const simio::TcpSocketPtr &socket_ptr) -> int { return socket_ptr->recv(buf); };
    return io_source_inner_.do_io<decltype(call_back), int>(call_back);
}
int simio::TcpStream::recv(std::string &buf) {
    auto call_back = [&](const simio::TcpSocketPtr &socket_ptr) -> int { return socket_ptr->recv(buf); };
    return io_source_inner_.do_io<decltype(call_back), int>(call_back);
}
// int simio::TcpStream::read(std::vector<char> &buf) {
//     auto call_back =
//         [&](const simio::TcpSocketPtr &socket_ptr) -> int { return sys::read(fd, (void *) buf.data(), buf.size()); };
//     return inner_.do_io<decltype(call_back), int>(call_back);
// }
// int simio::TcpStream::write(const std::vector<char> &buf) {
//     auto call_back =
//         [&](const simio::TcpSocketPtr &socket_ptr) -> int { return sys::write(fd, (void *) buf.data(), buf.size()); };
//     return inner_.do_io<decltype(call_back), int>(call_back);
// }
// int simio::TcpStream::read_vectored(std::vector<struct iovec> &buf) {
//     auto call_back =
//         [&](const simio::TcpSocketPtr &socket_ptr) -> int { return sys::read_vectored(fd, buf.data(), buf.size()); };
//     return inner_.do_io<decltype(call_back), int>(call_back);
// }
// int simio::TcpStream::write_vectored(const std::vector<struct iovec> &buf) {
//     auto call_back =
//         [&](const simio::TcpSocketPtr &socket_ptr) -> int { return sys::write_vectored(fd, buf.data(), buf.size()); };
//     return inner_.do_io<decltype(call_back), int>(call_back);
// }

void simio::TcpStream::event_register(simio::Registry *registry, simio::Token token, simio::Interest interest) {
    io_source_inner_.event_register(registry, token, interest);
}
void simio::TcpStream::event_reregister(simio::Registry *registry, simio::Token token, simio::Interest interest) {
    io_source_inner_.event_reregister(registry, token, interest);
}
void simio::TcpStream::event_deregister(simio::Registry *registry) {
    io_source_inner_.event_deregister(registry);
}