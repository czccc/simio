//
// Created by Cheng on 2021/3/4.
//

#include "simio/net/tcp_listener.h"
#include "simio/net/tcp_stream.h"

simio::TcpListener simio::TcpListener::bind(const simio::SocketAddr &addr) {
    TcpSocketPtr socket(TcpSocket::new_for_addr(addr));
    socket->set_reuse_addr(true);
    socket->bind(addr);
    socket->listen(1024);
    return TcpListener(socket);
}

simio::TcpListener simio::TcpListener::bind(const std::string &addr) {
    return TcpListener::bind(SocketAddr(addr));
}
simio::TcpListener simio::TcpListener::from_socket(simio::TcpSocketPtr socket) {
    return TcpListener(socket);
}

std::pair<simio::TcpStream, simio::SocketAddr> simio::TcpListener::accept() {
    auto call_back = [&](const simio::TcpSocketPtr &socket_ptr) -> std::pair<simio::TcpStream, simio::SocketAddr> {
        auto ret = socket_ptr->accept();
        return std::make_pair(TcpStream::from_socket(ret.first), ret.second);
    };
    return io_source_inner_.do_io<decltype(call_back), std::pair<simio::TcpStream, simio::SocketAddr>>(call_back);
}

simio::SocketAddr simio::TcpListener::local_addr() const {
    return io_source_inner_.get_inner()->get_local_addr();
}
bool simio::TcpListener::set_ttl(int ttl) {
    return io_source_inner_.get_inner()->set_ttl(ttl);
}
int simio::TcpListener::get_ttl() {
    return io_source_inner_.get_inner()->get_ttl();
}
bool simio::TcpListener::set_nonblocking(bool nonblocking) {
    return io_source_inner_.get_inner()->set_nonblocking(nonblocking);
}

void simio::TcpListener::event_register(simio::Registry *registry, simio::Token token, simio::Interest interest) {
    io_source_inner_.event_register(registry, token, interest);
}
void simio::TcpListener::event_reregister(simio::Registry *registry, simio::Token token, simio::Interest interest) {
    io_source_inner_.event_reregister(registry, token, interest);
}
void simio::TcpListener::event_deregister(simio::Registry *registry) {
    io_source_inner_.event_deregister(registry);
}
