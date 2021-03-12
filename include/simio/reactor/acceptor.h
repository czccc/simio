//
// Created by Cheng on 2021/3/9.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_ACCEPTOR_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_ACCEPTOR_H_

#include <utility>
#include <functional>

#include "simio/net/tcp_listener.h"
#include "simio/net/tcp_stream.h"
#include "simio/reactor/task.h"

namespace simio {
class Acceptor {
  public:
    explicit Acceptor(const SocketAddr &addr)
        : listener_(TcpListener::bind(addr)) {
        init();
    }
    explicit Acceptor(const std::string &addr)
        : listener_(TcpListener::bind(addr)) {
        init();
    }

    void init() {
        poller_->get_registry()->event_register(&listener_, 0, Interest::READABLE());
        build_io_task = [](std::pair<simio::TcpStream, simio::SocketAddr> new_accept) -> IOStreamTask<TcpStream> {
            return IOStreamTask<TcpStream>(new_accept.first);
        };
    }

    void start() {
        while (true) {
            poller_->poll(events, 1000);
            // if (!events_.as_vec().empty()) {
            //     std::cout << "server polled " << events_.as_vec().size() << " events_" << std::endl;
            // }
            // std::cout << "server polled " << events_.as_vec().size() << " events_" << std::endl;
            for (auto &&ev : events.as_vec()) {
                switch (Event::from_sys_event(ev).token()) {
                    case 0:
                        try {
                            auto new_accept = listener_.accept();
                            std::cout << "server new connection from addr: " << new_accept.second << " "
                                      << new_accept.first
                                      << std::endl;
                            IOStreamTask<TcpStream> task = build_io_task(new_accept);
                        } catch (std::system_error &e) {
                            std::cout << "server" << e.what() << std::endl;
                        }
                        break;
                    default:break;
                }
            }
        }
    }

    void spawn_task(IOStreamTask<TcpStream> task) {

    }

  private:
    EventList events{1024};
    std::unique_ptr<Poll> poller_{};
    TcpListener listener_;

    std::function<IOStreamTask<TcpStream>(std::pair<simio::TcpStream, simio::SocketAddr>)>
        build_io_task{
        [](std::pair<simio::TcpStream, simio::SocketAddr> new_accept) -> IOStreamTask<TcpStream> {
            return IOStreamTask<TcpStream>(new_accept.first);
        }
    };
};
}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_ACCEPTOR_H_
