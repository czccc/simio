//
// Created by Cheng on 2021/3/21.
//


#include "simio.h"

#include <iostream>
#include <cerrno>

#include <cstring>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <map>

using namespace simio;
using namespace std;

Task on_accept(const std::pair<simio::TcpStream, simio::SocketAddr> &pair) {
    cout << "Tcp accept: " << pair.second << endl;
    return Task(pair.first);
}

void discard(Task *task) {
    TcpStream stream = task->inner_;
    string s(1024, '\0');
    int num = stream.recv(s);
    if (num == 0) {
        cout << "server close" << endl;
        task->close();
        return;
    }
    cout << "server recv: " << s << endl;
}

void client(int index) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    TcpStream stream = TcpStream::connect(SocketAddr("127.0.0.1:5000"));
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        string buf("Client");
        buf.push_back(char(index + '0'));
        buf.append(" send the number: ");
        buf.push_back(char(i + '0'));
        stream.send(buf);
        cout << "client send: " << buf << endl;
    }
}

int main() {
    std::vector<std::thread> vec{};
    for (int i = 1; i < 5; i++) {
        vec.emplace_back(client, i);
    }

    SocketAddr addr("127.0.0.1:5000");
    TcpServer server(addr);
    server.set_accept_callback(on_accept);
    server.set_stream_callback(discard);
    server.start();

}
