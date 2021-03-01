//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_EPOLL_H
#define SIMIO_EPOLL_H

#include <stdatomic.h>
#include <atomic>
#include <vector>
#include <sys/epoll.h>
#include <memory>

#include "../../../library.h"

namespace simio {

namespace sys {

using Event = struct epoll_event;
using EventList = std::vector<Event>;

class Selector {
  public:

    Selector();

    ~Selector();

    int select(EventList events, int timeout) const;

    void event_register(int fd, Token token, const Interest &interest) const;

    void event_reregister(int fd, Token token, const Interest &interest) const;

    void event_deregister(int fd) const;

    bool register_waker();

    int get_id() const {
        return id;
    }

  private:
    static std::atomic<int> next_id;
    int id;
    int ep;
    std::shared_ptr<std::atomic<bool>> has_waker;

};
Token token(Event event) {
    return Token(event.data.u64);
}

bool is_readable(Event event) {
    return (event.events & EPOLLIN) || (event.events & EPOLLPRI);
}

bool is_writable(Event event) {
    return event.events & EPOLLOUT;
}

bool is_priority(Event event) {
    return event.events & EPOLLPRI;
}

bool is_error(Event event) {
    return event.events & EPOLLERR;
}

bool is_read_closed(Event event) {
    return event.events & EPOLLHUP || (event.events & EPOLLIN && event.events & EPOLLHUP);
}

bool is_write_closed(Event event) {
    return event.events & EPOLLHUP || (event.events & EPOLLOUT && event.events & EPOLLERR) ||
        (event.events & EPOLLERR);
}

}

}

#endif //SIMIO_EPOLL_H
