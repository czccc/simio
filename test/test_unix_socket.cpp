//
// Created by Cheng on 2021/3/5.
//

#include <iostream>
#include "sys/socket.h"
#include <cerrno>
#include "simio.h"

#include <cstring>
#include <thread>
#include <pthread.h>
#include <unistd.h>

using namespace std;
using simio::SocketAddr;

extern int errno;

void server(SocketAddr addr_, int timeout) {

    const sockaddr *addr = addr_.get();
    int flag = 0;
    flag |= SOCK_STREAM;
    // flag |= SOCK_NONBLOCK;
    // flag |= SOCK_CLOEXEC;
    int socket = ::socket(AF_INET, flag, 0);
    cout << "Server socket_: " << socket << endl;
    int bind_ret = ::bind(socket, addr, sizeof(*addr));
    cout << "Server bind: " << bind_ret << endl;
    if (bind_ret < 0) {
        cout << "Server errno: " << strerror(errno) << endl;
    }
    int listen_ret = ::listen(socket, 1024);
    cout << "Server listen: " << listen_ret << endl;
    if (listen_ret < 0) {
        cout << "Server errno: " << strerror(errno) << endl;
    }
    cout << "Server start accept..." << endl;
    sockaddr ret_addr{};
    unsigned int len = 0;
    int new_socket = -1;
    while (1) {
        // while (new_socket < 0) {
        new_socket = ::accept4(socket, &ret_addr, &len, 0);
        if (new_socket < 0) {
            cout << "Server errno: " << strerror(errno) << endl;
        }
        // sleep(1);
        SocketAddr new_addr(ret_addr);
        cout << "Server accept new socket_: " << new_socket << " addr: " << new_addr << endl;

        ::getsockname(new_socket, &ret_addr, &len);
        cout << "Server local addr: " << simio::SocketAddr(ret_addr) << " len: " << len << endl;

        ::getpeername(new_socket, &ret_addr, &len);
        cout << "Server peer addr: " << simio::SocketAddr(ret_addr) << " len: " << len << endl;

        close(new_socket);
    }

    // SocketAddr new_addr(ret_addr);
    // cout << "Server accept new addr: " << new_addr << endl;
    //
    // cout << ::getsockname(new_socket, &ret_addr, &len) << endl;
    // cout << "local addr: " << simio::SocketAddr(ret_addr) << " len: " << len << endl;
    //
    // cout << ::getpeername(new_socket, &ret_addr, &len) << endl;
    // cout << "peer addr: " << simio::SocketAddr(ret_addr) << " len: " << len << endl;
}

void client(SocketAddr addr_, int timeout) {

    const sockaddr *addr = addr_.get();
    int flag = 0;
    flag |= SOCK_STREAM;
    // flag |= SOCK_NONBLOCK;
    // flag |= SOCK_CLOEXEC;
    while (true) {
        sleep(timeout);
        int socket = ::socket(AF_INET, flag, 0);
        cout << "Client socket_: " << socket << endl;
        int connect_ret = ::connect(socket, addr, sizeof(*addr));
        if (connect_ret < 0) {
            cout << "Client errno: " << strerror(errno) << endl;
        }
        cout << "Client connect " << addr_ << " ok" << endl;

        close(socket);
    }

}

int main() {
    // extern int errno;
    SocketAddr addr("0.0.0.0:8994");
    SocketAddr addr1("0.0.0.0:8995");
    // SocketAddr addr("127.0.0.1:8994");
    thread t1(server, addr, 2);
    thread t2(client, addr, 3);

    thread t3(server, addr1, 3);
    thread t4(client, addr1, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

}