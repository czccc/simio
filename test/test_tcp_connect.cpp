//
// Created by Cheng on 2021/3/6.
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

const SocketAddr addr("127.0.0.1:5679");

// int server() {
//     TcpListener listener = TcpListener::bind(addr);
//     // std::pair<TcpSocketPtr, SocketAddr> ret;
//     // ret = listener.accept();
// }


void ping_pong(TcpStream stream, Poll &poll, Event ev, const string &pre) {
    if (ev.is_readable()) {
        // std::vector<char> buf(20);
        string buf(20, 0);
        int len = stream.recv(buf);
        std::string msg(buf);
        cout << pre << " " << stream << " read  len: " << len << " msg: " << msg << endl;
        // poll.get_registry()->event_reregister(&stream, ev.token(), Interest::WRITABLE());
    }
    if (ev.is_writable()) {
        // std::vector<char> buf{'h', 'e', 'l', 'l', 'o'};
        string buf("hello, ");
        buf += pre;
        int len = stream.send(buf);
        std::string msg(buf);
        cout << pre << " " << stream << " write len: " << len << " msg: " << msg << endl;
        // poll.get_registry()->event_reregister(&stream, ev.token(), Interest::READABLE());
    } else {
        cout << pre << " " << stream << " neither read or write" << endl;
    }
    usleep(500000 + rand() % 50000);
}

void client_loop() {
    sleep(2);

    TcpStream client = TcpStream::connect(addr);
    // auto new_accept = listener.accept();
    std::cout << "client connecting to " << addr << std::endl;

    Token client_token = 1;

    Poll poll{};
    EventList events(10);
    poll.get_registry()->event_register(&client, client_token, Interest::WRITABLE() | Interest::READABLE());

    std::cout << "client Poll id: " << poll.get_registry()->get_selector().get_id() << " start loop.." << std::endl;

    while (true) {
        poll.poll(events, 1000);
        if (!events.as_vec().empty()) {
            // std::cout << "client polled " << events.as_vec().size() << " events" << std::endl;
        }
        // std::cout << "client polled " << events.as_vec().size() << " events" << std::endl;
        for (auto &&ev : events.as_vec()) {
            switch (Event::from_sys_event(ev).token()) {
                case 1:
                    try {
                        // auto new_accept = listener.accept();
                        // std::cout << "client connection to addr: " << addr << " " << client << std::endl;
                        ping_pong(client, poll, Event::from_sys_event(ev), "client");
                    } catch (std::system_error &e) {
                        std::cout << "client " << e.what() << std::endl;
                    }
                    break;
                default:break;
            }
        }

    }

}

void server() {

    TcpListener listener = TcpListener::bind(addr);
    Token listener_token = 0;

    std::map<Token, TcpStream> m{};
    Token next_token = 2;

    std::cout << "server listening to " << addr << std::endl;
    Poll poll{};
    EventList events(10);
    poll.get_registry()->event_register(&listener, listener_token, Interest::READABLE() | Interest::WRITABLE());

    std::cout << "server Poll id: " << poll.get_registry()->get_selector().get_id() << " start loop.." << std::endl;

    while (true) {
        poll.poll(events, 1000);
        if (!events.as_vec().empty()) {
            // std::cout << "server polled " << events.as_vec().size() << " events" << std::endl;
        }
        // std::cout << "server polled " << events.as_vec().size() << " events" << std::endl;
        for (auto &&ev : events.as_vec()) {
            switch (Event::from_sys_event(ev).token()) {
                case 0:
                    try {
                        auto new_accept = listener.accept();
                        std::cout << "server new connection from addr: " << new_accept.second << " " << new_accept.first
                                  << std::endl;
                        poll.get_registry()->event_register(&new_accept.first,
                                                            next_token,
                                                            Interest::WRITABLE());
                        m.insert({next_token, new_accept.first});
                        next_token++;
                    } catch (std::system_error &e) {
                        std::cout << "server" << e.what() << std::endl;
                    }
                    break;
                default:
                    try {
                        Token t = Event::from_sys_event(ev).token();
                        auto s = m.find(t);
                        ping_pong(s->second, poll, Event::from_sys_event(ev), "server");
                    } catch (std::system_error &e) {
                        std::cout << "server" << e.what() << std::endl;
                    }
                    break;
            }
        }
    }
}

int main() {
    thread t1(client_loop);
    thread t2(server);

    t1.join();
    t2.join();
}